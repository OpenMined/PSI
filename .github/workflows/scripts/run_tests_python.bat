echo on

python -m pip install --upgrade pip

echo %pythonLocation%
dir %pythonLocation%/

set PATH=%PATH%;%pythonLocation%
set PYTHONPATH=%pythonLocation%
set PYTHON_BIN_PATH=%pythonLocation%\python.exe

bazel.exe test --test_output=all --cache_test_results=no --test_timeout=900 //private_set_intersection/python:tests
