__author__ = 'wei'
from x527.tinyrpc.protocols.terpc import *
import json
import pytest

@pytest.fixture
def terpc():
    return TERPCProtocol()

def test_request(terpc):
    req = terpc.create_request('my_func', [1, '2'])
    assert isinstance(req.unique_id, str)
    assert req.function == 'my_func'
    assert req.params == [1, '2']
    other_req = terpc.parse_request(req.serialize())
    assert req.unique_id == other_req.unique_id
    assert req.function == other_req.function
    assert req.params == other_req.params


