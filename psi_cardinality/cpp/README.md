# PSI Cardinality - C++

Private Set Intersection Cardinality protocol based on ECDH and Bloom Filters.

## Build and test


Build all libraries with or without optimizations, or build a specific module

```
# Build everything using the fastbuild optimization configuration
bazel build //psi_cardinality/cpp/...

# With a specific optimization flag '-c opt'
bazel build -c opt //psi_cardinality/cpp/...

# Specific module(s)
bazel build -c opt //psi_cardinality/cpp:psi_cardinality_client
bazel build -c opt //psi_cardinality/cpp:psi_cardinality_server
```

Build and run tests

```
bazel test //psi_cardinality/cpp/...
```

Build and run benchmarks

```
bazel run -c opt //psi_cardinality/cpp:psi_cardinality_benchmark
```
