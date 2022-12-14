# PSI - Python

Private Set Intersection protocol based on ECDH, Bloom Filters, and Golomb Compressed Sets.

## Tests

```
bazel test -c opt --test_output=all //private_set_intersection/python:tests
```

## Benchmarks

```
bazel run -c opt --test_output=all //private_set_intersection/python:benchmarks
```
