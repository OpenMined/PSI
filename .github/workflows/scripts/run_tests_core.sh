#!/bin/sh
set -e

# C++
bazel test -c opt --cxxopt="-std=c++17" --test_output=all //private_set_intersection/cpp/...

# C
bazel test -c opt --cxxopt="-std=c++17" --test_output=all //private_set_intersection/c/...
