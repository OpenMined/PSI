"""Private Set Intersection protocol based on ECDH and Bloom
Filters.
"""
try:
    # Used in Bazel envs
    from private_set_intersection.python import _openmined_psi as psi
except ImportError:
    # Default package
    import openmined_psi as psi

client = psi.client
server = psi.server

proto_server_setup = psi.proto_server_setup
proto_request = psi.proto_request
proto_response = psi.proto_response

__version__ = psi.__version__

__all__ = [
    "client",
    "server",
    "proto_server_setup",
    "proto_request",
    "proto_response",
    "__version__",
]
