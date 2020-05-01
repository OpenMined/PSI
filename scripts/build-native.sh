#!/usr/bin/env bash
set -euo pipefail

# Build the native variant
bazel build -c opt //src/cpp:psi_cardinality_benchmark