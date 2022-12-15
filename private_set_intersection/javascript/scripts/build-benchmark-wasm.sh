#!/usr/bin/env bash
set -euo pipefail

bazel build -c opt //private_set_intersection/javascript/cpp:psi_benchmark_wasm.js
