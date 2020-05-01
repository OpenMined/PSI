#!/usr/bin/env bash
set -euo pipefail

# Source emsdk
source ./submodules/emsdk/emsdk_env.sh

# Build the pure JS variant
bazel build -c opt //src/javascript:psi_cardinality_benchmark_js.js --config=js
