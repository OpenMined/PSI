#!/usr/bin/env bash

# Copy bazel output files into the javascript `dist` folder
test -f bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_client_js.js && cp -rf bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_client_js.js psi_cardinality/javascript/bin/
test -f bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_client_wasm.js && cp -rf  bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_client_wasm.js psi_cardinality/javascript/bin/
test -f bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_combined_js.js && cp -rf  bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_combined_js.js psi_cardinality/javascript/bin/
test -f bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_combined_wasm.js && cp -rf  bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_combined_wasm.js psi_cardinality/javascript/bin/
test -f bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_server_js.js && cp -rf  bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_server_js.js psi_cardinality/javascript/bin/
test -f bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_server_wasm.js && cp -rf  bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_server_wasm.js psi_cardinality/javascript/bin/
test -f bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_benchmark_js.js && cp -rf  bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_benchmark_js.js psi_cardinality/javascript/bin/
test -f bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_benchmark_wasm.js && cp -rf  bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_benchmark_wasm.js psi_cardinality/javascript/bin/

exit 0