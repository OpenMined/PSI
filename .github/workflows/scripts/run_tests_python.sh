#!/bin/sh
set -e

# Python
bazel test --test_output=all //psi_cardinality/python/tests:tests
