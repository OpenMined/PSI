#!/usr/bin/env bash
set -euo pipefail

bazel build --config=wasm -c opt //private_set_intersection/javascript/cpp:psi_wasm_node.js
bazel build --config=wasm -c opt //private_set_intersection/javascript/cpp:psi_wasm_web.js
bazel build --config=wasm -c opt //private_set_intersection/javascript/cpp:psi_wasm_worker.js

# Build the protobuf compiler.
bazel build -c opt --platforms="@local_config_platform//:host" @protobuf//:protoc
