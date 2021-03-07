#!/bin/sh
set -e

# Python + Bazel
bazel test --test_output=all --test_timeout=900 --incompatible_require_linker_input_cc_api=false --features=-supports_dynamic_linker //private_set_intersection/python:tests

# Python package
pip install .
python private_set_intersection/python/tests.py
