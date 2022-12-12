#!/bin/sh
set -e

# Go
bazel test -c opt --test_output=all //private_set_intersection/go/...
