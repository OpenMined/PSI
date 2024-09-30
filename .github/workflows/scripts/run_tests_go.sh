#!/bin/sh
set -e

# Go
bazel test --test_output=all //private_set_intersection/go/...
