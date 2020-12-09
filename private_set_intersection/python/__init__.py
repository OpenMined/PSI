"""Private Set Intersection protocol based on ECDH and Bloom
Filters.
"""
try:
    # Used in Bazel envs
    from private_set_intersection.python import _openmined_psi as psi
except ImportError:
    # Default package
    import openmined_psi as psi

from wrappers import client, server, proto_server_setup, proto_request, proto_response

__version__ = psi.__version__

__all__ = [
    "client",
    "server",
    "proto_server_setup",
    "proto_request",
    "proto_response",
    "__version__",
]
