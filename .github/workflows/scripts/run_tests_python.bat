echo on

python -m pip install --upgrade pip

set PATH=%PATH%;%pythonLocation%
set PYTHON_PATH=%pythonLocation%

echo %PYTHON_PATH%

SET PYTHONHOME=%PYTHON_PATH%
SET PYTHONPATH=%PYTHON_PATH%
SET PYTHONEXECUTABLE=%PYTHON_PATH%\python.exe
SET PYTHONWEXECUTABLE=%PYTHON_PATH%\pythonw.exe

SET PYTHON_EXECUTABLE=%PYTHONEXECUTABLE%
SET PYTHONW_EXECUTABLE=%PYTHONWEXECUTABLE%
SET PYTHON_BIN_PATH=%PYTHON_EXECUTABLE%
SET PYTHON_LIB_PATH=%PYTHON_PATH%\Lib\site-packages
SET PATH=%PYTHON_PATH%;%PYTHON_PATH%\Scripts;%DP0%\CMake\Bin;%PATH%

bazel test --test_output=all --cache_test_results=no --test_timeout=900 --python_path=%pythonLocation% //private_set_intersection/python:tests
