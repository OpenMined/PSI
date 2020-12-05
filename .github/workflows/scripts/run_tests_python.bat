echo on

python -c "import sys; print('\n'.join(sys.path))"
echo %PYTHONPATH%
echo %pythonLocation%

pip install .
python private_set_intersection/python/tests.py
