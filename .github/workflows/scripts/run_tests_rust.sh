#!/bin/sh
set -e

# Rust
bazel test --test_output=all //private_set_intersection/rust/...
