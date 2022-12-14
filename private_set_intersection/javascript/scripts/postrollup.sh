#!/usr/bin/env bash
set -euo pipefail

# Copy necessary files to dist directory
cp -rf package.json private_set_intersection/javascript/dist
cp -rf private_set_intersection/javascript/README.md private_set_intersection/javascript/dist
cp -rf LICENSE private_set_intersection/javascript/dist
cp -rf CHANGES.md private_set_intersection/javascript/dist

# Need to copy the protobuf definitions manually as rollup doesn't do this
mkdir -p private_set_intersection/javascript/dist/implementation/proto && cp -rf private_set_intersection/javascript/src/implementation/proto/psi_pb.d.ts private_set_intersection/javascript/dist/implementation/proto

# Change directory into dist and pack to get
# shorter deep import links.
cd private_set_intersection/javascript/dist

# Pack
npm pack
