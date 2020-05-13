#!/bin/sh
set -e

# C++
bazel test //psi_cardinality/cpp/...

# C
bazel test //psi_cardinality/c/...

# Go
bazel test //psi_cardinality/go/...

# JavaScript
npm run submodule:update
npm run em:update
npm run em:init
npm install
npm run build
npm run test