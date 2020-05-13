#!/usr/bin/env bash
set -euo pipefail

# Sets up llvm
cd third_party/emsdk/
./emsdk install latest
./emsdk activate latest
cd ../../