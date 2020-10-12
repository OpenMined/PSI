# PSI - Python

Private Set Intersection protocol based on ECDH, Bloom Filters, and Golomb Compressed Sets.

## Tests
```
bazel test //private_set_intersection/python:tests --test_output=all
```

## Benchmarks
```
bazel run -c opt //private_set_intersection/python:benchmarks --test_output=all
```
