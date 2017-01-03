import os
import shutil
import json
import mock
import logging
import pytest
from pypudding import pypudding
from x527.loggers.puddinglogger import *
from x527.loggers.reporter import ReporterProtocol
from x527.loggers import events, levels
from x527.loggers.publisher import TestPublisher
from x527.sequencer.sequence import TestSequence
from x527.tinyrpc.protocols.jsonrpc import *
from x527.sequencer.sequencer import SequencerServer


def setup_module():
    logger = logging.getLogger('iTest.pudding')
    logger.setLevel(logging.DEBUG)
    logger.addHandler(logging.StreamHandler())
    config_dir = os.path.expanduser('~/testerconfig')
    config_file = os.path.join(config_dir, 'zmqports.json')

    path = os.path.split(__file__)[0]
    src_path = os.path.join(path, 'data_plan__1.0.csv')
    dst_path = '/tmp/data_plan__1.0.csv'
    shutil.copy(src_path, dst_path)

# todo: change these tests to use mock to record the actual calls
# todo: actual run a sequence to test.


@pytest.fixture
def mock_responses():
    mock_responses = []
    for i in range(19):
        response = JSONRPCSuccessResponse()
        response.result = '6000'
        response.unique_id = '0'
        response.version = '1.0'
        mock_responses.append(response)
    return mock_responses


@pytest.fixture()
def sequencer_server(request):
    ss = SequencerServer(0, TestPublisher('', list()))

    def fin():
        ss.rpc_server.shutdown()
        print('sequencer shutting down')
    request.addfinalizer(fin)

    path = os.path.split(__file__)[0]
    plan_path = os.path.join(path, 'data_plan__1.0.csv')
    ss.sequencer.load_sequence(plan_path)
    ss.sequencer.stop_on_fail = False
    return ss


@pytest.fixture(scope='module')
def station_info():
    fp = open('/vault/data_collection/test_station_config/gh_station_info.json')
    config = json.load(fp)
    return config['ghinfo']


# todo fixed these reports
@pytest.fixture
def reports1():
    reports = []
    protocol = ReporterProtocol()
    path = os.path.split(__file__)[0]
    plan_path = os.path.join(path, 'data_plan__1.0.csv')
    sequence = TestSequence(plan_path)

    reports.append(protocol.create_report(events.SEQUENCE_START, name='test_pudding', version='1.9'))
    # report the serial number
    reports.append(protocol.create_report(events.ATTRIBUTE_FOUND, name='MLBSN', value='YM123456789'))
    # report a parametric test
    test = sequence[2]
    assert test.report_to_pudding
    reports.append(protocol.create_report(events.ITEM_START,  to_pdca=test.report_to_pudding, **test.row))
    reports.append(protocol.create_report(events.ITEM_FINISH, tid=test.tid, result=True,
                                          value='4', to_pdca=test.report_to_pudding))
    # report a test that we found out needs to be reported upon seeing the result
    test = sequence[3]
    assert test.report_to_pudding
    reports.append(protocol.create_report(events.ITEM_START, to_pdca=False, **test.row))
    reports.append(protocol.create_report(events.ITEM_FINISH, tid=test.tid, result=True,
                                          value='--PASS--', to_pdca=True))
    # report an exception
    test = sequence[4]
    assert test.report_to_pudding
    reports.append(protocol.create_report(events.ITEM_START, to_pdca=False, **test.row))
    reports.append(protocol.create_report(events.ITEM_FINISH, tid=test.tid, result=-1, value='',
                                          to_pdca=True, error='some error message'))
    # report another attribute
    reports.append(protocol.create_report(events.ATTRIBUTE_FOUND, name='an_attr', value='0x30'))
    reports.append(protocol.create_report(events.SEQUENCE_END, result=False, logs=plan_path + ', '))
    return reports


@pytest.fixture
def reports2():
    reports = []
    protocol = ReporterProtocol()
    path = os.path.split(__file__)[0]
    plan_path = os.path.join(path, 'data_plan__1.0.csv')
    sequence = TestSequence(plan_path)

    reports.append(protocol.create_report(events.SEQUENCE_START, name='test_pudding2', version='2.0'))
    # report the serial number
    reports.append(protocol.create_report(events.ATTRIBUTE_FOUND, name='MLBSN', value='YM123456787'))
    # report a parametric test
    test = sequence[2]
    assert test.report_to_pudding
    reports.append(protocol.create_report(events.ITEM_START,  to_pdca=test.report_to_pudding, **test.row))
    reports.append(protocol.create_report(events.ITEM_FINISH, tid=test.tid, result=True,
                                          value='7', to_pdca=test.report_to_pudding))
    # report a test that we found out needs to be reported upon seeing the result
    test = sequence[3]
    assert test.report_to_pudding
    reports.append(protocol.create_report(events.ITEM_START, to_pdca=False, **test.row))
    reports.append(protocol.create_report(events.ITEM_FINISH, tid=test.tid, result=True,
                                          value='--FAIL--', to_pdca=True))
    # report an exception
    test = sequence[4]
    assert test.report_to_pudding
    reports.append(protocol.create_report(events.ITEM_START, to_pdca=False, **test.row))
    reports.append(protocol.create_report(events.ITEM_FINISH, tid=test.tid, result=-1,
                                          value='', to_pdca=True, error='some error message'))
    # report another attribute
    reports.append(protocol.create_report(events.ATTRIBUTE_FOUND, name='an_attr', value='0x40'))
    reports.append(protocol.create_report(events.SEQUENCE_END, result=False, logs=plan_path + ', '))
    return reports


def test_pdata():
    assert ispdata('1')
    assert not ispdata('0xA')
    assert ispdata('-1.23')
    assert ispdata('-0.05')
    assert ispdata('0.05')
    assert ispdata('9.0')
    assert not ispdata('FCT')
    assert ispdata('.5')
    assert ispdata('5.')


def test_check_length():
    str1 = '1'*50
    str2 = '1'*48
    assert pdca_check('12_0.0.1', VERSION) == '12_0.0.1'
    assert pdca_check(str1, VERSION) == str2


def get_commit_obj(uut):
    reply = pypudding.IP_getJsonResultsObj(uut.uut)
    msg = pypudding.IP_reply_getError(reply)
    commit_obj = json.loads(msg)

    blobs = commit_obj['blobs']

    s_attr = commit_obj['test_station_attributes']

    u_attr = commit_obj['uut_attributes']
    t_attr = commit_obj['test_attributes']
    sn = t_attr['unit_serial_number']
    results = commit_obj['results']

    return sn, s_attr, u_attr, results, blobs

# todo: check InstantPudding Exception
# todo: check wrong data type: non numerical value


def test_commit_run(reports1, reports2):

    publisher = TestPublisher('TP_test_pudding', list())

    p_logger = PuddingLogger(publisher=publisher)
    protocol = ReporterProtocol()

    report_len = len(reports1)
    p_logger.log_sequence_start('tt', 0, reports1[0])
    p_logger.log_attribute_found('tt', 0, reports1[1])
    p_logger.log_sequence_start('tt', 1, reports2[0])
    p_logger.log_attribute_found('tt', 1, reports2[1])
    for i in range(2, report_len-2, 2):
        p_logger.log_item_start('tt', 0, reports1[i])
        p_logger.log_item_finish('tt', 0, reports1[i+1])
        p_logger.log_item_start('tt', 1, reports2[i])
        p_logger.log_item_finish('tt', 1, reports2[i+1])

    p_logger.log_attribute_found('tt', 0, reports1[-2])
    p_logger.log_attribute_found('tt', 1, reports2[-2])
    p_logger.log_sequence_end('tt', 0, reports1[-1])
    p_logger.log_sequence_end('tt', 1, reports2[-1])

    sn, s_attr, u_attr, results, blobs = get_commit_obj(p_logger.duts[0].uut)
    assert sn == 'YM123456789'
    assert s_attr['software_name'] == 'test_pudding'
    assert s_attr['software_version'] == '1.9__test_pudding'
    assert u_attr['an_attr'] == '0x30'
    assert len(results) == 4
    assert results[0]['value'] == '4'
    assert results[0]['result'] == 'pass'
    assert 'parametric_key' in results[0]
    assert results[0]['test'] == reports1[2].data['group']
    assert results[0]['sub_test'] == reports1[2].data['tid']

    assert results[1]['value'] == ''
    assert results[1]['result'] == 'pass'
    assert 'test_type' in results[1]
    assert results[1]['test'] == reports1[4].data['group']
    assert results[1]['sub_test'] == reports1[4].data['tid']

    assert results[2]['value'] == ''
    assert results[2]['result'] == 'fail'
    assert results[2]['message'] == 'some error message BOOT THE UNIT::BOOT_DIAGS_100_DETE; limits:NA; unit:NA; priority 1'
    assert 'test_type' in results[2]
    assert results[2]['test'] == reports1[6].data['group']
    assert results[2]['sub_test'] == reports1[6].data['tid']
    assert len(blobs) == 1

    sn, s_attr, u_attr, results, blobs = get_commit_obj(p_logger.duts[1].uut)
    assert sn == 'YM123456787'
    assert s_attr['software_name'] == 'test_pudding2'
    assert s_attr['software_version'] == '2.0__test_pudding2'
    assert u_attr['an_attr'] == '0x40'
    assert len(results) == 4
    assert len(blobs) == 1

    for msg in publisher.msg_list:
        if msg[1] == levels.REPORTER:
            report = protocol.parse_report(msg[3])
            # assert 'error_code' not in report.data, report.data
        assert not msg[1] == levels.CRITICAL, msg


def test_cancel_run():
    p_logger = PuddingLogger()
    protocol = ReporterProtocol()
    path = os.path.split(__file__)[0]
    plan_path = os.path.join(path, 'data_plan__1.0.csv')
    sequence = TestSequence(plan_path)
    sequence[4].report_to_pudding = True

    site = 0
    report = protocol.create_report(events.SEQUENCE_START, name='test_pudding', version='1.9')
    p_logger.log_sequence_start('tt', site, report)
    for test in sequence:
        report = protocol.create_report(events.ITEM_START, to_pdca=test.report_to_pudding, **test.row)
        p_logger.log_item_start('ts', site, report)
        if test.p_attribute is not None:
            report = protocol.create_report(events.ATTRIBUTE_FOUND, name=test.p_attribute, value='YM12345678')
            p_logger.log_attribute_found('ts', site, report)
        report = protocol.create_report(events.ITEM_FINISH, tid=test.tid, result=True, value='4',
                                        to_pdca=test.report_to_pudding)
        p_logger.log_item_finish('ts', site, report)
    report = protocol.create_report(events.SEQUENCE_END, result=-1, logs=plan_path,
                                    error='test cancel error message')
    p_logger.log_sequence_end('ts', site, report)
