echo on

bazel test --test_output=all --cache_test_results=no //private_set_intersection/python:tests

pip install .
python private_set_intersection/python/tests.py

