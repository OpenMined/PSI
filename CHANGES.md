# Version 1.0.0

Breaking:

Our goal was to get CI working after 2 years of dormancy. Part of that journey
meant updating almost all dependencies. There are no functional changes in this
release; however, there were many updates to the tooling which meant we needed
to deprecate a few things.

- Python 3.6 and 3.7 are no longer supported and are superceeded by 3.8+. For M1
  macs (arm64), building python < 3.8 is problematic. There is a breaking change
  in the import structure. We simplify the importing logic to remove complexity
  between the development environment and the published wheels. See the
  [code](private_set_intersection/python/__init__.py) for an example.
- The pure Javascript builds are no longer supported or packaged. We're making
  this change because this variant is an order of magnitude slower than WASM
  (which is already slower than the native builds).
- No longer supporting rust < 2018
- No longer supporting building on Windows

Feat:

- Updated to bazel 6.0.0
- Updated to use rust 2018 edition
- Updated to support go 1.17+
- Updated to support python 3.8+
- Removed all remnants of the custom emscripten toolchain (including the git
  submodule) in favor of the native `emsdk` bazel configurations.

Chore:

- Upgraded all dev dependencies for the entire project to the latest possible
  versions that were compatable with each other. This means we have changed
  forks for PJC to another that contains the latest upstream changes but with
  updated/compatable dependencies that the project requires.
- Updated tests to use `-c opt` for all CI tests as it was noticeably faster
- Updated all dev deps for the WebAssembly builds. The WASM build configuration
  is not entirely migrated to bazel and still requires the use of a few npm
  scripts to build, compile, and publish.

# Version 0.3.5

Feat:

- Use an updated version of private_join_and_compute which uses the cleaner absl
  libraries. This also pulls in some new dependencies for PJC such as gRPC.
- Allows the python variant to be built on Windows.
- Emsdk has been bumped to the latest version (emscripten v2.0.15). The latest
  version required changes to the `cc_toolchain_config.bzl` file which addresses
  some work arounds to build successfully.

# Version 0.3.4

Feat:

- Adds native Python protobuf definitions (`python_proto_library`) instead of
  referencing the protobufs from C++. The tradeoff is a more Pythonic library
  for a small performance penalty for serialization/deserialization across the
  Python <-> C++ boundary. However, the cryptographic operations still represent
  the majority of the work in the protocol and therefore the penalty introduced
  via the extra serialization should be negligible.

# Version 0.3.3

Feat:

- Add better Python support for integration with PySyft by adding wrappers.

# Version 0.3.2

Chore:

- Update build dependencies for TypeScript.

# Version 0.3.1

Fix:

- Adds protobuf TypeScript definitions to the bundle.

# Version 0.3.0

Feat:

- Added support for Golomb Compressed Sets as a memory-efficient alternative to
  Bloom Filters.

Breaking:

- The Bloom Filter implementation now has a slightly different API to support
  bulk insertion or intersection.
- The format of the ServerSetup protocol buffer has been changed to support
  serializing both Bloom Filters and Golomb Compressed Sets.

# Version 0.2.3

Feat:

- Include TypeScript declarations inside the bundle to support TS environments
- Reduce the numbed of JS bundles by using `umd` in rollup. No more `cjs`,
  `iife` or `es` as it is not necessary.

Breaking:

- Deep import links now use a single file structure instead of nested folders

# Version 0.2.2

Feat:

- Added support for React Native with WebAssembly

Minor breaking:

- Deep import links have changed to include another level specifying the
  environment, `node`, `web`, or `worker`.

# Version 0.2.1

## JS

Fix:

- There was a misconfiguration in rollup which did not properly bundle protocol
  buffers for the browser.

Breaking:

- Deep import links have changed to now support `cjs`, `iife`, and `es`.
  Notably, `umd` has been removed and superceeded by `cjs` and `iife` due to the
  fix above. Users will need to specify the deep import paths for nodejs (`cjs`)
  and the browser (`iife` or `es`) instead of relying on the previous `umd`
  build.

# Version 0.2.0

Feat:

- Implemented protocol buffers for data-marshaling for all languages

# Version 0.1.0

Initial release!

Feat:

- Data is passed between client and server as json strings for all languages
