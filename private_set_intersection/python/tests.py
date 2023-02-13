import pytest
import sys
import re

import private_set_intersection.python as psi

client_key = bytes(range(32))
server_key = bytes(range(1, 33))
fpr = 0.001
num_client_inputs = 10
num_server_inputs = 100
client_items = ["Element " + str(i) for i in range(num_client_inputs)]
server_items = ["Element " + str(2 * i) for i in range(num_server_inputs)]


def test_version():
    version = psi.__version__
    assert re.match(r"[0-9]+[.][0-9]+[.][0-9]+(-[A-Za-z0-9]+)?", version)


@pytest.mark.parametrize("reveal_intersection", [False, True])
@pytest.mark.parametrize(
    "ds", [psi.DataStructure.RAW, psi.DataStructure.GCS, psi.DataStructure.BLOOM_FILTER]
)
def test_integration(ds, reveal_intersection):
    c = psi.client.CreateFromKey(client_key, reveal_intersection)
    s = psi.server.CreateFromKey(server_key, reveal_intersection)

    setup = psi.ServerSetup()
    setup.ParseFromString(
        s.CreateSetupMessage(fpr, len(client_items), server_items, ds).SerializeToString()
    )

    request = psi.Request()
    request.ParseFromString(c.CreateRequest(client_items).SerializeToString())

    response = psi.Response()
    response.ParseFromString(s.ProcessRequest(request).SerializeToString())

    if reveal_intersection:
        intersection = c.GetIntersection(setup, response)
        iset = set(intersection)
        for idx in range(len(client_items)):
            if idx % 2 == 0:
                assert idx in iset
            else:
                assert idx not in iset
    else:
        intersection = c.GetIntersectionSize(setup, response)
        assert intersection >= (len(client_items) / 2.0)
        assert intersection <= (1.1 * len(client_items) / 2.0)


if __name__ == "__main__":
    sys.exit(pytest.main(["-s", "-v", "-x", __file__]))


@pytest.mark.parametrize("reveal_intersection", [False, True])
def test_server_sanity(reveal_intersection):
    s = psi.server.CreateWithNewKey(reveal_intersection)
    assert s != None

    key = s.GetPrivateKeyBytes()
    assert key != None

    other = psi.server.CreateFromKey(key, reveal_intersection)
    newkey = other.GetPrivateKeyBytes()

    assert key == newkey


@pytest.mark.parametrize("reveal_intersection", [False, True])
def test_client_sanity(reveal_intersection):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    assert c != None

    key = c.GetPrivateKeyBytes()
    assert key != None

    other = psi.client.CreateFromKey(key, reveal_intersection)
    newkey = other.GetPrivateKeyBytes()

    assert key == newkey


@pytest.mark.parametrize(
    "ds", [psi.DataStructure.RAW, psi.DataStructure.GCS, psi.DataStructure.BLOOM_FILTER]
)
@pytest.mark.parametrize("reveal_intersection", [False, True])
def test_empty_intersection(ds, reveal_intersection):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    s = psi.server.CreateWithNewKey(reveal_intersection)

    server_items2 = ["Other " + str(2 * i) for i in range(100)]

    setup = s.CreateSetupMessage(fpr, len(client_items), server_items2, ds)
    request = c.CreateRequest(client_items)
    resp = s.ProcessRequest(request)

    if reveal_intersection:
        intersection = c.GetIntersection(setup, resp)
        assert len(intersection) == 0
    else:
        intersection = c.GetIntersectionSize(setup, resp)
        assert intersection == 0
