#!/usr/bin/env bash
set -euo pipefail

# Clean
rm -rf private_set_intersection/javascript/bin/*
rm -rf private_set_intersection/javascript/dist/*

# Rebuild the protobufs to re-add them
./private_set_intersection/javascript/scripts/build-proto.sh

# Copy bazel output files into the javascript `bin` folder
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_js_node.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_js_web.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_js_worker.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_wasm_node.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_wasm_web.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_wasm_worker.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_js_node.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_js_web.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_js_worker.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_wasm_node.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_wasm_web.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_wasm_worker.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_js_node.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_js_web.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_js_worker.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_wasm_node.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_wasm_web.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_wasm_worker.js private_set_intersection/javascript/bin/
