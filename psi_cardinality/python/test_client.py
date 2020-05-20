import pytest
import sys
import psi_cardinality as psi
from psi_cardinality.python import psi_cardinality


def test_sanity():
    c = psi.client.Create()
    assert c != None


if __name__ == "__main__":
    sys.exit(pytest.main(["-s", "-v", "-x", __file__]))
