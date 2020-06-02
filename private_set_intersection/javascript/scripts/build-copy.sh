#!/usr/bin/env bash

# Copy bazel output files into the javascript `dist` folder
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_js.js && cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_js.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_wasm.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_wasm.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_js.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_js.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_wasm.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_wasm.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_js.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_js.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_wasm.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_wasm.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_benchmark_js.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_benchmark_js.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_benchmark_wasm.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_benchmark_wasm.js private_set_intersection/javascript/bin/

exit 0