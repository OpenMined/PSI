#!/bin/sh
set -e

if [ ${RUNNER_OS}=="macOS" ]:
then
    bazel clean --expunge
    export -u MACOSX_DEPLOYMENT_TARGET=10.13
fi

# Go
bazel test --test_output=all //private_set_intersection/go/...
