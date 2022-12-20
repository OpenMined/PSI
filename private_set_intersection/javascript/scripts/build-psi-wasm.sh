#!/usr/bin/env bash
set -euo pipefail

bazel build -c opt //private_set_intersection/javascript/cpp:psi_wasm_node.js

bazel build -c opt //private_set_intersection/javascript/cpp:psi_wasm_web.js

bazel build -c opt //private_set_intersection/javascript/cpp:psi_wasm_worker.js
