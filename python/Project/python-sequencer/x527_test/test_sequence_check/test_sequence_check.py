from x527.sequencer.sequence_check import *
from x527.tinyrpc.protocols.jsonrpc import *
import pytest
import os

@pytest.fixture(scope='module')
def sample_file():
    path = os.path.split(__file__)[0]
    sample_file_path = os.path.join(path, 'sample1__1.0.csv')
    return sample_file_path

def test_file_name():
    with pytest.raises(JSONRPCInvalidParamsError) as exc:
        check_csv_sequence("somefile.csv")
    assert exc.value.message.startswith("the sequence file name should be of the format [name]__[version].csv")

def test_error_check(sample_file):
    errors, warnings = check_csv_sequence(sample_file)
    assert len(warnings) == 2
    assert warnings[0].message == "========WARNGING==========on line 10,in test T5: relay NET1 is connected again without being disconnected first. Last time it was connected on line 9"
    assert warnings[1].message == "========WARNING========on line 14,in test T9: disconnect relay NET4 before it's connected"

    assert len(errors) == 7
    assert errors[0].message == '!!!!!ERROR!!!!!!on line 4, tid "wldfu_run_diags" is repeated; KEY=unitStage, VAL=POSTSMT2. Previously this tid appeared on line 3'
    assert errors[1].message == '!!!!!!ERROR!!!!!!on line 6,in test T1: no net is specified for the relay function'
    assert errors[2].message == '!!!!!!ERROR!!!!!!on line 7,in test T2: no net is specified for the supply function'
    assert errors[3].message == '!!!!!!ERROR!!!!!!on line 8,in test T3: no net is specified for the disconnect function'
    assert errors[4].message == '!!!!!ERROR!!!!!!on line 15, tid is empty'

    assert errors[5].message == '!!!!!!ERROR!!!!!!on line 13, relay NET3 is connected but never disconnected'
    assert errors[6].message == '!!!!!!ERROR!!!!!!on line 9, relay NET1 is connected but never disconnected'


