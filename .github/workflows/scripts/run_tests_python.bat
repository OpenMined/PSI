echo on

python -m pip install --upgrade pip
python -m pip install -r private_set_intersection\python\requirements_dev.txt 

pip install .
python private_set_intersection\python\tests.py
