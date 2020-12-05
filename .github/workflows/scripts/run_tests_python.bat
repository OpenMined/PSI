echo on

python -c "import sys; print('\n'.join(sys.path))"
echo %PYTHONPATH%

pip install .
python private_set_intersection/python/tests.py
