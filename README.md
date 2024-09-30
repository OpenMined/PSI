![om-logo](https://github.com/OpenMined/design-assets/blob/master/logos/OM/horizontal-primary-trans.png)

[![CD](https://github.com/OpenMined/PSI/actions/workflows/CD.yml/badge.svg?event=release)](https://github.com/OpenMined/PSI/actions/workflows/CD.yml)
![License](https://img.shields.io/github/license/OpenMined/PSI)
![OpenCollective](https://img.shields.io/opencollective/all/openmined)

# PSI

Private Set Intersection protocol based on ECDH and Golomb Compressed Sets or
Bloom Filters.

## Protocol

The Private Set Intersection (PSI) protocol involves two parties, a client and a
server, each holding a dataset. The goal of the protocol is for the client to
determine the intersection between their dataset and the server's dataset,
without revealing any information about their respective datasets to each other.

The protocol proceeds as follows:

1. Setup (server)

The server encrypts all its elements `x` under a commutative encryption scheme,
computing `H(x)^s` where `s` is its secret key. The encrypted elements are then
inserted into a container and sent to the client in the form of a serialized
protobuf and resembles* the following:

```
[ H(x_1)^(s), H(x_2)^(s), ... , H(x_n)^(s) ]
```

2. Request (client)

The client encrypts all their elements `x` using the commutative encryption
scheme, computing `H(x)^c`, where `c` is its secret key. The client sends its
encrypted elements to the server along with a boolean flag,
`reveal_intersection`, indicating whether the client wants to learn the elements
in the intersection or only its size (cardinality). The payload is sent as a
serialized protobuf and resembles* the following:

```
[ H(x_1)^(c), H(x_2)^(c), ... , H(x_n)^(c) ]
```

3. Response (server)

For each encrypted element `H(x)^c` received from the client, the server
encrypts it again under the commutative encryption scheme with its secret key
`s`, computing `(H(x)^c)^s = H(x)^(cs)`. The result is sent back to the client
in a serialized protobuf and resembles* the following:

```
[ H(x_1)^(cs), H(x_2)^(cs), ... , H(x_n)^(cs) ]
```

4. Compute intersection (client)

The client decrypts each element received from the server's response using its
secret key `c`, computing `(H(x)^(cs))^(1/c) = H(x)^s`. It then checks whether
each decrypted element is present in the container received from the server, and
reports the number of matches as the intersection size.

It's worth noting that the protocol has several variants, some of which
introduce a small false-positive rate, while others do not generate false
positives. This behavior is selective, and the false-positive rate can be tuned.
The selection has implications on communication costs as well.

__NOTE resembles*__: The protocol has configurable **containers**. Golomb
Compressed Sets (`Gcs`) is the default container but it can be overridden to be
`BloomFilter` or `Raw` encrypted strings. `Gcs` and `BloomFilter` will have
false positives whereas `Raw` will not. Using `Raw` increases the communication
cost as it is sending raw strings over the wire while the other two options
drastically reduce the cost at the price of having false positives.

## Security

See [SECURITY.md](SECURITY.md).

## Requirements

There are requirements for the entire project which each language shares. There
also could be requirements for each target language:

### Global Requirements

These are the common requirements across all target languages of this project.

- A compiler such as clang or gcc
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

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "emsdk",
    remote = "https://github.com/emscripten-core/emsdk.git",
    strip_prefix = "bazel",
    tag = "3.1.67",
)

load("@emsdk//:deps.bzl", emsdk_deps = "deps")

emsdk_deps()

load("@emsdk//:emscripten_deps.bzl", emsdk_emscripten_deps = "emscripten_deps")

emsdk_emscripten_deps(emscripten_version = "3.1.67")

load("@emsdk//:toolchains.bzl", "register_emscripten_toolchains")

register_emscripten_toolchains()
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
