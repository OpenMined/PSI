#!/usr/bin/env bash
set -euo pipefail

# Source emsdk
source ./submodules/emsdk/emsdk_env.sh

# Build the JS variant
bazel build -c opt //src/javascript:psi_cardinality_client_js.js --config=js