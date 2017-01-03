__author__ = 'wei'
from rpcproxy import RPCProxy
from x527.tinyrpc.protocols.jsonrpc import JSONRPCTimeoutError
from x527.tinyrpc.protocols.terpc import TERPCProtocol
from x527.tinyrpc.transports.zmq import ZmqClientTransport
from x527 import zmqports
import zmq

class TEProxy(RPCProxy):

    #timeout in seconds
    def __init__(self, site, publisher, url='tcp://localhost', retries=2, timeout=1, ctx=None):
        if not ctx:
            ctx = zmq.Context.instance()
        super(TEProxy, self).__init__(ZmqClientTransport.create(ctx, url + ':' + str(zmqports.TEST_ENGINE_PORT+site)),
                                      TERPCProtocol(),
                                      publisher,
                                      retries)
        self.identity = 'TEProxy'

    def send_test(self, test_item):
        test_dict = test_item._to_dict()
        req = self.protocol.create_request(test_dict['function'], test_dict['params'], test_item.unit, test_item.timeout)
        response = self.send_request(req, test_item.timeout)
        if response is None:
            raise JSONRPCTimeoutError('time out calling ' + test_item.function + '('
                                      + str(test_dict['params']) + ')')
        return response

    def send_cmd(self, func, params, unit='', timeout=3000):
        req = self.protocol.create_request(func, params, unit, timeout)
        response = self.send_request(req, timeout)
        if response is None:
            raise JSONRPCTimeoutError('time out calling ' + func + '('
                                      + str(params) + ')')
        return response