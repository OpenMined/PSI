//! Provides a way to create a `PsiServer` for the server side of the Private Set Intersection
//! protocol.

use crate::datastructure::PsiDataStructure;
use libc::*;
use protobuf::{self, Message};
use psi_rust_proto::*;
use std::ffi::CStr;
use std::{error, fmt, ptr, slice};
type PsiServerContext = *mut c_void;

/// A handle for the C context of a PSI server.
pub struct PsiServer {
    ctx: PsiServerContext,
}

#[repr(C)]
struct PsiServerBuffer {
    ptr: *const c_char,
    len: size_t,
}

extern "C" {
    fn psi_server_create_with_new_key(
        reveal_intersection: bool,
        ctx: *mut PsiServerContext,
        error_out: *mut *mut c_char,
    ) -> c_int;
    fn psi_server_create_from_key(
        key_bytes: PsiServerBuffer,
        reveal_intersection: bool,
        ctx: *mut PsiServerContext,
        error_out: *mut *mut c_char,
    ) -> c_int;
    fn psi_server_delete(ctx: *mut PsiServerContext);
    fn psi_server_create_setup_message(
        ctx: PsiServerContext,
        fpr: c_double,
        num_client_inputs: i64,
        input: *const PsiServerBuffer,
        input_len: size_t,
        output: *mut *mut c_char,
        output_len: *mut size_t,
        error_out: *mut *mut c_char,
        ds: PsiDataStructure,
    ) -> c_int;
    fn psi_server_process_request(
        ctx: PsiServerContext,
        client_request: PsiServerBuffer,
        output: *mut *mut c_char,
        output_len: *mut size_t,
        error_out: *mut *mut c_char,
    ) -> c_int;
    fn psi_server_get_private_key_bytes(
        ctx: PsiServerContext,
        output: *mut *mut c_char,
        output_len: *mut size_t,
        error_out: *mut *mut c_char,
    ) -> c_int;
}

impl PsiServer {
    /// Creates a new `PsiServer` instance with a fresh private key.
    pub fn create_with_new_key(reveal_intersection: bool) -> ServerResult<Self> {
        let mut server = Self {
            ctx: ptr::null_mut(),
        };
        let mut error_null_ptr: *mut c_char = ptr::null_mut();
        let error_ptr = &mut error_null_ptr;

        let res_code = unsafe {
            psi_server_create_with_new_key(reveal_intersection, &mut server.ctx, error_ptr)
        };

        if res_code != 0 {
            return Err(get_error(
                "Failed to create server context",
                *error_ptr,
                res_code,
            ));
        }

        if server.ctx.is_null() {
            return Err(ServerError::new(
                "Failed to create server context: Context is NULL. This should never happen."
                    .to_string(),
            ));
        }

        Ok(server)
    }

    /// Creates a new `PsiServer` instance with the provided private key.
    ///
    /// **Warning: this function should be used with caution, since reusing the server key
    /// for multiple requests can reveal information about the input sets. If in doubt,
    /// use `PsiServer::create_with_new_key`.**
    pub fn create_from_key(key: &[u8], reveal_intersection: bool) -> ServerResult<Self> {
        let mut server = Self {
            ctx: ptr::null_mut(),
        };
        let mut error_null_ptr: *mut c_char = ptr::null_mut();
        let error_ptr = &mut error_null_ptr;

        let key_bytes = PsiServerBuffer {
            ptr: key.as_ptr() as *const c_char,
            len: key.len() as size_t,
        };

        let res_code = unsafe {
            psi_server_create_from_key(key_bytes, reveal_intersection, &mut server.ctx, error_ptr)
        };

        if res_code != 0 {
            return Err(get_error(
                "Failed to create server context",
                *error_ptr,
                res_code,
            ));
        }

        if server.ctx.is_null() {
            return Err(ServerError::new(
                "Failed to create server context: Context is NULL. This should never happen."
                    .to_string(),
            ));
        }

        Ok(server)
    }

    /// Creates a setup message that can be sent to the client from the server's bloom
    /// filter that contains `H(x)^s`, for each element `x`, where `s` is the server's secret
    /// key.
    ///
    /// The false-positive rate `fpr` is the probability that any query of size `input_count`
    /// will result in a false positive.
    pub fn create_setup_message<T: AsRef<str>>(
        &self,
        fpr: f64,
        input_count: usize,
        raw_input: &[T],
        ds: Option<PsiDataStructure>,
    ) -> ServerResult<ServerSetup> {
        let input: Vec<PsiServerBuffer> = raw_input
            .iter()
            .map(|s| PsiServerBuffer {
                ptr: s.as_ref().as_ptr() as *const c_char,
                len: s.as_ref().len() as size_t,
            })
            .collect();

        let mut error_null_ptr: *mut c_char = ptr::null_mut();
        let error_ptr = &mut error_null_ptr;
        let mut out_null_ptr: *mut c_char = ptr::null_mut();
        let out_ptr = &mut out_null_ptr;
        let mut out_len = 0 as size_t;

        let res_code = unsafe {
            psi_server_create_setup_message(
                self.ctx,
                fpr as c_double,
                input_count as i64,
                input.as_ptr(),
                input.len() as size_t,
                out_ptr,
                &mut out_len,
                error_ptr,
                ds.unwrap_or_default(),
            )
        };

        if res_code != 0 {
            return Err(get_error(
                "Failed to create setup message",
                *error_ptr,
                res_code,
            ));
        }

        let res =
            unsafe { slice::from_raw_parts(*out_ptr as *const u8, out_len as usize) }.to_owned();
        unsafe { free(*out_ptr as *mut c_void) };
        let server_setup: ServerSetup = match protobuf::parse_from_bytes(&res) {
            Ok(s) => s,
            Err(e) => return Err(ServerError::new(e.to_string())),
        };

        Ok(server_setup)
    }

    /// Processes a client request and creates the server response that can be sent
    /// to the client.
    ///
    /// For each encrypted element `H(x)^c`, `(H(x)^c)^s) = H(x)^(cs)` is calculated
    /// and returned in the response.
    ///
    /// If `reveal_intersection = false`, then the calculated elements are sorted, so
    /// the client can only learn the size of the intersection, without being able to
    /// match individual elements between the client's dataset and the server's dataset.
    pub fn process_request(&self, request_proto: &Request) -> ServerResult<Response> {
        let request = match request_proto.write_to_bytes() {
            Ok(r) => r,
            Err(e) => return Err(ServerError::new(e.to_string())),
        };

        let mut error_null_ptr: *mut c_char = ptr::null_mut();
        let error_ptr = &mut error_null_ptr;
        let mut out_null_ptr: *mut c_char = ptr::null_mut();
        let out_ptr = &mut out_null_ptr;
        let mut out_len = 0 as size_t;
        let request_buf = PsiServerBuffer {
            ptr: request.as_ptr() as *const c_char,
            len: request.len() as size_t,
        };

        let res_code = unsafe {
            psi_server_process_request(self.ctx, request_buf, out_ptr, &mut out_len, error_ptr)
        };

        if res_code != 0 {
            return Err(get_error("Failed to process request", *error_ptr, res_code));
        }

        let res =
            unsafe { slice::from_raw_parts(*out_ptr as *const u8, out_len as usize) }.to_owned();
        unsafe { free(*out_ptr as *mut c_void) };
        let response: Response = match protobuf::parse_from_bytes(&res) {
            Ok(r) => r,
            Err(e) => return Err(ServerError::new(e.to_string())),
        };

        Ok(response)
    }

    /// Returns this `PsiServer` instance's private key. This key should only be used to create
    /// other `PsiServer` instances.
    ///
    /// **Do not send this key to any other party!**
    pub fn get_private_key_bytes(&self) -> ServerResult<Vec<u8>> {
        let mut error_null_ptr: *mut c_char = ptr::null_mut();
        let error_ptr = &mut error_null_ptr;
        let mut out_null_ptr: *mut c_char = ptr::null_mut();
        let out_ptr = &mut out_null_ptr;
        let mut out_len = 0 as size_t;

        let res_code =
            unsafe { psi_server_get_private_key_bytes(self.ctx, out_ptr, &mut out_len, error_ptr) };

        if res_code != 0 {
            return Err(get_error(
                "Failed to get private key bytes",
                *error_ptr,
                res_code,
            ));
        }

        // private key is 32 bytes long
        assert_eq!(out_len as usize, 32);

        let res =
            unsafe { slice::from_raw_parts(*out_ptr as *const u8, out_len as usize) }.to_owned();
        unsafe { free(*out_ptr as *mut c_void) };

        Ok(res)
    }
}

impl Drop for PsiServer {
    fn drop(&mut self) {
        unsafe {
            psi_server_delete(&mut self.ctx);
        }
    }
}

fn get_error(msg: &str, error_ptr: *mut c_char, res_code: c_int) -> ServerError {
    let error_str = unsafe {
        CStr::from_ptr(error_ptr)
            .to_str()
            .expect("Failed to get error string.")
            .to_owned()
    };

    unsafe {
        free(error_ptr as *mut c_void);
    }

    return ServerError::new(format!("{}: {} ({})", msg, error_str, res_code));
}

/// Result of a fallible PSI server function.
pub type ServerResult<T> = Result<T, ServerError>;

/// Error type typically used to indicate an encryption or C interface error for PSI servers.
#[derive(Debug)]
pub struct ServerError {
    msg: String,
}

impl ServerError {
    fn new(msg: String) -> Self {
        ServerError { msg: msg }
    }
}

impl fmt::Display for ServerError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.msg)
    }
}

impl error::Error for ServerError {
    fn description(&self) -> &str {
        &self.msg
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_error() {
        for reveal in [false, true] {
            assert!(PsiServer::create_from_key(&vec![0u8; 32], reveal).is_err());
        }
    }
}

