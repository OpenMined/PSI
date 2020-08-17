//! Private Set Intersection protocol based on ECDH and Bloom Filters.
//!
//! This crate provides Rust bindings to the core C++ PSI library, by wrapping the C interface.

extern crate libc;
extern crate protobuf;
// re-export protobufs so they can be used by the user
pub extern crate psi_rust_proto;

pub mod server;
pub mod client;

use std::ffi::CStr;

extern {
    fn psi_version() -> *const libc::c_char;
}

/// Returns the version of the core C++ PSI library.
pub fn version() -> String {
    unsafe { CStr::from_ptr(psi_version()).to_str().expect("The version string is not valid UTF-8.").to_owned() }
}

#[cfg(test)]
mod tests {
    extern crate semver;

    use super::*;

    #[test]
    fn test_version() {
        semver::Version::parse(&version()).expect("The version string is not formatted correctly.");
    }
}
