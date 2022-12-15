echo on

python -m pip install --upgrade pip
python -m pip install -r private_set_intersection\python\requirements_dev.txt 

bazel test -c opt --cxxopt="-std:c++17" --test_output=all //private_set_intersection/python:tests

@REM pip install .
@REM python private_set_intersection\python\tests.py
