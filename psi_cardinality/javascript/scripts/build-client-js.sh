#!/usr/bin/env bash
set -euo pipefail

# Source emsdk
source ./third_party/emsdk/emsdk_env.sh

# Build the JS variant
bazel build -c opt //psi_cardinality/javascript:psi_cardinality_client_js.js --config=js