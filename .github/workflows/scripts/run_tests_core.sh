#!/bin/sh
set -e

# C++
bazel test --test_output=all --host_copt=-DGRPC_BAZEL_BUILD //private_set_intersection/cpp/...

# C
bazel test --test_output=all --host_copt=-DGRPC_BAZEL_BUILD //private_set_intersection/c/...
