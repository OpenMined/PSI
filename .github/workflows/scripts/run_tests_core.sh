#!/bin/sh
set -e

if [ "${RUNNER_OS}" -eq "macOS" ]:
then
    bazel clean --expunge
fi

# C++
bazel test --test_output=all //private_set_intersection/cpp/...

# C
bazel test --test_output=all //private_set_intersection/c/...
