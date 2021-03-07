#!/bin/sh
set -e

# C++
bazel test --test_output=all --incompatible_require_linker_input_cc_api=false --features=-supports_dynamic_linker //private_set_intersection/cpp/...

# C
bazel test --test_output=all --incompatible_require_linker_input_cc_api=false --features=-supports_dynamic_linker //private_set_intersection/c/...
