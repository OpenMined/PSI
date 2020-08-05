# PSI - Rust

Private Set Intersection protocol based on ECDH and Bloom Filters.

This crate provides Rust bindings to the core C++ PSI library, by wrapping the C interface.

## Build and test

Build libraries and documentation with or without optimizations:

```
# Build everything using the fastbuild optimization configuration
bazel build //private_set_intersection/rust/...

# With a specific optimization flag '-c opt'
bazel build -c opt //private_set_intersection/rust/...
```

Build and run tests:

```
bazel test //private_set_intersection/rust/...
```

## Use in other projects

Add `deps = ["@org_openmined_psi//private_set_intersection/rust:rust_psi"]` to your Bazel rule
in your project's `BUILD` file.

## Updating crate dependencies

Typically, Rust projects are built using Cargo, which reads the `Cargo.toml` file to pull
necessary dependencies. In this project, Bazel is used, so a dummy `Cargo.toml` is created
in `//third_party/cargo` to specify the necessary dependencies. The dummy `Cargo.toml`
should be edited to add or update dependencies.

Afterwards, run `cargo raze` in the `//third_party/cargo` directory to generate the build files
for the Rust dependencies. Cargo raze can be installed through `cargo install cargo-raze`.
This should only need to be done once after updating the dummy `Cargo.toml`.
