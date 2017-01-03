# __author__ = 'wei'

import os
import pytest
import csv
from x527.sequencer.sequence import TestSequence
from x527.tinyrpc.protocols.jsonrpc import *

sequence = None
raw_sequence = None


def setup_module(module):
    path = os.path.split(__file__)[0]
    plan_path = os.path.join(path, 'short_plan__1.0.csv')
    global sequence
    sequence = TestSequence(plan_path)
    global raw_sequence
    raw_sequence = []
    with open(plan_path, 'rU') as f:
        reader = csv.DictReader(f)
        for row in reader:
            raw_sequence.append(row)

def test_invalid_load():
    with pytest.raises(JSONRPCInvalidParamsError) as e:
        seq = TestSequence('unknown_format.db')
    assert e.value.message.startswith('unsupported sequence db format')
    with pytest.raises(JSONRPCInvalidParamsError) as e:
        seq = TestSequence('non_exist__1.0.csv')
    assert e.value.message.startswith('[Errno 2] No such file or directory')

    with pytest.raises(JSONRPCInvalidParamsError) as e:
        seq = TestSequence('non_exist_1.0.csv')
    assert e.value.message.startswith('the sequence file name should be of the format [name]__[version].csv, are you using a single underscore as the separator between name and version?')

    sequence2 = TestSequence('DEFAULT')
    assert sequence2._sequence == [['verb_' + str(i), 'params_' + str(i)] for i in range(20)]
    path = os.path.split(__file__)[0]
    path = os.path.join(path, '  short_plan.csv')
    with pytest.raises(JSONRPCInvalidParamsError) as e:
        seq = TestSequence(path)
    assert e.value.message.startswith('the sequence file name should be of the format [name]__[version].csv')
    with pytest.raises(JSONRPCInvalidParamsError) as e:
        seq = TestSequence(1)
    assert e.value.message == 'non string as sequence db name'


def test_csv_load():
    assert len(sequence) == 17
    assert len(sequence.groups) == 5
    assert len(sequence.groups[0]) == 2
    assert len(sequence.groups[1]) == 5
    assert len(sequence.groups[2]) == 6
    assert len(sequence.groups[3]) == 2
    assert 'unitStage' in sequence.variables
    assert 'unitstage' not in sequence.variables
    assert sequence.name=='short_plan'
    assert sequence.version=='1.0'


def test_variables():
    assert len(sequence.variables) == 27
    assert 'active_adc_temp3' in sequence.variables
    sequence.variables['active_adc_temp3'] = 5
    assert sequence.variables['active_adc_temp3'] == 5


def test_report_to_pudding():
    test_item = sequence[0]
    assert test_item.tid == 'INTEL_HOG_100_STAT_UNITSTAGE'
    assert test_item.report_to_pudding

    test_item = sequence[1]
    assert test_item.tid == 'INTEL_HOG_110_CHAN_CHANNELID'
    assert test_item.report_to_pudding


def test_iterate():
    i = 0
    for item in sequence:
        raw_item = raw_sequence[i]
        # print 'index=%d, self.index=%d, : %s/%s' % (i, sequence.get_index(item['TID']), item['TID'], item['GROUP'])
        assert item['GROUP'] == raw_item['GROUP']
        assert item['TID'] == raw_item['TID']
        assert item['FUNCTION'] == raw_item['FUNCTION']
        if len(raw_item['PARAM1']) > 0:
            assert item['PARAM1'] == raw_item['PARAM1']
        assert item['LOW'] == raw_item['LOW']
        assert item['HIGH'] == raw_item['HIGH']
        assert item['KEY'] == raw_item['KEY']
        assert item['VAL'] == raw_item['VAL']
        i += 1

    assert i == 17
    group = sequence['SYSCFG']
    assert group[0].tid == 'SYSCFG_OFF_100_DIAG'
    assert sequence.get_index(group[0].tid) == 7
    assert sequence['BOOT_DIAGS_100_DETE'].tid == 'BOOT_DIAGS_100_DETE'
    assert sequence['BOOT_DIAGS_100_DETE'].timeout == 3000
    assert sequence[14]['TID'] == 'INF_ACT_NTC4_PARS_ACTIVE_ADC_TEMP4'
    assert sequence.get_index('INTEL_HOG_100_STAT_UNITSTAGE') == 0
    assert sequence.get_index('INF_ACT_NTC4_PARS_ACTIVE_ADC_TEMP4') == 14
    assert sequence['INTEL_HOG_110_CHAN_CHANNELID'].timeout == 500

    test_item = sequence[0]
    assert test_item.tid == 'INTEL_HOG_100_STAT_UNITSTAGE'
    test_dict = test_item._to_dict()
    assert len(test_dict['params']) == 0

    test_item = sequence[6]
    assert test_item.tid == 'BOOT_DIAGS_100_DETE'
    test_dict = test_item._to_dict()
    assert len(test_dict['params']) == 1


def test_group():
    group = sequence['BOOT THE UNIT']
    assert len(group) == 5
    assert group[3]['GROUP'] == 'BOOT THE UNIT'
    assert group[4]['TID'] == 'BOOT_DIAGS_100_DETE'

    new_seq = TestSequence()
    new_seq.append(group)
    new_seq.append(sequence['INFANCY'])
    assert len(new_seq) == 7
    assert new_seq.get_index('INF_ACT_NTC4_PARS_ACTIVE_ADC_TEMP4') == 6


def test_filter():
    test = sequence['INF_ACT_NTC4_PARS_ACTIVE_ADC_TEMP4']
    assert len(test.filters) == 1
    assert 'unitStage' in test.filters
    assert test.filters['unitStage'] == '6000'
    # todo: test all key/values are correct
    # todo: test exception if number of key/value don't match


def test_args():
    test = sequence['INTEL_HOG_110_CHAN_CHANNELID']
    assert test.returned_val == 'channelId'
    assert test.report_to_pudding


def test_limits():
    test = sequence['SYSCFG_MLB_110_PARS_MLBSN_VERIFY']
    assert test.judge_pass('12345')
    assert not test.judge_pass('54321')
    test = sequence['CAL_BUCK0_B0_MEAS_SLEEP1_BUCK0_CAL_VALUE']
    assert test.judge_pass(0.6)
    assert not test.judge_pass(0.62)
    assert not test.judge_pass(0.58)


def test_shouldrun():
    # todo: test the TestItem.should_run function
    pass


def test_todict():
    test = sequence['CAL_BUCK0_140_CALC_SLEEP1_BUCK0_CAL_VALUE_MV']
    assert 'sleep1_buck0_cal_value' in test.kwargs
    test.kwargs['sleep1_buck0_cal_value'] = 3.2
    jdata = test._to_dict()
    assert len(jdata['params']) == 1
    assert jdata['params'][0] == "smokeyshell -e 'print(3.2*1000)'"


# todo: test valid sequence: if the item should not be sent to PDCA, then it should not have limit, so it can not fail
# todo: test valid sequence: make sure tid is unique in the whole sequence
