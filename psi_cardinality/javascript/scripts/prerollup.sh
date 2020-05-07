#!/usr/bin/env bash
set -euo pipefail

# Clean
rm -rf psi_cardinality/javascript/bin/*
rm -rf psi_cardinality/javascript/dist/*

# Copy bazel output files into the javascript `bin` folder
cp -rf bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_client_js.js psi_cardinality/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_client_wasm.js psi_cardinality/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_server_js.js psi_cardinality/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_server_wasm.js psi_cardinality/javascript/bin/
