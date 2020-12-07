echo on

python -c "import sys; print('\n'.join(sys.path))"
echo %PYTHONPATH%
echo %pythonLocation%
set PATH=%PATH%;%pythonLocation%
set PYTHONPATH=%pythonLocation%
set PYTHON_BIN_PATH=%pythonLocation%\python.exe

bazel build //private_set_intersection/python:pybind_wrapper

rem pip install .
rem python private_set_intersection/python/tests.py
