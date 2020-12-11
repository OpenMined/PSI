#!/bin/sh
set -e

if [ "${RUNNER_OS}" -eq "macOS" ]:
then
    bazel clean --expunge
    export -u MACOSX_DEPLOYMENT_TARGET=10.14
fi

# Go
bazel test --test_output=all //private_set_intersection/go/...
