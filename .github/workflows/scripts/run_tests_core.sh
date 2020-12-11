#!/bin/sh
set -e

printenv

bazel clean --expunge
export MACOSX_DEPLOYMENT_TARGET=10.13

# C++
bazel test --test_output=all //private_set_intersection/cpp/...

# C
bazel test --test_output=all //private_set_intersection/c/...
