#!/usr/bin/env bash
set -euo pipefail

# Source emsdk
source ./third_party/emsdk/emsdk_env.sh

# Build the WASM variant
bazel build -c opt //private_set_intersection/javascript:psi_client_wasm_node.js --config=wasm
bazel build -c opt //private_set_intersection/javascript:psi_client_wasm_web.js --config=wasm
bazel build -c opt //private_set_intersection/javascript:psi_client_wasm_worker.js --config=wasm