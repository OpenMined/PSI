#!/usr/bin/env bash
set -euo pipefail

# Source emsdk
source ./third_party/emsdk/emsdk_env.sh

# Build the WASM variant
bazel build -c opt //psi_cardinality/javascript:psi_cardinality_combined_wasm.js --config=wasm