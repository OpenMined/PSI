#!/usr/bin/env bash
set -euo pipefail

# Source emsdk
source ./submodules/emsdk/emsdk_env.sh

# Build the WASM variant
bazel build -c dbg //psi_cardinality/javascript:psi_cardinality_server_wasm.js --config=wasm
