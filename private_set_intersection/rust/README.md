# PSI - Rust

Private Set Intersection protocol based on ECDH and Bloom Filters.

## Build and test

Build all libraries with or without optimizations, or build a specific module

```
# Build everything using the fastbuild optimization configuration
bazel build //private_set_intersection/rust/...

# With a specific optimization flag '-c opt'
bazel build -c opt //private_set_intersection/rust/...

# Specific module(s)
bazel build -c opt //private_set_intersection/rust:psi_client
bazel build -c opt //private_set_intersection/rust:psi_server
```

Build and run tests

```
bazel test //private_set_intersection/rust/...
```

Build and run benchmarks

```
bazel run -c opt //private_set_intersection/rust:psi_benchmark
```
