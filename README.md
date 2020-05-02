# PSI Cardinality
Private Set Intersection Cardinality protocol based on ECDH and Bloom Filters.

## Requirements

There are requirements for the entire project which each language shares. There also could be requirements for each target language:

### Global Requirements
These are the common requirements across all target languages of this project.

- A compiler such as clang, gcc, or msvc
- [Bazel](https://bazel.build)

### Target Requirements
Some target languages have additional requirements

#### C++

- None

#### JavaScript

- [NodeJS](https://nodejs.org/en/)
- [Yarn](https://yarnpkg.com/)


## Compiling and Running

The repository uses a folder structure to isolate the supported targets from one another:

```
src/<target language>/<sources>
```

### C++

Build all libraries (with all optimization levels), with a specific optimization level (-c opt), or a specific module

```
# Build everything using the fastbuild optimization configuration
bazel build //src/cpp/...

# With a specific optimization flag '-c opt'
bazel build -c opt //src/cpp/...

# Specific module(s)
bazel build -c opt //src/cpp:psi_cardinality_client
bazel build -c opt //src/cpp:psi_cardinality_server
```

Build and run tests

```
bazel test //src/cpp/...
```

Build and run benchmarks

```
bazel run -c opt //src/cpp:psi_cardinality_benchmark
```

### JavaScript

First, ensure you have updated submodules

```
yarn submodule:update
```

Then, update and initialize `emsdk`

```
yarn em:update
yarn em:init
```

Next, build the WebAssembly, pure JS, or both variants

```
yarn build:benchmark:wasm
yarn build:benchmark:js

# or both
yarn build:benchmark
```

Finally, run the benchmark for WebAssembly or pure JS

```
yarn benchmark:wasm
yarn benchmark:js
```

TODO: Build the client and server library wrapped with our JS abstraction.

## Using the Library
To use this library in another Bazel project, add the following in your WORKSPACE file:

```
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
   name = "com_github_openmined_psi_cardinality",
   strip_prefix = "psi-cardinality-master",
   url = "https://github.com/OpenMined/psi-cardinality/archive/master.zip",
)

load("@com_github_openmined_psi_cardinality//src/cpp:preload.bzl", "psi_cardinality_preload")

psi_cardinality_preload()

load("@com_github_openmined_psi_cardinality//src/cpp:deps.bzl", "psi_cardinality_deps")

psi_cardinality_deps()
```

A full description of the protocol can be found in the documentation of the [PSICardinalityClient](src/cpp/psi_cardinality_client.h) class.
The corresponding server class is [PSICardinalityServer](src/cpp/psi_cardinality_server.h).
An example of how to interleave the different phases of the protocol can be found in [psi_cardinality_server_test.cpp](src/cpp/psi_cardinality_server_test.cpp).
