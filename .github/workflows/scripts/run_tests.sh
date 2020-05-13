#!/bin/sh
set -e

# C++
bazel test --test_output=all //psi_cardinality/cpp/...

# C
bazel test --test_output=all //psi_cardinality/c/...

# Go
bazel test --test_output=all //psi_cardinality/go/...

# JavaScript
npm run submodule:update
npm run em:update
npm run em:init
npm install
npm run build
npm run test