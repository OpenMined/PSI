echo on

python -m pip install --upgrade pip
python -m pip install -r private_set_intersection\python\requirements_dev.txt 
rem set PATH=%PATH%;%pythonLocation%
rem set PYTHON_PATH=%pythonLocation%

rem echo %PYTHON_PATH%

rem SET PYTHONHOME=%PYTHON_PATH%
rem SET PYTHONPATH=%PYTHON_PATH%
rem SET PYTHONEXECUTABLE=%PYTHON_PATH%\python.exe
rem SET PYTHONWEXECUTABLE=%PYTHON_PATH%\pythonw.exe

rem SET PYTHON_EXECUTABLE=%PYTHONEXECUTABLE%
rem SET PYTHONW_EXECUTABLE=%PYTHONWEXECUTABLE%
rem SET PYTHON_BIN_PATH=%PYTHON_EXECUTABLE%
rem SET PYTHON_LIB_PATH=%PYTHON_PATH%\Lib\site-packages
rem SET PATH=%PYTHON_PATH%;%PYTHON_PATH%\Scripts;%DP0%\CMake\Bin;%PATH%

bazel test --test_output=all --cache_test_results=no --test_timeout=900 --python_path=%pythonLocation% //private_set_intersection/python:tests
