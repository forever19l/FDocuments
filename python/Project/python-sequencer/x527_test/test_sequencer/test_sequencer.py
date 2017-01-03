import os
import mock
import time
import datetime
import pytest
import zmq
from x527 import zmqports
from x527.tinyrpc.server import *
from x527.tinyrpc.protocols import sequencerrpc
from x527.tinyrpc.protocols.sequencerrpc import *
from x527.tinyrpc.proxy.teproxy import TEProxy
from x527.tinyrpc.protocols.terpc import TERPCProtocol
from x527.sequencer.sequencer import SequencerServer
from x527.sequencer.pluginserver import PluginServer
from x527.loggers import events, TestPublisher, ZmqPublisher

se_msg_list = []
ctx = zmq.Context()
publisher = ZmqPublisher(ctx, "tcp://*:" + str(zmqports.SEQUENCER_PUB), "TestSequencer")


@pytest.fixture
def delay_mock():
    m = mock.MagicMock()

    def time_delay(*args):
        response = JSONRPCSuccessResponse()
        response.result = '--PASS--'
        response.unique_id = '0'
        response.version = '1.0'
        time.sleep(1)   # delay 1ms is hardcoded here?
        return response

    m.side_effect = time_delay
    return m


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
    #return [response for i in range(19)]


@pytest.fixture
def sequencer_server(request):
    ss = SequencerServer(0, 1, publisher, False)

    def fin():
        ss.rpc_server.shutdown()
        print('sequencer shutting down')
    request.addfinalizer(fin)

    path = os.path.split(__file__)[0]
    plan_path = os.path.join(path, 'short_plan__1.0.csv')
    ss.sequencer.load_sequence(plan_path)
    return ss


@pytest.fixture
def ss2(request):
    ss = SequencerServer(1, 1, publisher, False)

    def fin():
        ss.rpc_server.shutdown()
        print('sequencer shutting down')
    request.addfinalizer(fin)

    path = os.path.split(__file__)[0]
    plan_path = os.path.join(path, 'data_plan__1.0.csv')
    ss.sequencer.load_sequence(plan_path)
    return ss


@pytest.fixture
def stage_mock():
    m = mock.MagicMock()
    response = JSONRPCSuccessResponse()
    response.result = '6000'
    response.unique_id = '0'
    response.version = '1.0'
    m.return_value = response
    return m


@pytest.fixture
def wrong_stage_mock():
    m = mock.MagicMock()
    response = JSONRPCSuccessResponse()
    response.result = '7000'
    response.unique_id = '0'
    response.version = '1.0'
    m.return_value = response
    return m


@pytest.fixture
def protocol():
    return SequencerRPCProtocol()


def test_handle_errors(sequencer_server, protocol):
    ss = sequencer_server
    req = protocol.create_request('invalid_param', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert response.version == sequencerrpc.SEQUENCER_RPC_VERSION
    assert not hasattr(response, 'result')
    assert response.error == 'Method not found: invalid_param'
    assert response._jsonrpc_error_code == -32601

    req = protocol.create_request('jump', '100')
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert response.version == sequencerrpc.SEQUENCER_RPC_VERSION
    assert not hasattr(response, 'result')
    assert response.error == 'the requested line 100 is beyond the end of the sequence'
    assert response._jsonrpc_error_code == -32602

    req = protocol.create_request('jump', '0')
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert response.version == sequencerrpc.SEQUENCER_RPC_VERSION
    assert not hasattr(response, 'result')
    assert response.error == 'the requested line 0 is beyond the end of the sequence'
    assert response._jsonrpc_error_code == -32602
    # todo: test other error types


def test_run(sequencer_server, protocol, mock_responses):
    ss = sequencer_server

    te_protocol = TERPCProtocol()
    m = mock.MagicMock()
    m.side_effect = mock_responses
    ss.sequencer.test_engine.send_request = m

    start_req = te_protocol.create_request('start_test', None, '', 5000)
    end_req = te_protocol.create_request('end_test', [0], '', 5000)

    assert len(ss.sequencer.sequence) == 17
    assert len(ss.sequencer.sequence.groups) == 5

    path = os.path.split(__file__)[0]
    plan_path = os.path.join(path, 'short_plan__1.0.csv')
    req = protocol.create_request('load', plan_path)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error')
    req = protocol.create_request('run', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error')
    while ss.sequencer.running:
        time.sleep(0.2)  # wait for sequencer to finish

    sequence = ss.sequencer.sequence
    assert sequence.variables['unitStage'] == '6000'
    expected = [mock.call(start_req, 5000),]
    for item in sequence:
        test_dict = item._to_dict()
        req = te_protocol.create_request(test_dict['function'], test_dict['params'], item.unit, item.timeout)
        expected.append(mock.call(req, item.timeout))
    expected.append(mock.call(end_req, 5000))
    assert len(m.mock_calls) == len(expected)
    assert m.mock_calls == expected


def test_exception_run(ss2, protocol, mock_responses):      # test exception from fctplugin
    te_protocol = TERPCProtocol()
    m = mock.MagicMock()
    m.side_effect = mock_responses
    ss2.sequencer.test_engine.send_request = m
    m_report = mock.MagicMock()
    ss2.sequencer.reporter.report = m_report
    req = protocol.create_request('run', None)
    response = ss2.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    while ss2.sequencer.running:
        time.sleep(0.2)
    assert m_report.mock_calls[-1] == \
           mock.call(events.SEQUENCE_END,
                     error='error running test TEST_PLUGIN_EXC; '
                           'error message is "Test Engine error: -32620:exception from plugin"',
                     result=False, logs='6000')


def test_run_mask(sequencer_server, protocol, mock_responses):
    ss = sequencer_server

    te_protocol = TERPCProtocol()
    m = mock.MagicMock()
    mock_responses[1].result = '7000'
    m.side_effect = mock_responses
    ss.sequencer.test_engine.send_request = m

    start_req = te_protocol.create_request('start_test', None, '', 5000)
    end_req = te_protocol.create_request('end_test', [0], '', 5000)

    path = os.path.split(__file__)[0]
    plan_path = os.path.join(path, 'short_plan__1.0.csv')
    req = protocol.create_request('load', plan_path)
    response = ss.rpc_server.handle_message(0, req.serialize())

    assert len(ss.sequencer.sequence) == 17
    assert len(ss.sequencer.sequence.groups) == 5
    sequence = ss.sequencer.sequence

    # now we confirm we do skip the wrong stage

    global se_msg_list
    se_msg_list = []

    req = protocol.create_request('run', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    ss.sequencer.wait(2)
    expected = [mock.call(start_req, 5000)]
    for item in sequence:
        if item.group != 'INFANCY':
            test_dict = item._to_dict()
            req = te_protocol.create_request(test_dict['function'], test_dict['params'], item.unit, item.timeout)
            expected.append(mock.call(req, item.timeout))
    expected.append(mock.call(end_req, 5000))
    assert sequence.variables['unitStage'] == '7000'
    assert len(m.mock_calls) == len(expected)
    assert m.mock_calls == expected


def test_jump_list(sequencer_server, protocol):
    ss = sequencer_server
    assert ss.sequencer.pc == 0
    req = protocol.create_request('jump', '5')
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    assert ss.sequencer.pc == 4

    req = protocol.create_request('list', '10')
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    assert len(response.result) == 11
    assert response.result[0] == [4, 1, 11]
    pc, start, stop = response.result[0]
    listings = response.result[1:]
    for index, line in enumerate(listings):
        test = eval(line[1])
        assert int(line[0]) == start+index+1
        assert test['TID'] == ss.sequencer.sequence[int(line[0])-1].tid

    req = protocol.create_request('jump', 'INFANCY')
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    assert ss.sequencer.pc == 13

    req = protocol.create_request('jump', 'BOOT_DIAGS_100_DETE')
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    assert ss.sequencer.pc == 6


def test_step(sequencer_server, protocol):
    ss = sequencer_server
    ss.sequencer.jump_to_line(14)

    m = mock.MagicMock()
    response = JSONRPCSuccessResponse()
    response.result = '6000'
    response.unique_id = '0'
    response.version = '1.0'
    m.return_value = response
    ss.sequencer.test_engine.send_test = m
    req = protocol.create_request('step', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    assert response.result[0] == 16
    # there should be no problem serialize this
    response.serialize()
    req = protocol.create_request('step', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    req = protocol.create_request('step', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    assert response.result is None


def test_show(sequencer_server, protocol):
    ss = sequencer_server
    ss.sequencer.sequence.variables['unitStage'] = 'PRESMT'
    req = protocol.create_request('show', 'unitStage')
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    assert response.result == 'PRESMT'
    s_response = response.serialize()


def test_next(sequencer_server, protocol):
    ss = sequencer_server
    ss.sequencer.pc = 8
    req = protocol.create_request('s_next', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    assert response.result == 9
    response.serialize()


def test_stop_on_fail(sequencer_server, protocol, mock_responses):
    ss = sequencer_server
    ss.sequencer.stop_on_fail = True
    assert len(ss.sequencer.sequence) == 17
    assert len(ss.sequencer.sequence.groups) == 5

    req = protocol.create_request('run', None)

    te_protocol = TERPCProtocol()
    m = mock.MagicMock()
    m.side_effect = mock_responses
    ss.sequencer.test_engine.send_request = m

    start_req = te_protocol.create_request('start_test', None, '', 5000)
    end_req = te_protocol.create_request('end_test', [0], '', 5000)

    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    expected = [mock.call(start_req, 5000), ]
    for item in ss.sequencer.sequence:
        if item.tid == 'BOOT_BATT_110_SUPP':
            break
        test_dict = item._to_dict()
        req = te_protocol.create_request(test_dict['function'], test_dict['params'], item.unit, item.timeout)
        expected.append(mock.call(req, item.timeout))
    expected.append(mock.call(end_req, 5000))
    while ss.sequencer.running:
        time.sleep(0.2)  # wait for sequencer to finish
    assert len(m.mock_calls) == len(expected)
    assert m.mock_calls == expected
    ss.stop_on_fail = False


def test_continue_on_detect(sequencer_server, protocol, mock_responses):
    ss = sequencer_server
    ss.sequencer.stop_on_fail = True
    assert len(ss.sequencer.sequence) == 17
    assert len(ss.sequencer.sequence.groups) == 5

    req = protocol.create_request('run', None)

    te_protocol = TERPCProtocol()
    m = mock.MagicMock()
    mock_responses[3].result = 1.5
    mock_responses[7].result = '--FAIL--'
    m.side_effect = mock_responses
    ss.sequencer.test_engine.send_request = m

    start_req = te_protocol.create_request('start_test', None, '', 5000)
    end_req = te_protocol.create_request('end_test', [0], '', 5000)

    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    expected = [mock.call(start_req, 5000), ]
    for item in ss.sequencer.sequence:
        print item.tid
        if item.tid == 'SYSCFG_OFF_110_DIAG':
            break
        test_dict = item._to_dict()
        req = te_protocol.create_request(test_dict['function'], test_dict['params'], item.unit, item.timeout)
        expected.append(mock.call(req, item.timeout))
    expected.append(mock.call(end_req, 5000))
    while ss.sequencer.running:
        time.sleep(0.2)  # wait for sequencer to finish
    assert len(m.mock_calls) == len(expected)
    assert m.mock_calls == expected
    ss.stop_on_fail = False


def test_special_result(sequencer_server, protocol):
    ss = sequencer_server
    sequencer = ss.sequencer
    sequence = sequencer.sequence
    test_item = sequence['BOOT_DIAGS_100_DETE']

    m = mock.MagicMock()
    response = JSONRPCSuccessResponse()
    response.result = '--PASS--'
    response.unique_id = '0'
    response.version = '1.0'
    m.return_value = response
    ss.sequencer.test_engine.send_test = m
    result = sequencer.run_test(test_item)
    assert result

    response.result = '--FAIL--'
    result = sequencer.run_test(test_item)
    assert not result

    response.result = '--FaiL-- '  # not the right case and an extra space
    result = sequencer.run_test(test_item)
    assert not result

    response.result = '--pAss--'  # not the right case
    result = sequencer.run_test(test_item)
    assert result


def test_status(sequencer_server, protocol, delay_mock):
    s2 = SequencerServer(2, 1)
    req = protocol.create_request('status', None)
    response = s2.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error')
    assert response.result == 'NONLOADED'

    ss = sequencer_server
    ss.sequencer.sequence.stop_on_fail = False
    req = protocol.create_request('status', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error')
    assert response.result == 'READY'

    ss.sequencer.test_engine.send_request = delay_mock
    t1 = datetime.datetime.now()
    # run
    req = protocol.create_request('run', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error')
    # check that its' running
    req = protocol.create_request('status', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error')
    assert response.result == 'RUNNING'
    t2 = datetime.datetime.now()
    tt = (t2-t1).total_seconds()
    # abort
    req = protocol.create_request('abort', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error')
    req = protocol.create_request('status', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error')
    assert response.result == 'READY'
    assert tt < 20


def test_timeout(sequencer_server, protocol, delay_mock):
    ss = sequencer_server
    ss.sequencer.sequence.stop_on_fail = False

    ss.sequencer.test_engine.send_request = delay_mock

    req = protocol.create_request('run', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error')
    assert ss.sequencer.running

    req = protocol.create_request('wait', 1)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error')
    assert response.result  # this should have time out
    req = protocol.create_request('abort', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error')

    m = mock.MagicMock()
    response = JSONRPCSuccessResponse()
    response.result = '--PASS--'
    response.unique_id = '0'
    response.version = '1.0'

    m.return_value = response
    ss.sequencer.test_engine.send_request = m

    req = protocol.create_request('run', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error')
    assert ss.sequencer.running

    req = protocol.create_request('wait', 1)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error')
    assert not response.result  # this should not have timedout


def test_plugin(ss2, protocol, mock_responses):
    te_protocol = TERPCProtocol()
    m = mock.MagicMock()
    m.side_effect = mock_responses
    ss2.sequencer.test_engine.send_request = m

    test = ss2.sequencer.sequence[0]
    assert test.call_engine
    assert test.function == 'station'

    test = ss2.sequencer.sequence['TEST_PLUGIN']
    assert not test.call_engine
    assert test.function == 'func_from_plugin'

    plugin = PluginServer()
    response = plugin.call(test)
    assert not hasattr(response, 'error'), response.error
    assert response.result == 6

    test = ss2.sequencer.sequence['TEST_PLUGIN_EXC']
    response = plugin.call(test)
    assert hasattr(response, 'error')
    assert response.error == 'exception from plugin'

    ss2.rpc_server.shutdown()


def test_etraveler(sequencer_server, protocol):
    ss = sequencer_server
    ss.sequencer.sequence.stop_on_fail = False

    ss.sequencer.test_engine.send_request = delay_mock

    req = protocol.create_request('run', ['wrong e_traveler data type'])
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert response.error == 'etraveler must be a dictionary'
    req = protocol.create_request('abort', None)
    ss.rpc_server.handle_message(0, req.serialize())

    req = protocol.create_request('run', dict(attribute='mydict'))
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert response.error == 'invalid key "attribute" in e_traveler'
    req = protocol.create_request('abort', None)
    ss.rpc_server.handle_message(0, req.serialize())

    req = protocol.create_request('run', dict(attributes='wrong attribute data_type'))
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert response.error == 'in e_traveler, attributes is not a dictionary'
    req = protocol.create_request('abort', None)
    ss.rpc_server.handle_message(0, req.serialize())
# todo test for overall results
# todo test single step is not allowed while a test is running
# todo add test for other functions exposed by sequencer server
# todo test list all with long list and empty list
