echo on

python -m pip install --upgrade pip

bazel test --test_output=all --cache_test_results=no --test_timeout=900 //private_set_intersection/python:tests
