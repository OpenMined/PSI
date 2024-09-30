import sys

import pytest

import private_set_intersection.python as psi


def helper_client_create_request(cnt, reveal_intersection):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    inputs = ["Element " + str(i) for i in range(cnt)]
    _req = c.CreateRequest(inputs)


@pytest.mark.parametrize("cnt", [1, 10, 100, 1000, 10000])
@pytest.mark.parametrize("reveal_intersection", [False, True])
def test_client_create_request(cnt, reveal_intersection, benchmark):
    benchmark(helper_client_create_request, cnt, reveal_intersection)


def helper_client_process_response(cnt, reveal_intersection, ds):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    s = psi.server.CreateWithNewKey(reveal_intersection)

    fpr = 1.0 / 1000000
    inputs = ["Element " + str(i) for i in range(cnt)]

    setup = s.CreateSetupMessage(fpr, len(inputs), inputs, ds)
    request = c.CreateRequest(inputs)
    resp = s.ProcessRequest(request)
    if reveal_intersection:
        _intersection = c.GetIntersection(setup, resp)
    else:
        _intersection = c.GetIntersectionSize(setup, resp)


@pytest.mark.parametrize("cnt", [1, 10, 100, 1000, 10000])
@pytest.mark.parametrize("reveal_intersection", [False, True])
@pytest.mark.parametrize("ds", [psi.DataStructure.GCS, psi.DataStructure.BLOOM_FILTER])
def test_client_process_response(cnt, reveal_intersection, ds, benchmark):
    benchmark(helper_client_process_response, cnt, reveal_intersection, ds)


def helper_server_setup(cnt, fpr, reveal_intersection, ds):
    s = psi.server.CreateWithNewKey(reveal_intersection)
    items = ["Element " + str(2 * i) for i in range(cnt)]
    _setup = s.CreateSetupMessage(fpr, 10000, items, ds)


@pytest.mark.parametrize("cnt", [1, 10, 100, 1000, 10000])
@pytest.mark.parametrize("fpr", [0.001, 0.000001])
@pytest.mark.parametrize("reveal_intersection", [False, True])
@pytest.mark.parametrize("ds", [psi.DataStructure.GCS, psi.DataStructure.BLOOM_FILTER])
def test_server_setup(cnt, fpr, reveal_intersection, ds, benchmark):
    benchmark(helper_server_setup, cnt, fpr, reveal_intersection, ds)


def helper_server_process_request(cnt, reveal_intersection, ds):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    s = psi.server.CreateWithNewKey(reveal_intersection)

    fpr = 1.0 / 1000000
    inputs = ["Element " + str(i) for i in range(cnt)]

    _setup = s.CreateSetupMessage(fpr, len(inputs), inputs, ds)
    request = c.CreateRequest(inputs)
    _resp = s.ProcessRequest(request)


@pytest.mark.parametrize("cnt", [1, 10, 100, 1000, 10000])
@pytest.mark.parametrize("reveal_intersection", [False, True])
@pytest.mark.parametrize("ds", [psi.DataStructure.GCS, psi.DataStructure.BLOOM_FILTER])
def test_server_process_request(cnt, reveal_intersection, ds, benchmark):
    benchmark(helper_server_process_request, cnt, reveal_intersection, ds)


if __name__ == "__main__":
    sys.exit(pytest.main(["-s", "-v", "-x", __file__]))
