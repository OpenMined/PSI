#!/bin/sh
set -e

if [ "${RUNNER_OS}" -eq "macOS" ]:
then
    bazel clean --expunge
fi

# Go
bazel test --test_output=all //private_set_intersection/go/...
