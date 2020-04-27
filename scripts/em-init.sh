#!/usr/bin/env bash
set -euo pipefail

# Sets up llvm
cd submodules/emsdk/ \
&& ./emsdk install latest \
&& ./emsdk activate latest \
&& cd ../../