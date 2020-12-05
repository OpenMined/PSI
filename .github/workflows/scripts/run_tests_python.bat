echo on

python -c "import sys; print('\n'.join(sys.path))"

pip install .
python private_set_intersection/python/tests.py
