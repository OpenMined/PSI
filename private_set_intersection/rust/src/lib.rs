//! Private Set Intersection protocol based on ECDH and Bloom Filters.
//!
//! This crate provides Rust bindings to the core C++ PSI library, by wrapping the C interface.

use libc;
// re-export protobufs so they can be used by the user
pub use psi_rust_proto;
pub mod client;
pub mod datastructure;
pub mod server;

use std::ffi::CStr;

extern "C" {
    fn psi_version() -> *const libc::c_char;
}

/// Returns the version of the core C++ PSI library.
pub fn version() -> String {
    unsafe {
        CStr::from_ptr(psi_version())
            .to_str()
            .expect("The version string is not valid UTF-8.")
            .to_owned()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use semver;

    #[test]
    fn test_version() {
        semver::Version::parse(&version()).expect("The version string is not formatted correctly.");
    }
}
