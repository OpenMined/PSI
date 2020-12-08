echo on

python -m pip install --upgrade pip

echo %pythonLocation%
dir %pythonLocation%\

set PATH=%PATH%;%pythonLocation%
set PYTHON_PATH=%pythonLocation%

SET PYTHONHOME=%PYTHON_PATH%
SET PYTHONPATH=%PYTHON_PATH%
SET PYTHONEXECUTABLE=%PYTHON_PATH%\python.exe
SET PYTHONWEXECUTABLE=%PYTHON_PATH%\pythonw.exe

SET PYTHON_EXECUTABLE=%PYTHON_PATH%\python.exe
SET PYTHONW_EXECUTABLE=%PYTHON_PATH%\pythonw.exe
SET PYTHON_BIN_PATH=%PYTHON_EXECUTABLE%
SET PYTHON_LIB_PATH=%PYTHON_PATH%\Lib\site-packages
SET PATH=%PYTHON_PATH%;%PYTHON_PATH%\Scripts;%DP0%\CMake\Bin;%PATH%

bazel.exe test --test_output=all --cache_test_results=no --test_timeout=900 --python_path=%pythonLocation% //private_set_intersection/python:tests
