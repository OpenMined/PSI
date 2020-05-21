#!/bin/sh
set -e

black --check psi_cardinality/python

# Print changes.
git diff
# Already well formated if 'git diff' doesn't output anything.
! ( git diff |  grep -q ^ ) || exit 1
