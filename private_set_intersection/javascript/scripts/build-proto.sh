#!/usr/bin/env bash
set -euo pipefail

# Path to this plugin
PROTOC_GEN_TS_PATH="./node_modules/.bin/protoc-gen-ts"

# Directory to write generated code to (.js and .d.ts files)
OUT_DIR_JS="./private_set_intersection/javascript/bin"
OUT_DIR_TS="./private_set_intersection/javascript/src/implementation/proto"

PROTO_DIR="./private_set_intersection/proto"

protoc \
    --plugin="protoc-gen-ts=${PROTOC_GEN_TS_PATH}" \
    --proto_path="${PROTO_DIR}" \
    --js_out="import_style=commonjs,binary:${OUT_DIR_JS}" \
    --ts_out="${OUT_DIR_TS}" \
    psi.proto
