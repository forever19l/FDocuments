import pytest
from x527.statemachine import event_names, state_names
from x527.statemachine.smrpc import *
from x527.statemachine.smproxy import *
from x527.loggers.publisher import *
import mock


def test_protocol():
    protocol = StateMachienRPCProtocol()
    request = protocol.create_request(event_names.EVENT_FINISH, 2)
    assert request.function == event_names.EVENT_FINISH
    assert request.params == [2]
    assert request.version == STATEMACHINE_RPC_VERSION

    s_req = request.serialize()
    req = protocol.parse_request(s_req)
    assert req.function == event_names.EVENT_FINISH
    assert request.params == [2]
    assert request.version == STATEMACHINE_RPC_VERSION

    e_res = request.error_respond(JSONRPCInvalidParamsError())
    e_response = protocol.parse_reply(e_res.serialize())
    assert e_response.version == STATEMACHINE_RPC_VERSION
    assert e_response._jsonrpc_error_code == JSONRPCInvalidParamsError.jsonrpc_error_code
    assert e_response.error == JSONRPCInvalidParamsError.message
    assert e_response.unique_id == request.unique_id


    res = request.respond(state_names.STATE_TESTING)
    response = protocol.parse_reply(res.serialize())
    assert response.version == STATEMACHINE_RPC_VERSION
    assert not hasattr(response, 'error')
    assert response.result == state_names.STATE_TESTING
    assert response.unique_id == request.unique_id

def test_proxy():
    protocal = StateMachienRPCProtocol()
    sm_proxy = SMProxy(TestPublisher)
    request = protocal.create_request(event_names.EVENT_FINISH, 3)
    assert sm_proxy.transport.endpoint == 'tcp://localhost:6480'
    m = mock.MagicMock()
    m.side_effect = [request.respond(state_names.STATE_TESTING)]
    sm_proxy.send_request = m
    new_state = sm_proxy.finish(3)
    assert m.mock_calls == [mock.call(request, 50)]
    assert new_state == state_names.STATE_TESTING