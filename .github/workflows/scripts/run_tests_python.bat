echo on

set MSYS_NO_PATHCONV=1
set MSYS2_ARG_CONV_EXCL="*"

pip install .
python private_set_intersection/python/tests.py
