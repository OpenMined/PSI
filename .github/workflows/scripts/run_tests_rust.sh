#!/bin/sh
set -e

# Rust
bazel test -c opt --test_output=all //private_set_intersection/rust/...
