# PSI - Rust

Private Set Intersection protocol based on ECDH, Bloom Filters, and Golomb Compressed Sets.

This crate provides Rust bindings to the core C++ PSI library, by wrapping the C interface.

## Build and test

Build libraries and documentation with or without optimizations:

```
# Build everything using the fastbuild optimization configuration

bazel build //private_set_intersection/rust/...

# With a specific optimization flag '-c opt'
bazel build -c opt //private_set_intersection/rust/...
```

Build and run tests:

```
bazel test //private_set_intersection/rust/...
```

Run the benchmark:

```
bazel run -c opt //private_set_intersection/rust:bench
```

## Developing

Simply add crates to the `MODULE.bazel` section and re-run your bazel commands.
