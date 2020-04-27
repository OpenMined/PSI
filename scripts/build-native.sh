#!/usr/bin/env bash
set -euo pipefail

# Build the native variant
bazel build -c opt //psi_cardinality:psi_cardinality_benchmark