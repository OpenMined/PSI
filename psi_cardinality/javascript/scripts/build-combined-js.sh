#!/usr/bin/env bash
set -euo pipefail

# Source emsdk
source ./submodules/emsdk/emsdk_env.sh

# Build the JS variant
bazel build -c opt //psi_cardinality/javascript:psi_cardinality_combined_js.js --config=wasm