#!/bin/sh
set -e

bazel clean --expunge

# Go
bazel test --test_output=all //private_set_intersection/go/...
