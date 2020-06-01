#!/usr/bin/env bash
set -euo pipefail

# Updates emsdk to the latest version
cd ./third_party/emsdk/
./emsdk update-tags
cd ../../
