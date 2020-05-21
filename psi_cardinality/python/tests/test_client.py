import pytest
import sys
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


def helper_client_create_request(cnt):
    c = psi.client.Create()
    inputs = ["Element " + str(i) for i in range(cnt)]
    req = c.CreateRequest(inputs)


@pytest.mark.parametrize("cnt", [1, 10, 100, 1000, 10000])
def test_client_create_request(cnt, benchmark):
    benchmark(helper_client_create_request, cnt)


def helper_client_process_response(cnt):
    c = psi.client.Create()
    s = psi.server.CreateWithNewKey()

    fpr = 1.0 / 1000000
    inputs = ["Element " + str(i) for i in range(cnt)]
    req = c.CreateRequest(inputs)

    setup = s.CreateSetupMessage(fpr, len(inputs), inputs)
    request = c.CreateRequest(inputs)
    resp = s.ProcessRequest(request)
    intersection = c.ProcessResponse(setup, resp)


@pytest.mark.parametrize("cnt", [1, 10, 100, 1000, 10000])
def test_client_process_response(cnt, benchmark):
    benchmark(helper_client_process_response, cnt)


if __name__ == "__main__":
    sys.exit(pytest.main(["-s", "-v", "-x", __file__]))
