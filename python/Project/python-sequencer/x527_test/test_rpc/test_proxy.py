__author__ = 'wei'

import pytest
import mock
from x527.tinyrpc.protocols.terpc import TERPCProtocol
from x527.tinyrpc.protocols.sequencerrpc import SequencerRPCProtocol
from x527.tinyrpc.proxy.teproxy import TEProxy
from x527.tinyrpc.proxy.sequencerporxy import SequencerProxy
from x527.loggers import TestPublisher
from x527.tinyrpc.protocols.jsonrpc import *
from x527.sequencer.sequence import *
from  datetime import datetime
import json

msg_list = []

@pytest.fixture
def send_mock():
    return mock.MagicMock()

@pytest.fixture
def recv_mock():
    m = mock.MagicMock()
    response = JSONRPCSuccessResponse()
    response.result = '7000'
    response.unique_id = '0'
    response.version = '1.0'
    m.return_value = response
    return m

@pytest.fixture
def test_item():
    # test_dict = dict(FUNCTION='func', TID='test_name', PARAM1='10', PARMA2='', VALIDATE='', KEY='', VAL='')
    test_dict = {}
    test_dict['GROUP'] = 'group'
    test_dict['FUNCTION'] = 'func'
    test_dict['TID'] = 'test_name'
    test_dict['LOW'] = ''
    test_dict['HIGH']=''
    test_dict['PARAM1'] = '10'
    test_dict['PARAM2'] = '{{varialbe}}'
    test_dict['TIMEOUT'] = ''
    test_dict['UNIT'] = ''
    test_dict['VALIDATE'] = ''
    test_dict['KEY'] = ''
    test_dict['VAL'] = ''
    test_item = TestItem(test_dict)
    return test_item

@pytest.fixture
def publisher():
    publisher = TestPublisher('proxy_test', msg_list)
    return publisher

@pytest.fixture
def clear_messages():
    global msg_list
    msg_list = []

def test_teproxy(recv_mock, publisher, test_item, send_mock, clear_messages):
    proxy = TEProxy(0, publisher)
    proxy.transport.socket.send = send_mock
    proxy.transport.socket.recv = recv_mock
    protocol = TERPCProtocol()

    delay = 100
    test_item.timeout = delay
    assert len(publisher.msg_list) == 0
    try:
        proxy.send_test(test_item)
    except JSONRPCTimeoutError:
        pass # we will get a timeout error because no test engine is running and the poller never gets polled
    req = protocol.create_request('func', ['10'], timeout=100) #PARAM2 should have been poped
    send_req = protocol.parse_request(send_mock.call_args[0][0])
    assert send_req == req

    #check that timeout value is correct
    ts0 = datetime.strptime(publisher.msg_list[0][0], '%m-%d_%H:%M:%S.%f')
    ts1 = datetime.strptime(publisher.msg_list[1][0], '%m-%d_%H:%M:%S.%f')
    tt = (ts1-ts0).total_seconds()
    assert tt>delay/1000.0, '\n'.join([str(msg) for msg in publisher.msg_list])
    assert tt<delay/1000.0+0.5, '\n'.join([str(msg) for msg in publisher.msg_list])

def test_sequencerproxy(recv_mock, send_mock, publisher, clear_messages):
    proxy = SequencerProxy(0, publisher)
    proxy.transport.socket.send = send_mock
    proxy.transport.socket.recv = recv_mock
    protocol = SequencerRPCProtocol()
    with pytest.raises(JSONRPCTimeoutError):
        proxy.send_cmd('func', 'p1', 100)

    req = protocol.create_request('func', 'p1')
    send_req = protocol.parse_request(send_mock.call_args[0][0])
    assert req==send_req
