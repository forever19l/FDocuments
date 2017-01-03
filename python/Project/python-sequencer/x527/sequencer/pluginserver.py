import importlib
from x527.tinyrpc.protocols.terpc import *
from x527.tinyrpc.protocols.jsonrpc import JSONRPCPluginError

class PluginServer(object):

    def __init__(self):
        self.sequence = None
        try:
            self.plugin = importlib.import_module('.fctplugin_n121', package='x527.sequencer.plugins')
        except ImportError:
            self.plugin = None

    def respond(self, result):
        req = TERPCRequest()
        req.unique_id = 1
        return req.respond(result)

    def error_respond(self, error_msg):
        error = JSONRPCPluginError(error_msg)
        req = TERPCRequest()
        req.unique_id = 1
        return req.error_respond(error)

    def call(self, test):
        if self.plugin is None:
            return self.error_respond('No plugin was imported')
        try:
            func = getattr(self.plugin, test.function)
            test_dict = test._to_dict()
            result = func(test_dict['params'], test.unit, test.timeout, self.sequence)
            return self.respond(result)
        except Exception as e:
            return self.error_respond(e.message)

    def invoke(self, func_name, *args):
        func = getattr(self.plugin, func_name)
        return func(*args)