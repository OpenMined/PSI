#!/bin/sh
set -e

if [ "${RUNNER_OS}" -eq "macOS" ]:
then
    bazel clean --expunge
fi

# Go
# Extra flags needed for bazel 4.0.0: --incompatible_require_linker_input_cc_api=false --features=-supports_dynamic_linker
bazel test --test_output=all --incompatible_require_linker_input_cc_api=false --features=-supports_dynamic_linker //private_set_intersection/go/...
