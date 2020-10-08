#!/usr/bin/env bash
set -euo pipefail

# Source emsdk
source ./third_party/emsdk/emsdk_env.sh

# Build the JS variant
bazel build -c opt //private_set_intersection/javascript:psi_client_js_node.js --config=js
bazel build -c opt //private_set_intersection/javascript:psi_client_js_web.js --config=js
bazel build -c opt //private_set_intersection/javascript:psi_client_js_worker.js --config=js