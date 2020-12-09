"""Private Set Intersection protocol based on ECDH and Bloom
Filters.
"""
from typing import List

try:
    # Used in Bazel envs
    from private_set_intersection.python import _openmined_psi as psi
except ImportError:
    # Default package
    import _openmined_psi as psi

__version__ = psi.__version__


class proto_server_setup:
    def __init__(self):
        """Constructor method for the proto_server_setup object.
        Returns:
            proto_server_setup object.
        """
        self.data = psi.cpp_proto_server_setup()

    def bits(self) -> bytes:
        return self.data.bits()

    def set_bits(self, *args, **kwargs):
        return self.data.set_bits(args, kwargs)

    def clear_bits(self):
        return self.data.clear_bits()

    def save(self) -> bytes:
        return self.data.save()

    def load(self, data: bytes):
        return self.data.load(data)

    @classmethod
    def Load(cls, data: bytes) -> "proto_server_setup":
        return psi.cpp_proto_server_setup.Load(data)


class proto_request:
    def __init__(self):
        """Constructor method for the proto_request object.
        Returns:
            proto_request object.
        """
        self.data = psi.cpp_proto_request()

    def encrypted_elements_size(self) -> int:
        return self.data.encrypted_elements_size()

    def encrypted_elements(self, idx: int = None):
        if idx is not None:
            return self.data.encrypted_elements(idx)

        return self.data.encrypted_elements()

    def add_encrypted_elements(self, el: bytes):
        return self.data.add_encrypted_elements(el)

    def clear_encrypted_elements(self):
        return self.data.clear_encrypted_elements()

    def reveal_intersection(self) -> bool:
        return self.data.reveal_intersection()

    def clear_reveal_intersection(self) -> bool:
        return self.data.clear_reveal_intersection()

    def set_reveal_intersection(self, state: bool) -> bool:
        return self.data.set_reveal_intersection(state)

    def save(self) -> bytes:
        return self.data.save()

    def load(self, data: bytes):
        return self.data.load(data)

    @classmethod
    def Load(cls, data: bytes) -> "proto_request":
        return psi.cpp_proto_request.Load(data)


class proto_response:
    def __init__(self):
        """Constructor method for the proto_response object.
        Returns:
            proto_response object.
        """
        self.data = psi.cpp_proto_response()

    def encrypted_elements_size(self) -> int:
        return self.data.encrypted_elements_size()

    def encrypted_elements(self, idx: int = None):
        if idx is not None:
            return self.data.encrypted_elements(idx)

        return self.data.encrypted_elements()

    def add_encrypted_elements(self, el: bytes):
        return self.data.add_encrypted_elements(el)

    def clear_encrypted_elements(self):
        return self.data.clear_encrypted_elements()

    def save(self) -> bytes:
        return self.data.save()

    def load(self, data: bytes):
        return self.data.load(data)

    @classmethod
    def Load(cls, data: bytes) -> "proto_request":
        return psi.cpp_proto_response.Load(data)


class client:
    def __init__(self, reveal_intersection: bool, key_bytes: bytes = None):
        """Constructor method for the client object.
        Args:
            reveal_intersection: indicates whether the client wants to learn the elements in the intersection or only its size.
            key_bytes: existing encryption key. If missing, a new key will be generated.
        Returns:
            client object.
        """
        if key_bytes:
            self.data = psi.cpp_client.CreateFromKey(key_bytes, reveal_intersection)
        else:
            self.data = psi.cpp_client.CreateWithNewKey(reveal_intersection)

    @classmethod
    def CreateWithNewKey(cls, reveal_intersection: bool):
        return psi.cpp_client.CreateWithNewKey(reveal_intersection)

    @classmethod
    def CreateFromKey(cls, key_bytes: bytes, reveal_intersection: bool):
        return psi.cpp_client.CreateFromKey(key_bytes, reveal_intersection)

    def CreateRequest(self, data: List[str]) -> proto_request:
        return self.data.CreateRequest(data)

    def GetIntersection(
        self, server_setup: proto_server_setup, server_response: proto_response
    ) -> List[int]:
        return self.data.GetIntersection(server_setup, server_response)

    def GetIntersectionSize(
        self, server_setup: proto_server_setup, server_response: proto_response
    ) -> int:
        return self.data.GetIntersectionSize(server_setup, server_response)

    def GetPrivateKeyBytes(self) -> bytes:
        return self.data.GetPrivateKeyBytes()


class server:
    def __init__(self, reveal_intersection: bool, key_bytes: bytes = None):
        """Constructor method for the server object.
        Args:
            reveal_intersection: indicates whether the server supports to return the elements in the intersection or only its size.
            key_bytes: existing encryption key.
        Returns:
            server object.
        """
        if key_bytes:
            self.data = psi.cpp_server.CreateFromKey(key_bytes, reveal_intersection)
        else:
            self.data = psi.cpp_server.CreateWithNewKey(reveal_intersection)

    @classmethod
    def CreateWithNewKey(cls, reveal_intersection: bool):
        return psi.cpp_server.CreateWithNewKey(reveal_intersection)

    @classmethod
    def CreateFromKey(cls, key_bytes: bytes, reveal_intersection: bool):
        return psi.cpp_server.CreateFromKey(key_bytes, reveal_intersection)

    def CreateSetupMessage(
        self, fpr: float, num_client_inputs: int, inputs: List[str]
    ) -> proto_server_setup:
        return self.data.CreateSetupMessage(fpr, num_client_inputs, inputs)

    def ProcessRequest(self, client_request: proto_request) -> proto_response:
        return self.data.ProcessRequest(client_request)

    def GetPrivateKeyBytes(self) -> bytes:
        return self.data.GetPrivateKeyBytes()


__all__ = [
    "client",
    "server",
    "proto_server_setup",
    "proto_request",
    "proto_response",
    "__version__",
]
