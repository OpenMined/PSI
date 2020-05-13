#!/bin/sh
set -e

# Lint files (all .cpp and .h files) inplace.
find ./psi-cardinality/  \( -iname *.h -o -iname *.cpp \) | xargs clang-format -i -style='google'
# Print changes.
git diff
# Already well formated if 'git diff' doesn't output anything.
! ( git diff |  grep -q ^ ) || return 1