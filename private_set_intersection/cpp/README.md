# PSI - C++

Private Set Intersection protocol based on ECDH, Bloom Filters, and Golomb Compressed Sets.

## Build and test


Build all libraries with or without optimizations, or build a specific module

```
# Build everything using the fastbuild optimization configuration

bazel build //private_set_intersection/cpp/...

# With a specific optimization flag '-c opt'
bazel build -c opt //private_set_intersection/cpp/...

# Specific module(s)
bazel build -c opt //private_set_intersection/cpp:psi_client
bazel build -c opt //private_set_intersection/cpp:psi_server
```

Build and run tests

```
bazel test //private_set_intersection/cpp/...
```

Build and run benchmarks

```
bazel run -c opt //private_set_intersection/cpp:psi_benchmark
```
