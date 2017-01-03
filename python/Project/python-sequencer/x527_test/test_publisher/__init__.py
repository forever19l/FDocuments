__author__ = 'wei'

from x527.loggers import LogSubscriber

class TestLogSubscriber(LogSubscriber):
    def __init__(self, port, level, url=None):
        super(TestLogSubscriber, self).__init__(port, level, url)
        self.messages = []

    def handle_msg(self, msg):
        self.messages.append(msg)