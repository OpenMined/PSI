#!/usr/bin/env bash
set -euo pipefail

# Source emsdk
source ./third_party/emsdk/emsdk_env.sh

# Build the JS variant
bazel build -c opt //private_set_intersection/javascript:psi_combined_js_node.js --config=wasm
bazel build -c opt //private_set_intersection/javascript:psi_combined_js_web.js --config=wasm
bazel build -c opt //private_set_intersection/javascript:psi_combined_js_worker.js --config=wasm