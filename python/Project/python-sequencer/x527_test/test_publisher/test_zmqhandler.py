import pytest
from x527.statemachine.core import ZMQHandler
import logging
from x527 import zmqports
from x527.loggers import levels
import time
from x527_test.test_publisher import TestLogSubscriber

def test_handler():
    port = str(zmqports.SM_PUB)
    handler = ZMQHandler('tcp://*:' + port)
    logger = logging.getLogger('test_zmq')
    logger.addHandler(handler)
    logger.setLevel(logging.DEBUG)
    time.sleep(2)
    s = TestLogSubscriber(port, levels.DEBUG)
    s.start()
    time.sleep(0.1)
    logger.debug('my message')
    time.sleep(0.1)
    msg = s.messages[0]
    s.receiving = False
    handler.close()
    assert msg[0] == zmqports.PUB_CHANNEL
    assert int(msg[2]) == levels.DEBUG
    assert msg[3] == 'statemachine_core'
    assert msg[4] == 'my message'


