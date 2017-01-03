# __author__ = 'wei'

import os
import zmq
import time
import argparse
import levels
import events
import traceback
from threading import Thread
from datetime import datetime
from x527 import zmqports
from x527.tinyrpc import *
from filelogger import CSVLogger, PivotLogger, FlowLogger, FlowPlainLogger, UartLogger, ArmLogger, DataloggerLogger, EngineLogger
from puddinglogger import PuddingLogger
from publisher import ZmqPublisher
from reporter import ReporterProtocol


class LoggerThread(Thread):

    event_function_map = {
                          events.SEQUENCE_START:    'log_sequence_start',
                          events.SEQUENCE_END:      'log_sequence_end',
                          events.ITEM_START:        'log_item_start',
                          events.ITEM_FINISH:       'log_item_finish',
                          events.ATTRIBUTE_FOUND:   'log_attribute_found',
                        }

    def __init__(self, publisher, site_count=6, url='tcp://localhost:'):
        super(LoggerThread, self).__init__()
        ctx = zmq.Context.instance()
        self.poller = zmq.Poller()
        self.subscribers = [ctx.socket(zmq.SUB) for i in range(site_count)]
        self.uart_subscribers = [ctx.socket(zmq.SUB) for i in range(site_count)]
        self.arm_subscribers = [ctx.socket(zmq.SUB) for i in range(site_count)]
        self.dl_subscribers = [ctx.socket(zmq.SUB) for i in range(site_count)]
        self.te_subscribers = [ctx.socket(zmq.SUB) for i in range(site_count)]
        self.logger = None
        self.receiving = True
        self.protocol = ReporterProtocol()
        self.publisher = publisher
        for i in range(site_count):
            self.register(self.subscribers[i], url, zmqports.SEQUENCER_PUB, i, zmqports.PUB_CHANNEL)
            self.register(self.uart_subscribers[i], url, zmqports.UART_PUB, i, '')
            self.register(self.arm_subscribers[i], url, zmqports.ARM_PUB, i, '')
            self.register(self.dl_subscribers[i], url, zmqports.DATALOGGER_PUB, i, '')
            self.register(self.te_subscribers[i], url, zmqports.TEST_ENGINE_PUB, i, zmqports.PUB_CHANNEL)

    def register(self, sub, url, baseport, offset, channel):
        sub.setsockopt(zmq.IDENTITY, str(offset))
        sub.connect(url + str(baseport + offset))
        sub.setsockopt(zmq.SUBSCRIBE, channel)
        self.poller.register(sub, zmq.POLLIN)

    @staticmethod
    def shutdown(subs):
        for sub in subs:
            sub.setsockopt(zmq.LINGER, 0)
            sub.close()

    def addon_log(self, sub, socks, id):
        if sub in socks and socks[sub] == zmq.POLLIN:
            try:
                while self.receiving:
                    msg = sub.recv_multipart(zmq.NOBLOCK)
                    site = int(sub.getsockopt(zmq.IDENTITY))
                    func = self.logger.__getattribute__('log_data')
                    if 'engine' in id:
                        topic, ts, level, origin, data = msg[:]
                    else:
                        data = msg[0]
                    func(None, site, data, id)
            except:
                pass

    def run(self):
        self.publisher.publish('logger started', level=levels.INFO, id_postfix='logger thread')
        self.heartbeat_at = time.time() + HEARTBEAT_INTERVAL
        while self.receiving:
            try:
                socks = dict(self.poller.poll(1000))
                for sub in self.subscribers:
                    if sub in socks and socks[sub] == zmq.POLLIN:
                        try:
                            while self.receiving:
                                msg = sub.recv_multipart(zmq.NOBLOCK)
                                topic, ts, level, origin, data = msg[:]
                                if int(level) > levels.REPORTER:
                                    continue
                                try:
                                    report = self.protocol.parse_report(data)
                                except ValueError as e:
                                    print 'error parsing the data: ' + str(data) + '; error detail is: ' + e.message
                                    continue
                                site = int(sub.getsockopt(zmq.IDENTITY))
                                if report.event not in self.event_function_map:
                                    continue
                                func_name = self.event_function_map[report.event]
                                func = self.logger.__getattribute__(func_name)
                                func(ts, site, report)
                        except:
                            pass
                for sub in self.uart_subscribers:
                    self.addon_log(sub, socks, 'uart')
                for sub in self.arm_subscribers:
                    self.addon_log(sub, socks, 'arm')
                for sub in self.dl_subscribers:
                    self.addon_log(sub, socks, 'dl')
                for sub in self.te_subscribers:
                    self.addon_log(sub, socks, 'engine')
            except zmq.ZMQError as e:
                print e.message, traceback.format_exc()
            t_now = time.time()
            if t_now >= self.heartbeat_at:
                self.publisher.publish(FCT_HEARTBEAT, level=levels.INFO, id_postfix='logger thread')
                self.heartbeat_at = t_now + HEARTBEAT_INTERVAL
        self.logger.close()
        self.shutdown(self.subscribers)
        self.shutdown(self.uart_subscribers)
        self.shutdown(self.arm_subscribers)
        self.shutdown(self.dl_subscribers)
        self.shutdown(self.te_subscribers)


def make_loggers(logger_str, path, publisher):
    logger, p = None, None
    # PuddingLogger has to be the end of logger chain to process the sequence_end after other file loggers finish
    if 'p' in logger_str:
        p = PuddingLogger(log_folder=path, publisher=publisher, next_logger=logger)
        logger = p
    if 'v' in logger_str:
        v = PivotLogger(log_folder=path, publisher=publisher, next_logger=logger)
        logger = v
        if p:
            p.loggers.append(v)
    if 'c' in logger_str:
        c = CSVLogger(log_folder=path, publisher=publisher, next_logger=logger)
        logger = c
    if 'f' in logger_str:
        f = FlowLogger(log_folder=path, publisher=publisher, next_logger=logger)
        logger = f
        if p:
            p.loggers.append(f)
    if 's' in logger_str:
        s = FlowPlainLogger(log_folder=path, publisher=publisher, next_logger=logger)
        logger = s
        if p:
            p.loggers.append(s)
    if 'd' in logger_str:
        d = DataloggerLogger(log_folder=path, publisher=publisher, next_logger=logger)
        logger = d
        if p:
            p.loggers.append(d)
    if 'a' in logger_str:
        a = ArmLogger(log_folder=path, publisher=publisher, next_logger=logger)
        logger = a
        if p:
            p.loggers.append(a)
    if 'u' in logger_str:
        u = UartLogger(log_folder=path, publisher=publisher, next_logger=logger)
        logger = u
        if p:
            p.loggers.append(u)
    if 'e' in logger_str:
        e = EngineLogger(log_folder=path, publisher=publisher, next_logger=logger)
        logger = e
        if p:
            p.loggers.append(e)

    for each in logger_str:
        if each not in ['p', 'v', 'c', 'f', 's', 'd', 'a', 'u', 'e']:
            print 'unsupported logger "'+each+'".'

    return logger


if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('loggers',
                        help='List of the loggers your want to enable, p=pudding, v=pivot, c=csv, f=flow, s=flow_plain,'
                             ' d=datalogger, a=ARM, u=UART, e=Engine'
                        )
    parser.add_argument('-d', '--directory', help='the log directory, default to "/tmp"', default='/tmp')
    args = parser.parse_args()
    if not os.path.exists(args.directory):
        os.mkdir(args.directory)
    ctx = zmq.Context()
    publisher = ZmqPublisher(ctx, "tcp://*:" + str(zmqports.LOGGER_PUB), 'logger')
    t = LoggerThread(publisher)
    t.logger = make_loggers(args.loggers, args.directory, publisher)

    time.sleep(0.2)
    t.start()

    raw_input()
    t.receiving = False
