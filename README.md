# PSI Cardinality
Private Set Intersection Cardinality protocol based on ECDH and Bloom Filters.

## Compiling and Running
The only requirement is [Bazel](https://bazel.build). To compile, run
```
bazel build //...
```
To run tests, use
```
bazel test //...
```
Benchmarks can be run using
```
bazel run -c opt //psi_cardinality:psi_cardinality_benchmark
```

## Using the Library
To use this library in a Bazel project, put the following in your WORKSPACE file:
```
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
   name = "com_github_schoppmp_psi_cardinality",
   strip_prefix = "psi-cardinality-master",
   url = "https://github.com/schoppmp/psi-cardinality/archive/master.zip",
)

load("@com_github_schoppmp_psi_cardinality//psi_cardinality:preload.bzl", "psi_cardinality_preload")

psi_cardinality_preload()

load("@com_github_schoppmp_psi_cardinality//psi_cardinality:deps.bzl", "psi_cardinality_deps")

psi_cardinality_deps()
```
A full description of the protocol can be found in the documentation of the [PSICardinalityClient](psi_cardinality/psi_cardinality_client.h) class.
The corresponding server class is [PSICardinalityServer](psi_cardinality/psi_cardinality_server.h).
An example of how to interleave the different phases of the protocol can be found in [psi_cardinality_server_test.cpp](psi_cardinality/psi_cardinality_server_test.cpp).
