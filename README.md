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
psi_cardinality/<target language>/<sources>
```

### C++

Build all libraries with or without optimizations, or build a specific module

```
# Build everything using the fastbuild optimization configuration
bazel build //psi_cardinality/cpp/...

# With a specific optimization flag '-c opt'
bazel build -c opt //psi_cardinality/cpp/...

# Specific module(s)
bazel build -c opt //psi_cardinality/cpp:psi_cardinality_client
bazel build -c opt //psi_cardinality/cpp:psi_cardinality_server
```

Build and run tests

```
bazel test //psi_cardinality/cpp/...
```

Build and run benchmarks

```
bazel run -c opt //psi_cardinality/cpp:psi_cardinality_benchmark
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

Now, install the rest of the dev dependencies

```
yarn install
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

To build the client, server, or both for WebAssembly and pure JS

```
yarn build:client
yarn build:server

# or both
yarn build
```

Run the tests or generate coverage reports. **Note** tests are run using the WASM variant.

```
yarn test

# or to see coverage
yarn coverage
```

### Go

[![Go Report Card](https://goreportcard.com/badge/github.com/bcebere/psi-cardinality)](https://goreportcard.com/report/github.com/bcebere/psi-cardinality)

#### PSI client

[![Documentation](https://img.shields.io/badge/godoc-reference-blue.svg)](https://pkg.go.dev/github.com/bcebere/psi-cardinality/psi_cardinality/go/client)
```
import "github.com/openmined/psi-cardinality/client"
```

#### PSI server

[![Documentation](https://img.shields.io/badge/godoc-reference-blue.svg)](https://pkg.go.dev/github.com/bcebere/psi-cardinality/psi_cardinality/go/server)
```
import "github.com/openmined/psi-cardinality/server"
```

#### Tests

```
bazel test //psi_cardinality/go/... --test_output=all
```

#### Benchmarks

Build and run benchmarks

```
bazel test //psi_cardinality/go/... --test_arg=-test.bench=. --test_output=all
```


## Using the Library

To use this library in another Bazel project, add the following in your WORKSPACE file:

```
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
   name = "org_openmined_psi_cardinality",
   remote = "https://github.com/OpenMined/psi-cardinality",
   branch = "master",
   init_submodules = True,
)

load("@org_openmined_psi_cardinality//psi_cardinality:preload.bzl", "psi_cardinality_preload")

psi_cardinality_preload()

load("@org_openmined_psi_cardinality//psi_cardinality:deps.bzl", "psi_cardinality_deps")

psi_cardinality_deps()

```

A full description of the protocol can be found in the documentation of the [PSICardinalityClient](psi_cardinality/cpp/psi_cardinality_client.h) class.
The corresponding server class is [PSICardinalityServer](psi_cardinality/cpp/psi_cardinality_server.h).
An example of how to interleave the different phases of the protocol can be found in [psi_cardinality_server_test.cpp](psi_cardinality/cpp/psi_cardinality_server_test.cpp).
