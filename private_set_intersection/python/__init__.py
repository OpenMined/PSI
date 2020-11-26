"""Private Set Intersection protocol based on ECDH and Bloom
Filters.
"""
try:
    from private_set_intersection.python import private_set_intersection_ext
except ImportError:
    import private_set_intersection_ext

client = private_set_intersection_ext.client
server = private_set_intersection_ext.server
__version__ = private_set_intersection_ext.__version__

__all__ = ["client", "server", "__version__"]
