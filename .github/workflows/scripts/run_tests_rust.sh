#!/bin/sh
set -e

bazel clean --expunge

# Rust
bazel test --test_output=all //private_set_intersection/rust/...
