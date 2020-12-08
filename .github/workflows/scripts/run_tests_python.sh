#!/bin/sh
set -e

bazel clean --expunge

# Python + Bazel
bazel test --test_output=all --cache_test_results=no --test_timeout=900 //private_set_intersection/python:tests

# Python package
pip install .
python private_set_intersection/python/tests.py
