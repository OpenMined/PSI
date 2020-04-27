#!/usr/bin/env bash
set -euo pipefail

# Updates emsdk to the latest version
cd ./submodules/emsdk/ \
&& ./emsdk update-tags \
&& cd ../../
