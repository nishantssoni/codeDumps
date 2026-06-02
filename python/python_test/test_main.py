from main import add, divide
import pytest


def test_add():
    assert add(2,3) == 5, "2 + 3 shoud be 5"
    assert add(-1,1) == 0, "-1 + -1 should be 0"
    assert add(0,0) == 0, "0 + 0 should be 0"

def test_divide():
    with pytest.raises(ValueError, match="cannot divide by zero"):
        divide(10,0)