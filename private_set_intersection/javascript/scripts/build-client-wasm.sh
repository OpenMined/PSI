#!/usr/bin/env bash
set -euo pipefail

# Source emsdk
source ./third_party/emsdk/emsdk_env.sh

# Build the WASM variant
bazel build -c opt //private_set_intersection/javascript:psi_client_wasm.js --config=wasm