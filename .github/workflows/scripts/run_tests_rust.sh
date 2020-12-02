#!/bin/sh
set -e

# Rust
bazel test --test_output=all --host_copt=-DGRPC_BAZEL_BUILD //private_set_intersection/rust/...
