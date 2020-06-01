import pytest
import sys
import private_set_intersection.python as psi


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
