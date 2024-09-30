#!/usr/bin/env bash
set -euxo pipefail

PATH="$PATH:$(pwd)/node_modules/.bin"

# Path to this plugin.
PROTOC_GEN_TS_PATH="./node_modules/ts-protoc-gen/bin/protoc-gen-ts"

# Directory to write generated code to (.js and .d.ts files).
OUT_DIR_JS="./private_set_intersection/javascript/bin"
OUT_DIR_TS="./private_set_intersection/javascript/src/implementation/proto"

PROTO_DIR="./private_set_intersection/proto"

# Reference the protobuf compiler. Must run build-psi-wasm first!
PROTOC_BINARY="./bazel-bin/external/protobuf~/protoc"

"$PROTOC_BINARY" \
    --plugin="protoc-gen-ts=${PROTOC_GEN_TS_PATH}" \
    --proto_path="${PROTO_DIR}" \
    --js_out="import_style=commonjs,binary:${OUT_DIR_JS}" \
    --ts_out="${OUT_DIR_TS}" \
    psi.proto
