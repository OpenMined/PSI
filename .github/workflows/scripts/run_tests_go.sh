#!/bin/sh
set -e

# Go
bazel test -c opt --cxxopt="-std=c++17" --test_output=all //private_set_intersection/go/...
