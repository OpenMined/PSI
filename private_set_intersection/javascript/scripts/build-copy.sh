#!/usr/bin/env bash

# Copy bazel output files into the javascript `dist` folder
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_js_node.js && cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_js_node.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_js_web.js && cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_js_web.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_js_worker.js && cp -rf bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_js_worker.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_wasm_node.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_wasm_node.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_wasm_web.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_wasm_web.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_wasm_worker.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_client_wasm_worker.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_js_node.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_js_node.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_js_web.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_js_web.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_js_worker.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_js_worker.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_wasm_node.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_wasm_node.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_wasm_web.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_wasm_web.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_wasm_worker.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_combined_wasm_worker.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_js_node.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_js_node.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_js_web.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_js_web.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_js_worker.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_js_worker.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_wasm_node.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_wasm_node.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_wasm_web.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_wasm_web.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_wasm_worker.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_server_wasm_worker.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_benchmark_js.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_benchmark_js.js private_set_intersection/javascript/bin/
test -f bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_benchmark_wasm.js && cp -rf  bazel-out/wasm-opt/bin/private_set_intersection/javascript/psi_benchmark_wasm.js private_set_intersection/javascript/bin/

exit 0