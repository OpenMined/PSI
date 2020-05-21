#!/bin/sh
set -e

# Go
bazel test --test_output=all //psi_cardinality/go/...
