"""Private Set Intersection protocol based on ECDH and Bloom
Filters.
"""
from typing import List

from private_set_intersection.python import _openmined_psi as psi
from private_set_intersection.proto.psi_python_proto_pb.private_set_intersection.proto.psi_pb2 import (
    ServerSetup,
    Request,
    Response,
)
from enum import Enum

__version__ = psi.__version__


class DataStructure(Enum):
    RAW = psi.data_structure.Raw
    GCS = psi.data_structure.GCS
    BLOOM_FILTER = psi.data_structure.BloomFilter


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

    def CreateRequest(self, data: List[str]) -> Request:
        """Create a request protobuf to be serialized and sent to the server.
        Args:
            data: client items.
        Returns:
            A Protobuffer with the request.
        """
        interm_req = self.data.CreateRequest(data).save()

        req = Request()
        req.ParseFromString(interm_req)

        return req

    def GetIntersection(self, server_setup: ServerSetup, server_response: Response) -> List[int]:
        """Process the server's response and return the intersection of the client and server inputs.
        Args:
            server_setup: A protobuffer with the setup message.
            server_response: A protobuffer with server's response.
        Returns:
            A list of indices in clients set.
        """
        interm_server_setup = psi.cpp_proto_server_setup.Load(server_setup.SerializeToString())
        interm_response = psi.cpp_proto_response.Load(server_response.SerializeToString())

        return self.data.GetIntersection(interm_server_setup, interm_response)

    def GetIntersectionSize(self, server_setup: ServerSetup, server_response: Response) -> int:
        """Process the server's response and return the size of the intersection.
        Args:
            server_setup: A protobuffer with the setup message.
            server_response: A protobuffer with server's response.
        Returns:
            The intersection size.
        """
        interm_server_setup = psi.cpp_proto_server_setup.Load(server_setup.SerializeToString())
        interm_response = psi.cpp_proto_response.Load(server_response.SerializeToString())

        return self.data.GetIntersectionSize(interm_server_setup, interm_response)

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
        self, fpr: float, num_client_inputs: int, inputs: List[str], ds=DataStructure.GCS
    ) -> ServerSetup:
        """Create a setup message from the server's dataset to be sent to the client.
        Args:
            fpr: the probability that any query of size `num_client_inputs` will result in a false positive.
            num_client_inputs: Client set size.
            inputs: Server items.
            ds: The underlying data structure to use. Defaults to GCS.
        Returns:
            A Protobuf with the setup message.
        """
        interm_msg = self.data.CreateSetupMessage(fpr, num_client_inputs, inputs, ds.value).save()
        msg = ServerSetup()
        msg.ParseFromString(interm_msg)
        return msg

    def ProcessRequest(self, client_request: Request) -> Response:
        """Process a client query and returns the corresponding server response to be sent to the client.
        Args:
            client_request: A Protobuf containing the client request
        Returns:
            A Protobuf with the server response.
        """
        interm_req = psi.cpp_proto_request.Load(client_request.SerializeToString())
        interm_resp = self.data.ProcessRequest(interm_req).save()

        resp = Response()
        resp.ParseFromString(interm_resp)

        return resp

    def GetPrivateKeyBytes(self) -> bytes:
        """Return this instance's private key. This key should only be used to create other server instances. DO NOT SEND THIS KEY TO ANY OTHER PARTY!
        Returns:
            Bytes containing the key.
        """
        return self.data.GetPrivateKeyBytes()


__all__ = [
    "DataStructure",
    "client",
    "server",
    "ServerSetup",
    "Response",
    "Request",
    "__version__",
]
