__author__ = 'wei'
import pytest

from x527.tinyrpc.server import *
from x527 import zmqports
from x527.tinyrpc.protocols.terpc import *
from x527.tinyrpc.protocols import terpc
from x527.loggers import TestPublisher
te_msg_list = []

class TestDispatcher(object):
    def __init__(self, publisher):
        self.publisher = publisher
        self.received_calls = []

    def dispatch(self, request):
        self.publisher.publish(request.serialize(), 'Dispatcher received request')
        if request.function == 'my_func':
            self.received_calls.append([request.function, request.params])
            return request.respond(47)
        elif request.function == 'invalid_param':
            raise JSONRPCInvalidParamsError('mock error')

@pytest.fixture(scope='module')
def test_engine(request):
    publisher = TestPublisher('TEST_ENGINE', te_msg_list)
    rpc_server = RPCServer(
        zmqports.TEST_ENGINE_PORT + 0,
        TERPCProtocol(),
        TestDispatcher(publisher),
        publisher
    )

    def fin():
        rpc_server.shutdown()
        print('te_server shutting down')
    request.addfinalizer(fin)

    return rpc_server

@pytest.fixture
def protocol():
    return TERPCProtocol()

def test_handle_msg(test_engine, protocol):
    req = protocol.create_request('my_func', [1, '2'])
    response = test_engine.handle_message(0, req.serialize())
    assert response.version == terpc.TE_RPC_VERSION
    assert response.result == 47
    assert response.unique_id == req.unique_id
    assert not hasattr(response, 'error')

    req = protocol.create_request('invalid_param', None)
    response = test_engine.handle_message(0, req.serialize())
    assert response.version == terpc.TE_RPC_VERSION
    assert response._jsonrpc_error_code == -32602
    assert response.error == 'mock error'
    assert not hasattr(response, 'result')
    assert response.unique_id == req.unique_id #can not guarantee a unique id if error parsing the request

# todo: test version not matched