#!/bin/sh
set -e

if [ ${RUNNER_OS}=="macOS" ]:
then
    bazel clean --expunge
    export -u MACOSX_DEPLOYMENT_TARGET=10.13
fi

# C++
bazel test --test_output=all //private_set_intersection/cpp/...

# C
bazel test --test_output=all //private_set_intersection/c/...
