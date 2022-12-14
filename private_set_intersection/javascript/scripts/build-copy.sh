#!/usr/bin/env bash

# Copy bazel output files into the javascript `dist` folder

test -f bazel-bin/private_set_intersection/javascript/cpp/psi_wasm_node.js/wasm_node.js && cp -rf  bazel-bin/private_set_intersection/javascript/cpp/psi_wasm_node.js/wasm_node.js private_set_intersection/javascript/bin/psi_wasm_node.js
test -f bazel-bin/private_set_intersection/javascript/cpp/psi_wasm_web.js/wasm_web.js && cp -rf  bazel-bin/private_set_intersection/javascript/cpp/psi_wasm_web.js/wasm_web.js private_set_intersection/javascript/bin/psi_wasm_web.js
test -f bazel-bin/private_set_intersection/javascript/cpp/psi_wasm_worker.js/wasm_worker.js && cp -rf  bazel-bin/private_set_intersection/javascript/cpp/psi_wasm_worker.js/wasm_worker.js private_set_intersection/javascript/bin/psi_wasm_worker.js
test -f bazel-bin/private_set_intersection/javascript/cpp/psi_benchmark_wasm.js/benchmark_wasm.js && cp -rf  bazel-bin/private_set_intersection/javascript/cpp/psi_benchmark_wasm.js/benchmark_wasm.js private_set_intersection/javascript/bin/psi_benchmark_wasm.js

exit 0
