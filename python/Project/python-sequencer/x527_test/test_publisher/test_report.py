import pytest
import os
import mock
import time
from x527.loggers import levels, events, TestPublisher
from x527.loggers.reporter import *
from x527.loggers.reportlogger import *
from x527.sequencer import sequencer
from x527.sequencer.sequencer import SequencerServer
from x527.engine.testengine import DummyTestEngine, ReplayDispatcher
from x527.tinyrpc.protocols.terpc import *
from x527.tinyrpc.protocols.sequencerrpc import *


@pytest.fixture(scope='module')
def sequencer_server(request):
    ss = SequencerServer(1, 1)

    def fin():
        ss.rpc_server.shutdown()
        print('sequencer shutting down')
    request.addfinalizer(fin)

    path = os.path.split(__file__)[0]
    plan_path = os.path.join(path, 'short_plan__1.0.csv')
    ss.sequencer.load_sequence(plan_path)
    ss.sequencer.stop_on_fail = False
    return ss


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


def test_reporter():
    publisher = TestPublisher('test', list())
    reporter = Reporter(publisher)
    reporter.report(events.SEQUENCE_START, name='my_name', version='my_version')
    reporter.report(events.ITEM_START, to_pdca=False, group='G1', tid='test1', low=0, high=1, unit='v')
    reporter.report(events.ITEM_FINISH, tid='test2', value=2123, result=True, to_pdca=True)
    with pytest.raises(ReportDataError) as excinfo:
        reporter.report(events.ITEM_FINISH, value='123', result=False)

    reporter.report(events.ATTRIBUTE_FOUND, name="myattr", value='somevalue')
    reporter.report(events.SEQUENCE_END, result=1)
    reporter.report(events.SEQUENCE_END, result=0, logs='some_log_files')
    reporter.report(events.SEQUENCE_END, result=-1, error='my error message')
    with pytest.raises(ReportDataError) as excinfo:
        reporter.report(events.SEQUENCE_END, result=-1)

    protocol = ReporterProtocol()
    reports = []
    for msg in publisher.msg_list:
        assert msg[1] == levels.REPORTER
        reports.append(protocol.parse_report(msg[3]))

    assert len(reports) == 7
    assert reports[0].event == events.SEQUENCE_START
    assert reports[0].data['name'] == 'my_name'
    assert reports[0].data['version'] == 'my_version'
    assert reports[1].data['to_pdca'] == False
    assert reports[2].event == events.ITEM_FINISH
    assert reports[2].data['tid'] == 'test2'
    assert reports[2].data['value'] == 2123
    assert reports[2].data['result'] == True
    assert reports[2].data['to_pdca'] == True
    assert reports[3].event == events.ATTRIBUTE_FOUND
    assert reports[4].data['logs'] == ''
    assert reports[5].data['logs'] == 'some_log_files'
    assert reports[6].event == events.SEQUENCE_END
    assert reports[6].data['error'] == 'my error message'
    assert reports[6].data['result'] == -1
    assert reports[6].data['logs'] == ''


def run_sequence(ss, tp):
    protocol = SequencerRPCProtocol()
    tp.msg_list = []
    e_traveler = dict(attributes=dict(MLBSN='YM123456789'))
    req = protocol.create_request('run', e_traveler)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    while True:
        if ss.sequencer.status() == 'READY':
            break
        time.sleep(1)
    # ss.sequencer.wait(8)


def test_complete_run(sequencer_server, mock_responses):
    # todo figure out why test test takes so much time
    ss = sequencer_server

    m = mock.MagicMock()
    m.side_effect = mock_responses
    ss.sequencer.test_engine.send_request = m
    tp = TestPublisher('TP', list())
    ss.sequencer.publisher = tp
    ss.sequencer.stop_on_fail = False
    ss.sequencer.reporter = Reporter(tp)

    assert len(ss.sequencer.sequence) == 17
    assert len(ss.sequencer.sequence.groups) == 5

    run_sequence(ss, tp)
    r_data = []
    r_protocol = ReporterProtocol()
    for message in tp.msg_list:
        ts, level, id_str, msg = message
        if level == levels.REPORTER:
            r_data.append(r_protocol.parse_report(msg))

    assert r_data[0].event == events.SEQUENCE_START
    assert r_data[0].data['name'] == 'short_plan'
    assert r_data[0].data['version'] == '1.0'
    assert r_data[1].event == events.ATTRIBUTE_FOUND
    assert r_data[1].data['name'] == 'MLBSN'
    assert r_data[1].data['value'] == 'YM123456789'
    # todo test other events data in between
    assert r_data[-1].event == events.SEQUENCE_END
    assert r_data[-1].data['result'] == False
    assert r_data[-1].data['logs'] == '6000'
    assert len(r_data) == 37


def test_fail_run(sequencer_server, mock_responses):
    ss = sequencer_server

    m = mock.MagicMock()
    m.side_effect = mock_responses
    ss.sequencer.test_engine.send_request = m
    tp = TestPublisher('TP', list())
    ss.sequencer.publisher = tp
    ss.sequencer.stop_on_fail = False
    ss.sequencer.reporter = Reporter(tp)
    ss.sequencer.stop_on_fail = True
    ss.sequencer.test_engine.send_request = m
    run_sequence(ss, tp)
    r_data = []
    r_protocol = ReporterProtocol()
    for message in tp.msg_list:
        ts, level, id_str, msg = message
        if level == levels.REPORTER:
            r_data.append(r_protocol.parse_report(msg))

    assert r_data[0].event == events.SEQUENCE_START
    assert r_data[0].data['name'] == 'short_plan'
    assert r_data[0].data['version'] == '1.0'
    assert r_data[1].event == events.ATTRIBUTE_FOUND
    assert r_data[1].data['name'] == 'MLBSN'
    assert r_data[1].data['value'] == 'YM123456789'
    assert r_data[-1].event == events.SEQUENCE_END
    assert r_data[-1].data['logs'] == '6000'
    assert r_data[-1].data['result'] == False
    assert r_data[-2].event == events.ITEM_FINISH
    assert len(r_data) == 9


def test_unknown_exception_run(sequencer_server, mock_responses):
    ss = sequencer_server

    m = mock.MagicMock()
    m.side_effect = mock_responses[:3]  # mock_responses will throw an exception because it runs out of exception
    ss.sequencer.test_engine.send_request = m
    tp = TestPublisher('TP', list())
    ss.sequencer.publisher = tp
    ss.sequencer.stop_on_fail = False
    ss.sequencer.reporter = Reporter(tp)
    ss.sequencer.stop_on_fail = True
    ss.sequencer.test_engine.send_request = m
    run_sequence(ss, tp)
    r_data = []
    r_protocol = ReporterProtocol()
    for message in tp.msg_list:
        ts, level, id_str, msg = message
        if level == levels.REPORTER:
            r_data.append(r_protocol.parse_report(msg))

    assert r_data[0].event == events.SEQUENCE_START
    assert r_data[0].data['name'] == 'short_plan'
    assert r_data[0].data['version'] == '1.0'
    assert r_data[-1].event == events.SEQUENCE_END
    assert r_data[-1].data['result'] == False
    assert r_data[-1].data['logs'] == '6000'
    # todo: maybe add the error message back for failed runs, but a better place should be in the ITEM_FINISH event
    assert r_data[-2].event == events.ITEM_FINISH
    assert len(r_data) == 9


def test_engine_exception_run(sequencer_server, mock_responses):
    ss = sequencer_server

    m = mock.MagicMock()
    mock_responses[2].error = 'planted error'
    mock_responses[2]._jsonrpc_error_code = -10000
    m.side_effect = mock_responses
    ss.sequencer.test_engine.send_request = m
    tp = TestPublisher('TP', list())
    ss.sequencer.publisher = tp
    ss.sequencer.stop_on_fail = False
    ss.sequencer.reporter = Reporter(tp)
    ss.sequencer.stop_on_fail = True
    ss.sequencer.test_engine.send_request = m
    run_sequence(ss, tp)
    r_data = []
    r_protocol = ReporterProtocol()
    for message in tp.msg_list:
        ts, level, id_str, msg = message
        if level == levels.REPORTER:
            r_data.append(r_protocol.parse_report(msg))

    assert r_data[0].event == events.SEQUENCE_START
    assert r_data[0].data['name'] == 'short_plan'
    assert r_data[0].data['version'] == '1.0'

    # we do receive the report for tests that are not supposed to go to PDCA
    assert r_data[4].data['tid'] == 'INTEL_HOG_110_CHAN_CHANNELID'
    assert r_data[5].data['tid'] == 'INTEL_HOG_110_CHAN_CHANNELID'
    assert r_data[5].data['result'] == -1
    assert r_data[5].data['to_pdca']
    assert r_data[5].data['value'] == ''
    assert 'error' in r_data[5].data
    assert r_data[5].event == events.ITEM_FINISH

    # todo: maybe add the error message back for failed runs, but a better place should be in the ITEM_FINISH event
    assert r_data[-1].data['result'] == False
    assert len(r_data) == 7
# todo: add test puddingreport. especially for test function that replies '--PASS--' and '--FAIL--'
# todo: run sequence multiple times to make sure the returned_value and p_attribute data are not staled
