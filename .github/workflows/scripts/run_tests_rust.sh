#!/bin/sh
set -e

if [ "${RUNNER_OS}" -eq "macOS" ]:
then
    bazel clean --expunge
    export MACOSX_DEPLOYMENT_TARGET=10.14
fi

# Rust
bazel test --test_output=all //private_set_intersection/rust/...
