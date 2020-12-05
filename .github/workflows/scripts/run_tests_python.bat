echo on

python -c "import sys; print('\n'.join(sys.path))"
echo %PYTHONPATH%
echo %pythonLocation%
set PATH=%PATH%;%pythonLocation%
set PYTHONPATH=%pythonLocation%
set PYTHON_BIN_PATH=%pythonLocation%\python.exe

pip install .
python private_set_intersection/python/tests.py
