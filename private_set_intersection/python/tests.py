import pytest
import sys
import re

try:
    # Used in Bazel envs
    import private_set_intersection.python as psi
except ImportError:
    # Default package
    import openmined_psi as psi


@pytest.mark.parametrize("reveal_intersection", [False, True])
def test_sanity(reveal_intersection):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    assert c != None


@pytest.mark.parametrize("reveal_intersection", [False, True])
def test_client_server(reveal_intersection):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    s = psi.server.CreateWithNewKey(reveal_intersection)

    client_items = ["Element " + str(i) for i in range(1000)]
    server_items = ["Element " + str(2 * i) for i in range(10000)]

    fpr = 1.0 / (1000000000)
    setup = s.CreateSetupMessage(fpr, len(client_items), server_items)
    request = c.CreateRequest(client_items)
    resp = s.ProcessRequest(request)

    if reveal_intersection:
        intersection = c.GetIntersection(setup, resp)
        iset = set(intersection)
        for idx in range(len(client_items)):
            if idx % 2 == 0:
                assert idx in iset
            else:
                assert idx not in iset
    else:
        intersection = c.GetIntersectionSize(setup, resp)
        assert intersection >= (len(client_items) / 2.0)
        assert intersection <= (1.1 * len(client_items) / 2.0)


def test_version():
    version = psi.__version__
    assert re.match(r"[0-9]+[.][0-9]+[.][0-9]+(-[A-Za-z0-9]+)?", version)


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


@pytest.mark.parametrize("reveal_intersection", [False, True])
def test_server_client(reveal_intersection):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    s = psi.server.CreateWithNewKey(reveal_intersection)

    client_items = ["Element " + str(i) for i in range(1000)]
    server_items = ["Element " + str(2 * i) for i in range(10000)]

    fpr = 1.0 / (1000000000)
    setup = s.CreateSetupMessage(fpr, len(client_items), server_items)
    request = c.CreateRequest(client_items)
    resp = s.ProcessRequest(request)

    if reveal_intersection:
        intersection = c.GetIntersection(setup, resp)
        iset = set(intersection)
        for idx in range(len(client_items)):
            if idx % 2 == 0:
                assert idx in iset
            else:
                assert idx not in iset
    else:
        intersection = c.GetIntersectionSize(setup, resp)
        assert intersection >= (len(client_items) / 2.0)
        assert intersection <= (1.1 * len(client_items) / 2.0)


@pytest.mark.parametrize("reveal_intersection", [False, True])
def test_serialization_setup_msg(reveal_intersection):
    s = psi.server.CreateWithNewKey(reveal_intersection)

    server_items = ["Element " + str(2 * i) for i in range(10000)]

    fpr = 1.0 / (1000000000)
    setup = s.CreateSetupMessage(fpr, 1000, server_items)

    buff = setup.save()
    recreated = psi.proto_server_setup.Load(buff)
    assert isinstance(buff, bytes)
    assert setup.bits() == recreated.bits()

    recreated_local = psi.proto_server_setup()
    recreated_local.load(buff)
    assert setup.bits() == recreated_local.bits()


@pytest.mark.parametrize("reveal_intersection", [False, True])
def test_serialization_request(reveal_intersection):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    client_items = ["Element " + str(i) for i in range(1000)]
    request = c.CreateRequest(client_items)

    buff = request.save()
    recreated = psi.proto_request.Load(buff)
    assert isinstance(buff, bytes)
    assert request.encrypted_elements_size() == recreated.encrypted_elements_size()
    assert request.encrypted_elements() == recreated.encrypted_elements()
    assert request.reveal_intersection() == recreated.reveal_intersection()

    recreated_local = psi.proto_request()
    recreated_local.load(buff)

    assert request.encrypted_elements_size() == recreated_local.encrypted_elements_size()
    assert request.encrypted_elements() == recreated_local.encrypted_elements()
    assert request.reveal_intersection() == recreated_local.reveal_intersection()


@pytest.mark.parametrize("reveal_intersection", [False, True])
def test_serialization_response(reveal_intersection):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    s = psi.server.CreateWithNewKey(reveal_intersection)

    client_items = ["Element " + str(i) for i in range(1000)]
    server_items = ["Element " + str(2 * i) for i in range(10000)]

    fpr = 1.0 / (1000000000)
    setup = s.CreateSetupMessage(fpr, len(client_items), server_items)
    req = c.CreateRequest(client_items)
    resp = s.ProcessRequest(req)

    buff = resp.save()
    recreated = psi.proto_response.Load(buff)
    assert isinstance(buff, bytes)
    assert resp.encrypted_elements_size() == recreated.encrypted_elements_size()
    assert resp.encrypted_elements() == recreated.encrypted_elements()

    recreated_local = psi.proto_response()
    recreated_local.load(buff)

    assert resp.encrypted_elements_size() == recreated_local.encrypted_elements_size()
    assert resp.encrypted_elements() == recreated_local.encrypted_elements()


@pytest.mark.parametrize("reveal_intersection", [False, True])
def test_empty_intersection(reveal_intersection):
    c = psi.client.CreateWithNewKey(reveal_intersection)
    s = psi.server.CreateWithNewKey(reveal_intersection)

    client_items = ["Element " + str(i) for i in range(1000)]
    server_items = ["Other " + str(2 * i) for i in range(10000)]

    fpr = 1.0 / (1000000000)
    setup = s.CreateSetupMessage(fpr, len(client_items), server_items)
    request = c.CreateRequest(client_items)
    resp = s.ProcessRequest(request)

    if reveal_intersection:
        intersection = c.GetIntersection(setup, resp)
        assert len(intersection) == 0
    else:
        intersection = c.GetIntersectionSize(setup, resp)
        assert intersection == 0


if __name__ == "__main__":
    sys.exit(pytest.main(["-s", "-v", "-x", __file__]))
