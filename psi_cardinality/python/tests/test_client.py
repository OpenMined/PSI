import pytest
import sys
import psi_cardinality.python as psi


def test_sanity():
    c = psi.client.Create()
    assert c != None


if __name__ == "__main__":
    sys.exit(pytest.main(["-s", "-v", "-x", __file__]))
