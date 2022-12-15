![om-logo](https://github.com/OpenMined/design-assets/blob/master/logos/OM/horizontal-primary-trans.png)

[![Tests](https://github.com/OpenMined/PSI/workflows/Tests/badge.svg?branch=master&event=push)](https://github.com/OpenMined/PSI/actions?query=workflow%3ATests+branch%3Amaster+event%3Apush)
![License](https://img.shields.io/github/license/OpenMined/PSI)
![OpenCollective](https://img.shields.io/opencollective/all/openmined)

# PSI

Private Set Intersection protocol based on ECDH, Bloom Filters, and Golomb
Compressed Sets.

## Requirements

There are requirements for the entire project which each language shares. There
also could be requirements for each target language:

### Global Requirements

These are the common requirements across all target languages of this project.

- A compiler such as clang, or gcc
- [Bazel](https://bazel.build)

## Installation

The repository uses a folder structure to isolate the supported targets from one
another:

```
private_set_intersection/<target language>/<sources>
```

### C++

See the [C++ README.md](private_set_intersection/cpp/README.md)

### JavaScript

See the [JavaScript README.md](private_set_intersection/javascript/README.md)

### Go

See the [Go README.md](private_set_intersection/go/README.md)

### Python

See the [Python README.md](private_set_intersection/python/README.md)

### Rust

See the [Rust README.md](private_set_intersection/rust/README.md)

## Usage

To use this library in another Bazel project, add the following to your
WORKSPACE file:

```
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
   name = "org_openmined_psi",
   remote = "https://github.com/OpenMined/PSI",
   branch = "master",
)

load("@org_openmined_psi//private_set_intersection:preload.bzl", "psi_preload")

psi_preload()

load("@org_openmined_psi//private_set_intersection:deps.bzl", "psi_deps")

psi_deps()

load("@build_bazel_rules_nodejs//:index.bzl", "node_repositories", "npm_install")

node_repositories()

npm_install(
    name = "npm",
    package_json = "//:package.json",
    package_lock_json = "//:package-lock.json",
)

load("@emsdk//:emscripten_deps.bzl", emsdk_emscripten_deps = "emscripten_deps")

emsdk_emscripten_deps()

```

A full description of the protocol can be found in the documentation of the
[PsiClient](private_set_intersection/cpp/psi_client.h) class. The corresponding
server class is [PsiServer](private_set_intersection/cpp/psi_server.h). An
example of how to interleave the different phases of the protocol can be found
in [psi_server_test.cpp](private_set_intersection/cpp/psi_server_test.cpp).

## Changes

See [CHANGES.md](CHANGES.md).

## Contributing

Pull requests are welcome. For major changes, please open an issue first to
discuss what you would like to change.

Please make sure to update tests as appropriate.

## Contributors

See [CONTRIBUTORS.md](CONTRIBUTORS.md).

## License

[Apache License 2.0](https://choosealicense.com/licenses/apache-2.0/)
