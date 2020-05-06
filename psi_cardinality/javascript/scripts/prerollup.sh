#!/usr/bin/env bash
set -euo pipefail

# Clean the dist folder
rm -rf psi_cardinality/javascript/bin/*

# Copy bazel output files into the javascript `dist` folder
cp -rf bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_client_js.js psi_cardinality/javascript/bin/
cp -rf  bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_client_wasm.js psi_cardinality/javascript/bin/
cp -rf  bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_server_js.js psi_cardinality/javascript/bin/
cp -rf  bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_server_wasm.js psi_cardinality/javascript/bin/
