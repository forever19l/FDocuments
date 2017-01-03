from jsonrpc import *

TE_RPC_VERSION = '1.0'

class TERPCServerError(FixedErrorMessageMixin, ServerError):
    jsonrpc_error_code = -40001
    message = 'Test Engine Internal error'

    def __init__(self, msg=''):
        super(TERPCServerError, self).__init__()
        if msg:
            self.message = msg

class TERPCRequest(JSONRPCRequest):

    timeout = 3000

    def error_respond(self, error):
        response = super(TERPCRequest, self).error_respond(error)
        response.version = TE_RPC_VERSION
        return response

    def respond(self, result):
        response = super(TERPCRequest, self).respond(result)
        response.version = TE_RPC_VERSION
        return response

    def __eq__(self, other):
        if self.version!=other.version:
            return False
        if self.function!=other.function:
            return False
        if self.params!=other.params:
            return False
        if self.timeout!=other.timeout:
            return False
        if self.unit!=other.unit:
            return False
        return True

    def _to_dict(self):
        jdata = {
            'jsonrpc':self.version,
            'id':self.unique_id,
            'function':self.function,
            'params': self.params,
            'timeout': self.timeout,
            'unit' : self.unit
        }
        return jdata
        
class TERPCProtocol(JSONRPCProtocol):

    '''
    if no error, the reply should be: dict(id='some_id', jsonrpc='version', result=PASS/FAIL, returned='some value'}
    if the function call expects a result, meaning Param2 is not emptied, then returned should have the returned value,
    otherwise there should be no returned
    if error, the reply should be: dict(id='some_id', 'jsonrpc'='version', 'error'=dict(code=error_code, message='error message'}
    '''
    _ALLOWED_REPLY_KEYS = sorted(['id', 'jsonrpc', 'error', 'result'])

    _ALLOWED_REQUEST_KEYS = sorted(['id', 'jsonrpc', 'function', 'params', 'timeout', 'unit'])

    version = TE_RPC_VERSION

    def __init__(self, *args, **kwargs):
        super(TERPCProtocol, self).__init__(*args, **kwargs)

    def create_request(self, function, params, unit='', timeout=3000):
        request = TERPCRequest()

        request.unique_id = self._get_unique_id()
        request.version = self.version
        request.function = function
        if params is None:
            request.params = []
        else:
            request.params = params
        request.timeout = timeout
        request.unit = unit
        return request

    def parse_reply(self, data):
        #receive variable value
        try:
            rep = json.loads(data)
        except Exception as e:
            raise InvalidReplyError(e)

        for k in rep.keys():
            if not k in self._ALLOWED_REPLY_KEYS:
                raise InvalidReplyError('Key not allowed: %s' % k)

        if not 'jsonrpc' in rep:
            raise InvalidReplyError('Missing jsonrpc (version) in response.')

        if rep['jsonrpc'] != TE_RPC_VERSION:
            raise InvalidReplyError('Wrong JSONRPC version')

        if not 'id' in rep:
            raise InvalidReplyError('Missing id in response')

        if 'error' in rep:
            response = JSONRPCErrorResponse()
            error = rep['error']
            response.error = error['message']
            response.version=rep['jsonrpc']
            response.unique_id = rep['id']
            response._jsonrpc_error_code = error['code']
        else:
            response = JSONRPCSuccessResponse()
            response.result = rep.get('result', None)
            response.unique_id = rep.get('id')
            response.version = rep.get('jsonrpc')

        response.unique_id = rep['id']
        return response


    def _parse_subrequest(self, req):
        for k in req.keys():
            if not k in self._ALLOWED_REQUEST_KEYS:
                raise JSONRPCInvalidRequestError('key in request not allowed')

        if req.get('jsonrpc', None) != TE_RPC_VERSION:
            raise JSONRPCInvalidRequestError('json rpc version unmatched')

        if 'function' not in req:
            raise JSONRPCInvalidRequestError('missing function in request')

        if 'timeout' not in req:
            raise JSONRPCInvalidRequestError('no timeout field in test engine request')

        if 'unit' not in req:
            raise JSONRPCInvalidRequestError('no unit field in test engine request')

        request = TERPCRequest()
        request.function = str(req['function']).lower()
        request.unique_id = req.get('id', '0')
        request.params = req.get('params', None)
        request.version = req.get('jsonrpc')
        request.timeout = req.get('timeout')
        request.unit = req.get('unit')
        return request

