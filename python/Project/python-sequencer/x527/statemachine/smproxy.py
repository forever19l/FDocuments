from x527.tinyrpc.proxy.rpcproxy import RPCProxy
from x527.tinyrpc.protocols.jsonrpc import JSONRPCTimeoutError
from x527.tinyrpc.transports.zmq import ZmqClientTransport
from x527 import zmqports
import zmq
from smrpc import StateMachienRPCProtocol, StateMachineRPCRequest

class SMProxy(RPCProxy):

    #timeout in seconds
    def __init__(self, publisher, url='tcp://localhost', retries=2, timeout=1, ctx=None):
        if not ctx:
            ctx = zmq.Context()
        super(SMProxy, self).__init__(ZmqClientTransport.create(ctx, url + ':' + str(zmqports.STATEMACHINE_PORT)),
                                      StateMachienRPCProtocol(),
                                      publisher,
                                      retries)
        self.identity = 'SMProxy'

    def send_cmd(self, event_name, event_data, timeout):
        req = self.protocol.create_request(event_name, event_data)
        response = self.send_request(req, 50) #we are always going to use 50 as the timeout for statemachine calls
        return response