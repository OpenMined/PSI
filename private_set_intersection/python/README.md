# PSI - Python

Private Set Intersection protocol based on ECDH, Bloom Filters, and Golomb Compressed Sets.

## Tests

(\*nix --cxxopt="-std=c++17", windows --cxxopt="-std:c++17")

```
bazel test -c opt --cxxopt="-std=c++17" --test_output=all //private_set_intersection/python:tests
```

## Benchmarks

```
bazel run -c opt --cxxopt="-std=c++17" --test_output=all //private_set_intersection/python:benchmarks
```
