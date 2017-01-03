import zmq

from threading import Thread
import time
from x527.tinyrpc.protocols.jsonrpc import RPCError
from x527.tinyrpc.protocols.terpc import TERPCProtocol
from x527.tinyrpc.server import RPCServer
from x527 import zmqports
from x527.tinyrpc.proxy.teproxy import TEProxy
from x527.loggers import StdOutPublisher, ZmqPublisher
from x527.tinyrpc.protocols import jsonrpc
import cmd
import argparse
import csv

class DummyDispatcher(object):
    func_list = []
    default_ret = '6000'
    def __init__(self, publisher):
        self.publisher = publisher

    def dispatch(self, request):
        self.publisher.publish(request.serialize(), 'Dispatcher received request')
        self.func_list.append(request.function)
        time.sleep(0.05)
        if request.function == jsonrpc.SERVER_READY:
            return request.respond('--PASS--')
        if request.function == 'long_test':
            time.sleep(5)
            return request.respond(True)
        if request.function == 'fixturetype':
            return request.respond('SIPFIXTURE')
        if request.function == 'station':
            return request.respond('COMBINE-FCT')
        if request.function == 'getsn':
            return request.respond('YM123456789')
        if request.function == 'measure':
            return request.respond(1.1101833258)
        if request.function == 'end_test':
            return request.respond('')
        if request.function == 'start_test':
            return request.respond('')
        return request.respond(self.default_ret)

class ReplayDispatcher(object):
    def __init__(self, publisher, replay_file):
        self.publisher = publisher
        self.answers = []
        f = open(replay_file, 'rU')
        reader = csv.DictReader(f)
        for row in reader:
            self.answers.append(row['Return'])
        f.close()

    def dispatch(self, request):
        if request.function!='::stop::':
            return request.respond(self.answers.pop())

class DummyTestEngine(Thread):
    def __init__(self, site):
        super(DummyTestEngine, self).__init__()
        self.site = site

        ctx = zmq.Context().instance()
        # Ensure subscriber connection has time to complete
        time.sleep(1)
        self.publisher = ZmqPublisher(ctx, "tcp://*:" + str(zmqports.TEST_ENGINE_PUB + site), "DummyEngine_{}".format(site))
        time.sleep(0.5)  # give time for the subscribers to connect to this publisher
        self.rpc_server = RPCServer(
            zmqports.TEST_ENGINE_PORT + site,
            TERPCProtocol(),
            DummyDispatcher(self.publisher),
            self.publisher
        )

    def run(self):
        self.publisher.publish('Test Engine {} Starting...'.format(self.site))
        self.rpc_server.serve_forever()
        self.publisher.publish('Test Engine {} Stopped...'.format(self.site))

def client_routine(site, function, params, timeout=1000):
    try:
        proxy = TEProxy(site, StdOutPublisher('TEProxy'))
        proxy.send_cmd(function, params, timeout)
    except RPCError as e:
        print e.message

def test_engine(site):
    t1 = Thread(target=client_routine, args=(site, jsonrpc.SERVER_READY, None, 500))
    t1.start()
    t2 = Thread(target=client_routine, args=(site, 'short', None))
    t2.start()
    time.sleep(0.5)
    # if the server receives the stop request first, the other client will hang waiting for reply
    t3 = Thread(target=client_routine, args=(site, '::stop::', None))
    t3.start()

    time.sleep(0.1)
    t4 = Thread(target=client_routine, args=(site, 'test_retry', None))
    t4.start()

    t1.join()
    t2.join()
    t3.join()
    t4.join()
    #te.join()

class EngineDriver(cmd.Cmd):
    prompt = 'test engine>'
    intro = 'Dummy Engine driver'
    driver = None

    def default(self, line):
        self.proxy.send_cmd(line, None)

    def do_quit(self, line):
        self.proxy.send_cmd('::stop::', None)
        return True

def start_client(site):
    driver = EngineDriver()
    driver.proxy = TEProxy(site, StdOutPublisher('TEProxy'))
    driver.cmdloop()

def start_server(site):
    te = DummyTestEngine(site)
    te.start()
    return te

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-t', '--test', help='run a test', action='store_true')
    parser.add_argument('-s', '--site', help='the site of the sequencer to connect to', type=int, default=0)
    parser.add_argument('-o', '--server_only', help='only start the server', action='store_true', default=False)
    parser.add_argument('-c', '--client_only', help='only start the client', action='store_true', default=False)
    args = parser.parse_args()

    if args.test:
        test_engine(args.site)
        exit()

    site = args.site
    if args.client_only:
        start_client(site)
        exit()
    if args.server_only:
        server =start_server(site)
        server.join()
        exit()

    server = start_server(site)
    time.sleep(1)
    start_client(site)



    server.join()