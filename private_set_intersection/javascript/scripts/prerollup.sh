#!/usr/bin/env bash
set -euo pipefail

# Clean
rm -rf private_set_intersection/javascript/bin/*
rm -rf private_set_intersection/javascript/dist/*

# Rebuild the protobufs to re-add them
./private_set_intersection/javascript/scripts/build-proto.sh

# Copy bazel output files into the javascript `bin` folder
cp -rf bazel-bin/private_set_intersection/javascript/cpp/psi_combined_wasm_node.js/combined_wasm_node.js private_set_intersection/javascript/bin/psi_combined_wasm_node.js
cp -rf bazel-bin/private_set_intersection/javascript/cpp/psi_combined_wasm_web.js/combined_wasm_web.js private_set_intersection/javascript/bin/psi_combined_wasm_web.js
cp -rf bazel-bin/private_set_intersection/javascript/cpp/psi_combined_wasm_worker.js/combined_wasm_worker.js private_set_intersection/javascript/bin/psi_combined_wasm_worker.js
