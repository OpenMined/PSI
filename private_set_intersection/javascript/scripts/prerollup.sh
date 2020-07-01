#!/usr/bin/env bash
set -euo pipefail

# Clean
rm -rf private_set_intersection/javascript/bin/*
rm -rf private_set_intersection/javascript/dist/*

# Rebuild the protobufs to re-add them
./private_set_intersection/javascript/scripts/build-proto.sh

# Copy bazel output files into the javascript `bin` folder
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_js.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_wasm.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_js.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_wasm.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_js.js private_set_intersection/javascript/bin/
cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_wasm.js private_set_intersection/javascript/bin/
