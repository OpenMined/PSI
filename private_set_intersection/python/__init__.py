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
        """Constructor method for the server_setup Protobuf object.
        Returns:
            proto_server_setup object.
        """
        self.data = psi.cpp_proto_server_setup()

    def bits(self) -> bytes:
        """Return the serialized setup message"""
        return self.data.bits()

    def set_bits(self, *args, **kwargs):
        """Set the serialized setup message"""
        return self.data.set_bits(args, kwargs)

    def clear_bits(self):
        """Clear the serialized setup message"""
        return self.data.clear_bits()

    def save(self) -> bytes:
        """Save the protobuffer to wire format"""
        return self.data.save()

    def load(self, data: bytes):
        """Load the protobuffer from wire format"""
        return self.data.load(data)

    @classmethod
    def Load(cls, data: bytes) -> "proto_server_setup":
        """Load the protobuffer from wire format"""
        return psi.cpp_proto_server_setup.Load(data)


class proto_request:
    def __init__(self):
        """Constructor method for the request Protobuf object.
        Returns:
            proto_request object.
        """
        self.data = psi.cpp_proto_request()

    def encrypted_elements_size(self) -> int:
        """Count of encrypted items in the request."""
        return self.data.encrypted_elements_size()

    def encrypted_elements(self, idx: int = None):
        """Encrypted items in the request
        Args:
            idx: If provided, returns the item at the idx position. Otherwise, returns all elements.
        """
        if idx is not None:
            return self.data.encrypted_elements(idx)

        return self.data.encrypted_elements()

    def add_encrypted_elements(self, el: bytes):
        """Add an item to the encrypted items in the request.
        Args:
            el: bytes buffer of the new item.
        """
        return self.data.add_encrypted_elements(el)

    def clear_encrypted_elements(self):
        """Clear the encrypted items in the request."""
        return self.data.clear_encrypted_elements()

    def reveal_intersection(self) -> bool:
        """Get reveal_intersection flag value"""
        return self.data.reveal_intersection()

    def clear_reveal_intersection(self) -> bool:
        """Reset reveal_intersection flag value"""
        return self.data.clear_reveal_intersection()

    def set_reveal_intersection(self, state: bool) -> bool:
        """Set reveal_intersection flag value"""
        return self.data.set_reveal_intersection(state)

    def save(self) -> bytes:
        """Save the protobuffer to wire format"""
        return self.data.save()

    def load(self, data: bytes):
        """Load the protobuffer from wire format"""
        return self.data.load(data)

    @classmethod
    def Load(cls, data: bytes) -> "proto_request":
        """Load the protobuffer from wire format"""
        return psi.cpp_proto_request.Load(data)


class proto_response:
    def __init__(self):
        """Constructor method for the response Protobuf object.
        Returns:
            proto_response object.
        """
        self.data = psi.cpp_proto_response()

    def encrypted_elements_size(self) -> int:
        """Count of encrypted items in the response."""
        return self.data.encrypted_elements_size()

    def encrypted_elements(self, idx: int = None):
        """Encrypted items in the response.
        Args:
            idx: If provided, returns the item at the idx position. Otherwise, returns all elements.
        """
        if idx is not None:
            return self.data.encrypted_elements(idx)

        return self.data.encrypted_elements()

    def add_encrypted_elements(self, el: bytes):
        """Add an item to the encrypted items in the response.
        Args:
            el: bytes buffer of the new item.
        """
        return self.data.add_encrypted_elements(el)

    def clear_encrypted_elements(self):
        """Clear the encrypted items in the request."""
        return self.data.clear_encrypted_elements()

    def save(self) -> bytes:
        """Save the protobuffer to wire format"""
        return self.data.save()

    def load(self, data: bytes):
        """Load the protobuffer from wire format"""
        return self.data.load(data)

    @classmethod
    def Load(cls, data: bytes) -> "proto_request":
        """Load the protobuffer from wire format"""
        return psi.cpp_proto_response.Load(data)


class client:
    def __init__(self, data: psi.cpp_client):
        """Constructor method for the client object.
        Args:
            data: cpp_client object.
        Returns:
            client object.
        """
        self.data = data

    @classmethod
    def CreateWithNewKey(cls, reveal_intersection: bool):
        """Constructor method for the client object.
        Args:
            reveal_intersection: indicates whether the client wants to learn the elements in the intersection or only its size.
        Returns:
            client object.
        """
        return cls(psi.cpp_client.CreateWithNewKey(reveal_intersection))

    @classmethod
    def CreateFromKey(cls, key_bytes: bytes, reveal_intersection: bool):
        """Constructor method for the client object.
        Args:
            reveal_intersection: indicates whether the client wants to learn the elements in the intersection or only its size.
            key_bytes: existing encryption key.
        Returns:
            client object.
        """
        return cls(psi.cpp_client.CreateFromKey(key_bytes, reveal_intersection))

    def CreateRequest(self, data: List[str]) -> proto_request:
        """Create a request protobuf to be serialized and sent to the server.
        Args:
            data: client items.
        Returns:
            A Protobuffer with the request.
        """
        return self.data.CreateRequest(data)

    def GetIntersection(
        self, server_setup: proto_server_setup, server_response: proto_response
    ) -> List[int]:
        """Process the server's response and return the intersection of the client and server inputs.
        Args:
            server_setup: A protobuffer with the setup message.
            server_response: A protobuffer with server's response.
        Returns:
            A list of indices in clients set.
        """
        return self.data.GetIntersection(server_setup, server_response)

    def GetIntersectionSize(
        self, server_setup: proto_server_setup, server_response: proto_response
    ) -> int:
        """Process the server's response and return the size of the intersection.
        Args:
            server_setup: A protobuffer with the setup message.
            server_response: A protobuffer with server's response.
        Returns:
            The intersection size.
        """
        return self.data.GetIntersectionSize(server_setup, server_response)

    def GetPrivateKeyBytes(self) -> bytes:
        """Returns this instance's private key. This key should only be used to create other client instances. DO NOT SEND THIS KEY TO ANY OTHER PARTY!
        Returns:
            Bytes containing the key.
        """
        return self.data.GetPrivateKeyBytes()


class server:
    def __init__(self, data: psi.cpp_server):
        """Constructor method for the server object.
        Args:
            data: cpp_server object.
        Returns:
            server object.
        """
        self.data = data

    @classmethod
    def CreateWithNewKey(cls, reveal_intersection: bool):
        """Constructor method for the server object.
        Args:
            reveal_intersection: indicates whether the server supports to return the elements in the intersection or only its size.
        Returns:
            server object.
        """
        return cls(psi.cpp_server.CreateWithNewKey(reveal_intersection))

    @classmethod
    def CreateFromKey(cls, key_bytes: bytes, reveal_intersection: bool):
        """Constructor method for the server object.
        Args:
            reveal_intersection: indicates whether the server supports to return the elements in the intersection or only its size.
            key_bytes: existing encryption key.
        Returns:
        Returns:
            server object.
        """
        return cls(psi.cpp_server.CreateFromKey(key_bytes, reveal_intersection))

    def CreateSetupMessage(
        self, fpr: float, num_client_inputs: int, inputs: List[str]
    ) -> proto_server_setup:
        """Create a setup message from the server's dataset to be sent to the client.
        Args:
            fpr: the probability that any query of size `num_client_inputs` will result in a false positive.
            num_client_inputs: Client set size.
            inputs: Server items.
        Returns:
            A Protobuf with the setup message.
        """
        return self.data.CreateSetupMessage(fpr, num_client_inputs, inputs)

    def ProcessRequest(self, client_request: proto_request) -> proto_response:
        """Process a client query and returns the corresponding server response to be sent to the client.
        Args:
            client_request: A Protobuf containing the client request
        Returns:
            A Protobuf with the server response.
        """
        return self.data.ProcessRequest(client_request)

    def GetPrivateKeyBytes(self) -> bytes:
        """Return this instance's private key. This key should only be used to create other server instances. DO NOT SEND THIS KEY TO ANY OTHER PARTY!
        Returns:
            Bytes containing the key.
        """
        return self.data.GetPrivateKeyBytes()


__all__ = [
    "client",
    "server",
    "proto_server_setup",
    "proto_request",
    "proto_response",
    "__version__",
]
