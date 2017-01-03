import pytest
from x527.statemachine import event_names, state_names
from x527.statemachine.smrpc import *
from x527.statemachine.smproxy import *
from x527.loggers.publisher import *
from x527.statemachine.statemachine import *
import mock

@pytest.fixture
def fixture():
    fixture = mock.MagicMock()
    fixture.engage = mock.MagicMock()
    fixture.disengage = mock.MagicMock()
    fixture.close = mock.MagicMock()
    return mock.MagicMock()

@pytest.fixture
def sequencers():
    mock_seq = [mock.MagicMock() for i in range(4)]
    for m in mock_seq:
        m.running = False
    return mock_seq

def change_seq_states(sequencers, sites, new_state ):
    for site in sites:
        sequencers[site].running = new_state

def test_transitions(fixture, sequencers):
    sm = TesterStateMachine(fixture, sequencers)
    assert sm.state=='idle'
    sm.dut_ready()
    assert sm.state=='ready_to_load'
    assert fixture.mock_calls[0] == ('open', (), {})
    sm.abort()
    assert sm.state=='idle'
    assert fixture.mock_calls[1] == ('close', (), {})
    sm.dut_ready()
    assert fixture.mock_calls[2] == ('open', (), {})

    #start testing
    e_travelers ={1:{'sn':'001'}, 2:{'sn':'002'}, 3:{'sn':'003'}}
    sm.start(e_travelers)
    assert fixture.mock_calls[3] == ('engage', (), {})
    assert sm.state=='testing'
    assert sequencers[1].mock_calls[0]==('s_run', (e_travelers[1],))
    assert sequencers[2].mock_calls[0]==('s_run', (e_travelers[2],), )
    assert sequencers[3].mock_calls[0]==('s_run', (e_travelers[3],), )
    assert len(sequencers[0].mock_calls) == 0
    change_seq_states(sequencers, [1,2,3], True )
    e_travelers = {0:{'sn':000}}
    sm.start(e_travelers)
    assert sm.state == 'testing'
    assert sequencers[0].mock_calls[0]==('s_run', (e_travelers[0],), )
    assert len(sequencers[1].mock_calls) == 1
    assert len(sequencers[2].mock_calls) == 1
    assert len(sequencers[3].mock_calls) == 1
    sequencers[0].running = True

    #abort one sequence
    sm.abort(3)
    sequencers[3].running = False
    assert sequencers[3].mock_calls[1]==('abort', (), {})
    assert sm.state=='testing'  #abort can not transition to done state if not all sites are done
    sequencers[2].running = False
    sm.finish(2)
    assert sm.state=='testing' #finish can not transition to done state if not all sites are done
    sequencers[0].running = False
    sequencers[1].running = False
    sm.finish(1)
    assert sm.state=='done' #finish can tranistion to done state if this is the last one

    sm.start(e_travelers)
    assert sm.state == 'testing'
    sm.abort(0)
    assert sm.state == 'done' #abort can transition to done state if thsi is the last one

    #now we are done
    sm.will_unload()
    assert fixture.mock_calls[4] == ('disengage', (), {})
    assert sm.state == 'ok_to_unload'

    sm.abort()
    assert fixture.mock_calls[5] == ('close', (), {})
    assert sm.state == 'done'

    sm.will_unload()
    assert fixture.mock_calls[6] == ('disengage', (), {})
    assert sm.state == 'ok_to_unload'

    sm.dut_removed()
    assert fixture.mock_calls[7] == ('close', (), {})
    assert sm.state == 'idle'