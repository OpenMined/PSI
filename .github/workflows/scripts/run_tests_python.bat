echo on

python -c "import sys; print('\n'.join(sys.path))"
echo %PYTHONPATH%
echo %pythonLocation%
set PATH=%PATH%;%pythonLocation%
set PYTHONPATH=%pythonLocation%
set PYTHON_BIN_PATH=%pythonLocation%\python.exe

python ./configure.py

bazel test --test_output=all --cache_test_results=no --python_path=%PYTHON_BIN_PATH% //private_set_intersection/python:tests

pip install .
python private_set_intersection/python/tests.py
