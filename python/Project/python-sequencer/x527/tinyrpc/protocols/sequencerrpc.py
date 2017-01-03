from jsonrpc import *
import json


SEQUENCER_RPC_VERSION = '1.0'

class SequencerRPCRequest(JSONRPCRequest):
    def error_respond(self, error):
        response = super(SequencerRPCRequest, self).error_respond(error)
        response.version = SEQUENCER_RPC_VERSION
        return response

    def respond(self, result):
        response = super(SequencerRPCRequest, self).respond(result)
        response.version = SEQUENCER_RPC_VERSION
        return response

    def _to_dict(self):
        jdata = {
            'jsonrpc':self.version,
            'id':self.unique_id,
            'function':self.function,
            'params':self.params
        }
        return jdata


class SequencerRPCProtocol(JSONRPCProtocol):
    """JSONRPC protocol implementation.
    """

    _ALLOWED_REPLY_KEYS = sorted(['id', 'jsonrpc', 'error', 'result'])
    _ALLOWED_REQUEST_KEYS = sorted(['id', 'jsonrpc', 'function', 'params', 'timeout'])

    version = SEQUENCER_RPC_VERSION

    def __init__(self, *args, **kwargs):
        super(SequencerRPCProtocol, self).__init__(*args, **kwargs)

    def create_request(self, verb, line):

        request = SequencerRPCRequest()
        request.unique_id = self._get_unique_id()
        request.version = self.version

        request.function = verb
        if line is None:
            request.params = None
        else:
            request.params = [line]
        return request

    def parse_reply(self, data):
        try:
            rep = json.loads(data)
        except Exception as e:
            raise InvalidReplyError(e)

        for k in rep.keys():
            if not k in self._ALLOWED_REPLY_KEYS:
                raise InvalidReplyError('Key not allowed: %s' % k)

        if not 'jsonrpc' in rep:
            raise InvalidReplyError('Missing jsonrpc (version) in response.')

        if rep['jsonrpc'] != SEQUENCER_RPC_VERSION:
            raise InvalidReplyError('Wrong JSONRPC version')

        if not 'id' in rep:
            raise InvalidReplyError('Missing id in response')

        if 'error' in rep:
            response = JSONRPCErrorResponse()
            error = rep['error']
            response.error = error['message']
            response._jsonrpc_error_code = error['code']
        else:
            response = JSONRPCSuccessResponse()
        response.result = rep.get('result', None)
        response.version = rep['jsonrpc']

        response.unique_id = rep['id']
        return response


    def _parse_subrequest(self, req):
        for k in req.keys():
            if not k in self._ALLOWED_REQUEST_KEYS:
                raise JSONRPCInvalidRequestError(k + ' is not a valid request key')
        
        if req.get('jsonrpc', None) != SEQUENCER_RPC_VERSION:
            raise JSONRPCInvalidRequestError('json rpc version unmatched')

        if not 'function' in req:
            raise JSONRPCInvalidRequestError('no verb in the request')

        request = SequencerRPCRequest()
        request.function = str(req['function']).lower()
        request.version = req['jsonrpc']
        request.params = req['params']
        request.unique_id = req.get('id', None)
        return request
