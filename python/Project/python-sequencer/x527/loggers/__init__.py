from publisher import *
import zmq
from x527 import zmqports
from threading import Thread
import time
import levels

class LogSubscriber(Thread):
    def __init__(self, port, level, url=None):
        super(LogSubscriber, self).__init__()
        ctx = zmq.Context.instance()
        self.subscriber = ctx.socket(zmq.SUB)
        if url is None:
            url = 'tcp://localhost:' + str(port)
        self.subscriber.connect(url)
        self.url = url
        self.subscriber.setsockopt(zmq.SUBSCRIBE, zmqports.PUB_CHANNEL)
        self.receiving = True
        self.level = level

    def run(self):
        print 'ready to subscribe to ' + str(self.url)
        while self.receiving:
            try:
                msg = self.subscriber.recv_multipart(zmq.NOBLOCK)
                self.handle_msg(msg)
            except zmq.ZMQError:
                pass
            time.sleep(0.02)
        self.subscriber.setsockopt(zmq.LINGER, 0)
        self.subscriber.close()
