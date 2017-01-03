import os
import time
from multiprocessing import Process
from x527.sequencer.sequencer import SequencerServer
from x527.tinyrpc.protocols.sequencerrpc import *
from x527.engine import testengine


def run_sequence(num):
    protocol = SequencerRPCProtocol()
    path = os.path.split(__file__)[0]
    plan_path = os.path.join(path, 'short_plan__1.0.csv')
    ss = SequencerServer(num+20, 1)
    te = testengine.DummyTestEngine(num+20)
    te.start()

    ss.sequencer.load_sequence(plan_path)
    ss.sequencer.stop_on_fail = False
    ss.sequencer.connect()

    te.rpc_server.dispatcher.func_list = []

    req = protocol.create_request('run', None)
    response = ss.rpc_server.handle_message(0, req.serialize())
    assert not hasattr(response, 'error'), response.error
    while ss.sequencer.running:
        time.sleep(0.2)  # wait for sequencer to finish

    sequence = ss.sequencer.sequence
    assert sequence.variables['unitStage'] == 'COMBINE-FCT'
    expected = []
    for item in sequence:
        expected.append(item.function)
    assert expected == te.rpc_server.dispatcher.func_list[1:-1]

    testengine.client_routine(num+20, '::stop::', None)

    te.join()

    # print('testengine_{} shutting down'.format(num+20))
    ss.rpc_server.shutdown()
    # print('sequencer_{} shutting down'.format(num+20))


def test_run_multiprocess():
    process = [Process(target=run_sequence, args=(i,)) for i in range(6)]

    for p in process:
        p.start()

    for p in process:
        p.join()

    # time.sleep(5)
    for p in process:
        # print 'Before:', p, p.is_alive()
        if p.is_alive():
            p.terminate()
            print '%s.exitcode = %s' % (p, p.exitcode)
            print 'TERMINATED:', p, p.is_alive()
        assert not p.exitcode
