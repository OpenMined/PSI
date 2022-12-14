#!/usr/bin/env bash
set -euo pipefail

bazel build -c opt \
--copt="-Wno-error=deprecated-declarations" \
--copt="-Wno-error=deprecated-builtins" \
--copt="-Wno-error=deprecated-non-prototype" \
--copt="-Wno-error=unused-function" \
--copt="-Wno-error=array-parameter" \
--copt="-Wno-error=pessimizing-move" \
--copt="-Wno-error=unused-but-set-variable" \
//private_set_intersection/javascript/cpp:psi_wasm_node.js

bazel build -c opt \
--copt="-Wno-error=deprecated-declarations" \
--copt="-Wno-error=deprecated-builtins" \
--copt="-Wno-error=deprecated-non-prototype" \
--copt="-Wno-error=unused-function" \
--copt="-Wno-error=array-parameter" \
--copt="-Wno-error=pessimizing-move" \
--copt="-Wno-error=unused-but-set-variable" \
//private_set_intersection/javascript/cpp:psi_wasm_web.js

bazel build -c opt \
--copt="-Wno-error=deprecated-declarations" \
--copt="-Wno-error=deprecated-builtins" \
--copt="-Wno-error=deprecated-non-prototype" \
--copt="-Wno-error=unused-function" \
--copt="-Wno-error=array-parameter" \
--copt="-Wno-error=pessimizing-move" \
--copt="-Wno-error=unused-but-set-variable" \
//private_set_intersection/javascript/cpp:psi_wasm_worker.js
