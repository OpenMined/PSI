
![om-logo](https://github.com/OpenMined/design-assets/blob/master/logos/OM/horizontal-primary-trans.png)

[![Tests](https://github.com/OpenMined/PSI/workflows/Tests/badge.svg?branch=master&event=push)](https://github.com/OpenMined/PSI/actions?query=workflow%3ATests+branch%3Amaster+event%3Apush)
![Coverage](https://img.shields.io/codecov/c/github/OpenMined/PSI)
![License](https://img.shields.io/github/license/OpenMined/PSI)
![OpenCollective](https://img.shields.io/opencollective/all/openmined)

# PSI Cardinality

Private Set Intersection Cardinality protocol based on ECDH and Bloom Filters.

## Requirements

There are requirements for the entire project which each language shares. There also could be requirements for each target language:

### Global Requirements

These are the common requirements across all target languages of this project.

- A compiler such as clang, gcc, or msvc
- [Bazel](https://bazel.build)

## Installation

The repository uses a folder structure to isolate the supported targets from one another:

```
psi_cardinality/<target language>/<sources>
```

### C++

See the [C++ README.md](psi_cardinality/cpp/README.md)

### JavaScript

See the [JavaScript README.md](psi_cardinality/javascript/README.md)

### Go

See the [Go README.md](psi_cardinality/go/README.md)

### Python

See the [Python README.md](psi_cardinality/python/README.md)


## Usage

To use this library in another Bazel project, add the following in your WORKSPACE file:

```
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
   name = "org_openmined_psi_cardinality",
   remote = "https://github.com/OpenMined/PSI",
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


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## Contributors

Please make sure to fill this section in with **all former and current** contributors to the project. [Documentation on how to do this is located here.](https://github.com/all-contributors/all-contributors)

## License
[Apache License 2.0](https://choosealicense.com/licenses/apache-2.0/)
