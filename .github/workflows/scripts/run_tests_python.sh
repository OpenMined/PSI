#!/bin/sh
set -e

# Python
bazel test --test_output=all //private_set_intersection/python:tests
