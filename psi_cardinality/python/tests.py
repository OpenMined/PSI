import pytest
import sys
import re
import psi_cardinality.python as psi


def test_sanity():
    c = psi.client.Create()
    assert c != None


def test_client_server():
    c = psi.client.Create()
    s = psi.server.CreateWithNewKey()

    client_items = ["Element " + str(i) for i in range(1000)]
    server_items = ["Element " + str(2 * i) for i in range(10000)]

    setup = s.CreateSetupMessage(0.01, len(client_items), server_items)
    request = c.CreateRequest(client_items)
    resp = s.ProcessRequest(request)
    intersection = c.ProcessResponse(setup, resp)

    assert intersection >= (len(client_items) / 2.0)
    assert intersection <= (1.1 * len(client_items) / 2.0)


def test_version():
    version = psi.__version__
    assert re.match(r"[0-9]+[.][0-9]+[.][0-9]+(-[A-Za-z0-9]+)?", version)


def test_sanity():
    s = psi.server.CreateWithNewKey()
    assert s != None

    key = s.GetPrivateKeyBytes()
    assert key != None

    other = psi.server.CreateFromKey(key)
    newkey = other.GetPrivateKeyBytes()

    assert key == newkey


def test_server_client():
    c = psi.client.Create()
    s = psi.server.CreateWithNewKey()

    client_items = ["Element " + str(i) for i in range(1000)]
    server_items = ["Element " + str(2 * i) for i in range(10000)]

    setup = s.CreateSetupMessage(0.01, len(client_items), server_items)
    request = c.CreateRequest(client_items)
    resp = s.ProcessRequest(request)
    intersection = c.ProcessResponse(setup, resp)

    assert intersection >= (len(client_items) / 2.0)
    assert intersection <= (1.1 * len(client_items) / 2.0)


if __name__ == "__main__":
    sys.exit(pytest.main(["-s", "-v", "-x", __file__]))
