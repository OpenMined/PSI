import pytest
import sys
import psi_cardinality.python as psi


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


def helper_server_setup(cnt, fpr):
    s = psi.server.CreateWithNewKey()
    items = ["Element " + str(2 * i) for i in range(cnt)]
    setup = s.CreateSetupMessage(fpr, 10000, items)


@pytest.mark.parametrize("cnt", [1, 10, 100, 1000, 10000])
@pytest.mark.parametrize("fpr", [0.001, 0.000001])
def test_server_setup(cnt, fpr, benchmark):
    benchmark(helper_server_setup, cnt, fpr)


def helper_server_process_request(cnt):
    c = psi.client.Create()
    s = psi.server.CreateWithNewKey()

    fpr = 1.0 / 1000000
    inputs = ["Element " + str(i) for i in range(cnt)]
    req = c.CreateRequest(inputs)

    setup = s.CreateSetupMessage(fpr, len(inputs), inputs)
    request = c.CreateRequest(inputs)
    resp = s.ProcessRequest(request)


@pytest.mark.parametrize("cnt", [1, 10, 100, 1000, 10000])
def test_server_process_request(cnt, benchmark):
    benchmark(helper_server_process_request, cnt)


if __name__ == "__main__":
    sys.exit(pytest.main(["-s", "-v", "-x", __file__]))
