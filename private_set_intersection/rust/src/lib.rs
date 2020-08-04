//! Private Set Intersection protocol based on ECDH and Bloom Filters.
//!
//! This crate provides Rust bindings to the core C++ PSI library, by wrapping the C interface.

extern crate libc;
extern crate protobuf;
// re-export protobufs so they can be used by the user
pub extern crate psi_rust_proto;

pub mod server;
pub mod client;
