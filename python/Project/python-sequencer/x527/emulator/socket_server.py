from threading import Thread
from socket import *
import zmq
from x527.emulator import *
from x527 import zmqports
from x527.loggers import events, levels
from x527.loggers.reporter import ReporterProtocol
from datetime import datetime


class PortServer(Thread):
    def __init__(self, name, host, port, queue, verbose=False):
        super(PortServer, self).__init__()
        self.port = port
        self.sock = socket(AF_INET, SOCK_STREAM)
        self.sock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
        self.sock.bind((host, port))
        self.sock.listen(5)
        self.connection = None
        self.verbose = verbose
        self.active = True
        self.queue = queue
        self.name = name

    def ts(self):
        if self.verbose:
            return str(datetime.now())+'\t'+self.name+'\t'
        return ''

    def send(self, buf):
        if self.connection:
            if buf[-1] == '\r':
                buf += '\n'
            elif buf[-1] == '\n':
                pass
            else:
                buf += '\r\n'
            self.connection.sendall(buf)
            return True
        else:
            return False

    def queue_in(self, item):
        self.queue.put_nowait(item)

    def close(self):
        self.active = False
        print self.ts()+'server stopping ...'

    def run(self):
        while self.active:
            print self.ts()+'waiting for connection ...'
            self.connection, address = self.sock.accept()
            print self.ts()+'received connection from ', address
        self.connection.close()
        self.sock.shutdown(SHUT_RDWR)
        print self.ts()+'Port {} closed'.format(self.port)


class CmdServer(PortServer):
    def run(self):
        while self.active:
            print self.ts()+'waiting for connection ...'
            self.connection, address = self.sock.accept()
            connection = self.connection
            print self.ts()+'received connection from ', address

            while self.active:
                data = connection.recv(BUFSIZE)
                if not data:
                    break
                print self.ts()+'cmd received: '+data
                self.queue_in([self.name, data])
            connection.close()
        self.sock.shutdown(SHUT_RDWR)
        print self.ts()+'Port {} closed'.format(self.port)


class SeqListener(Thread):
    event_function_map = {
                          events.SEQUENCE_START: 'log_sequence_start',
                          events.SEQUENCE_END: 'log_sequence_end',
                          events.ITEM_START: 'log_item_start',
                          events.ITEM_FINISH: 'log_item_finish',
                          events.ATTRIBUTE_FOUND: 'log_attribute_found'
                          }

    def __init__(self, host=None, site=0, queue=None, verbose=False):
        super(SeqListener, self).__init__()
        ctx = zmq.Context()
        self.site = site
        self.poller = zmq.Poller()
        self.subscriber = ctx.socket(zmq.SUB)
        self.receiving = True
        self.protocol = ReporterProtocol()
        self.url = host
        sub = self.subscriber
        sub.setsockopt(zmq.IDENTITY, str(site))
        sub.connect(self.url + str(zmqports.SEQUENCER_PUB + site))
        sub.setsockopt(zmq.SUBSCRIBE, zmqports.PUB_CHANNEL)
        self.poller.register(sub, zmq.POLLIN)
        self.tid = None
        self.verbose = verbose
        self.queue = queue
        self.name = 'SEQ'

    def ts(self):
        if self.verbose:
            return str(datetime.now())+'\t'+self.name+'\t'
        return ''

    def queue_in(self, item):
        self.queue.put_nowait(item)

    def run(self):
        print self.ts()+'started({})'.format(self.site)
        while self.receiving:
            try:
                socks = dict(self.poller.poll(1000))
                sub = self.subscriber
                if sub in socks and socks[sub] == zmq.POLLIN:
                    msg = sub.recv_multipart(zmq.NOBLOCK)
                    topic, ts, level, origin, data = msg[:]
                    if int(level) > levels.REPORTER:
                        continue
                    try:
                        report = self.protocol.parse_report(data)
                        if int(report.event) == events.ITEM_START:
                            self.tid = str(report.data['tid'])
                            print self.ts()+'TID:', self.tid
                            self.queue_in(['TID', self.tid])
                    except ValueError as e:
                        print self.ts()+'error parsing the data: ' + str(data) + '; error detail is: ' + e.message
                        continue
            except zmq.ZMQError as e:
                print e.message
        sub = self.subscriber
        sub.setsockopt(zmq.LINGER, 0)
        sub.close()
