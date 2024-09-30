#!/usr/bin/env bash
set -euo pipefail

bazel build --config=wasm -c opt //private_set_intersection/javascript/cpp:psi_benchmark_wasm.js
