#!/bin/sh
set -e

# Rust
bazel test -c opt --cxxopt="-std=c++17"  --test_output=all //private_set_intersection/rust/...
