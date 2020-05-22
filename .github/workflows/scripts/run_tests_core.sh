#!/bin/sh
set -e

# C++
bazel test --test_output=all //psi_cardinality/cpp/...

# C
bazel test --test_output=all //psi_cardinality/c/...
