import os
import pytest
import json
import pypudding
from pypudding import IPUUT, IPTestSpec, IPTestResult, InstantPuddingError


@pytest.fixture(scope='module')
def station_info():
    fp = open('/vault/data_collection/test_station_config/gh_station_info.json')
    config = json.load(fp)
    return config['ghinfo']


@pytest.fixture
def uut():
    uut = IPUUT()
    uut.start()
    uut.add_attribute(pypudding.IP_ATTRIBUTE_STATIONSOFTWARENAME, "PyPudding")
    uut.add_attribute(pypudding.IP_ATTRIBUTE_STATIONSOFTWAREVERSION,  '1.1')
    return uut


def test_read_station_info(station_info):
    gh_station = pypudding.IPGHStation()
    station_type = gh_station[pypudding.IP_STATION_TYPE]
    assert station_type == station_info['STATION_TYPE']
    assert gh_station[pypudding.IP_STATION_NUMBER] == station_info['STATION_NUMBER']


def test_commit_run():
    uut = IPUUT(u'YM123456789')
    uut.start()
    uut.add_attribute(pypudding.IP_ATTRIBUTE_STATIONSOFTWARENAME, u"PyPudding")
    uut.add_attribute(pypudding.IP_ATTRIBUTE_STATIONSOFTWAREVERSION,  u'1.1')

    # pass fail test passing:
    spec = IPTestSpec('PassingTest')
    result = IPTestResult(pypudding.IP_PASS)
    uut.add_result(spec, result)

    # parametric test pass
    spec = IPTestSpec(u'Parametric data test', subtest_name='pass_test',
                      limits={'low_limit': 0, 'high_limit': 9}, unit=u'ly')
    result = IPTestResult(pypudding.IP_PASS, 4)
    uut.add_result(spec, result)

    # parametric test fail
    spec = IPTestSpec(u'Parametric data test', subtest_name=u'fail_test',
                      limits={'low_limit': 0, 'high_limit': 8}, unit='ly')
    result = IPTestResult(pypudding.IP_FAIL, 11, u'value out of range')
    uut.add_result(spec, result)
    uut.add_attribute(u'VBOOST_LEFT', u'0x01')
    path = os.path.split(__file__)[0]
    plan_path = os.path.join(path, 'pypudding.py')
    uut.add_blob_file(u'myself', plan_path)

    uut.done()
    uut.commit(pypudding.IP_PASS)
    reply = pypudding.IP_getJsonResultsObj(uut.uut)
    msg = pypudding.IP_reply_getError(reply)
    commit_obj = json.loads(msg)

    blobs = commit_obj['blobs']
    assert len(blobs) == 1
    assert blobs[0]['display_name'] == 'myself'

    s_attr = commit_obj['test_station_attributes']
    assert s_attr['software_name'] == 'PyPudding'
    assert s_attr['software_version'] == '1.1'

    u_attr = commit_obj['uut_attributes']
    assert u_attr.keys() == ['VBOOST_LEFT']
    assert u_attr['VBOOST_LEFT'] == '0x01'
    t_attr = commit_obj['test_attributes']
    assert t_attr['unit_serial_number'] == 'YM123456789'
    results = commit_obj['results']
    real_results = [r for r in results if r['test'] != 'FATAL ERROR']
    assert len(real_results) == 3

    r0 = real_results[0]
    assert r0['test'] == 'PassingTest'
    assert r0['test_type'] == 'PF'
    assert r0['result'] == 'pass'
    
    r1 = real_results[1]
    assert r1['test'] == 'Parametric data test'
    assert r1['sub_test'] == 'pass_test'
    assert r1['parametric_key'] == 'Parametric data test pass_test'
    assert r1['result'] == 'pass'
    assert r1['lower_limit'] == '0'
    assert r1['upper_limit'] == '9'
    assert r1['value'] == '4'
    
    r2 = real_results[2]
    assert r2['test'] == 'Parametric data test'
    assert r2['sub_test'] == 'fail_test'
    assert r2['parametric_key'] == 'Parametric data test fail_test'
    assert r2['result'] == 'fail'
    assert r2['lower_limit'] == '0'
    assert r2['upper_limit'] == '8'
    assert r2['value'] == '11'

    pypudding.IP_reply_destroy(reply)
    del uut


def test_name_too_long(uut):
    # InstantPudding.log shows an error and the result is NOT in the committed JSON file
    uut.set_sn('YM12345678A')

    long_name = 'n' * (pypudding.PDCA_TEST_NAME_MAX_LEN + 1)
    with pytest.raises(InstantPuddingError) as exc:
        spec = IPTestSpec(long_name, subtest_name='pass_test', limits={'low_limit': 0, 'high_limit': 9}, unit='ly')
    assert exc.value.message.startswith("test name is too long")

    long_name = 'n' * (pypudding.PDCA_SUBTEST_NAME_MANX_LEN + 1)
    with pytest.raises(InstantPuddingError) as exc:
        spec = IPTestSpec('passing_test', subtest_name=long_name, limits={'low_limit': 0, 'high_limit': 9}, unit='ly')
    assert exc.value.message.startswith("subtest name is too long")

    long_name = 'n' * (pypudding.PDCA_SUBSUBTEST_NAME_MANX_LEN + 1)
    with pytest.raises(InstantPuddingError) as exc:
        spec = IPTestSpec('passing_test', subtest_name='passing sub test',
                          subsubtest_name=long_name, limits={'low_limit': 0, 'high_limit': 9}, unit='ly')
    assert exc.value.message.startswith("subsubtest name is too long")

    long_name = 'n' * (pypudding.PDCA_UNIT_MAX_LEN + 1)
    with pytest.raises(InstantPuddingError) as exc:
        spec = IPTestSpec('passing_test', subtest_name='passing sub test',
                          limits={'low_limit':0, 'high_limit': 9}, unit=long_name)
    assert exc.value.message.startswith("unit is too long")

    long_msg = 'n' * (pypudding.PDCA_FAIL_MSG_MAX_LEN + 1)
    with pytest.raises(InstantPuddingError) as exc:
        r = IPTestResult(pypudding.IP_FAIL, message=long_msg)
    assert exc.value.message.startswith("fail message is too long")

    long_name = 'n' * (pypudding.PDCA_ATTRIBUTE_NAME_MAX_LEN + 1)
    with pytest.raises(InstantPuddingError) as exc:
        uut.add_attribute(long_name, 'some value')
    assert exc.value.message.startswith("attribute name is too long")

    long_value = 'n' * (pypudding.PDCA_ATTRIBUTE_VALUE_MAX_LEN + 1)
    with pytest.raises(InstantPuddingError) as exc:
        uut.add_attribute('some attribute', long_value)
    assert exc.value.message.startswith("attribute value is too long")

    uut.cancel()
    del uut


def test_no_fail_message(uut):
    # instant pudding log shows an error but the result is in the committed JSON file
    uut.set_sn('YM12345678C')

    long_name = 'no_fail_msg'
    spec = IPTestSpec(long_name, subtest_name='pass_test', limits={'low_limit': 0, 'high_limit': 9}, unit='ly')
    result = IPTestResult(pypudding.IP_FAIL, -3)
    assert result._message == 'FAIL' # a default fail message is added
    result.message = 'my message'
    assert result._message == 'my message'
    result = IPTestResult(pypudding.IP_FAIL, -3, 'my message 2')
    assert result._message == 'my message 2'
    result.set_result(pypudding.IP_FAIL)
    assert result._message == 'my message 2'  # my message is not overwritten

    uut.cancel()
    del uut


def test_numeric_format(uut):
    # instant pudding log shows an error but the result is in the committed JSON file
    uut.set_sn('YM12345678C')
    # valid numerical values
    spec = IPTestSpec('test name', 'subtest_name', limits={'low_limit': '.4', 'high_limit': '-12e-3'})
    with pytest.raises(InstantPuddingError) as exc:
        spec = IPTestSpec('test name', 'subtest_name', limits={'low_limit': '0x45', 'high_limit':'-12e-3'})
    assert exc.value.message.endswith("is not a numerical value accepted by PDCA")

    result = IPTestResult(pypudding.IP_PASS, '+.3e2')
    with pytest.raises(InstantPuddingError) as exc:
        result = IPTestResult(pypudding.IP_PASS, '2fe3')
    assert exc.value.message.endswith("is not a numerical value accepted by PDCA")
    uut.cancel()
    del uut


def test_no_sn(uut):
    uut.start()
    uut.add_attribute(pypudding.IP_ATTRIBUTE_STATIONSOFTWARENAME, u"PyPudding")
    uut.add_attribute(pypudding.IP_ATTRIBUTE_STATIONSOFTWAREVERSION,  u'1.1')

    spec = IPTestSpec(u'Parametric data test', subtest_name='pass_test', limits={'low_limit':0, 'high_limit':9}, unit=u'ly')
    result = IPTestResult(pypudding.IP_PASS, 4)
    uut.add_result(spec, result)

    uut.done()
    with pytest.raises(InstantPuddingError) as exc:
        uut.commit(pypudding.IP_PASS)
    assert exc.value.message.endswith("trying to commit without an SN")

    uut.cancel()
    del uut