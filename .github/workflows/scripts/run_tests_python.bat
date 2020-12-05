echo on

python -c "import sys; print('\n'.join(sys.path))"
echo %PYTHONPATH%
echo %pythonLocation%
set PATH=%PATH%;%pythonLocation%
set PYTHONPATH=%pythonLocation%

pip install .
python private_set_intersection/python/tests.py
