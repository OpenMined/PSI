//! Provides a way to create a `PsiClient` for the client side of the Private Set Intersection
//! protocol.

use libc::*;

use std::{fmt, error, ptr, mem};
use std::ffi::CStr;

use protobuf::{self, Message};

use psi_rust_proto::*;

type PsiClientContext = *mut c_void;

/// A handle for the C context of a PSI client.
pub struct PsiClient {
    ctx: PsiClientContext,
    reveal_intersection: bool
}

#[repr(C)]
struct PsiClientBuffer {
    ptr: *const c_char,
    len: size_t
}

extern {
    fn psi_client_create_with_new_key(reveal_intersection: bool, ctx: *mut PsiClientContext, error_out: *mut *mut c_char) -> c_int;
    fn psi_client_create_from_key(key_bytes: PsiClientBuffer, reveal_intersection: bool, ctx: *mut PsiClientContext, error_out: *mut *mut c_char) -> c_int;
    fn psi_client_delete(ctx: *mut PsiClientContext);
    fn psi_client_create_request(ctx: PsiClientContext, inputs: *const PsiClientBuffer, input_len: size_t, output: *mut *mut c_char, output_len: *mut size_t, error_out: *mut *mut c_char) -> c_int;
    fn psi_client_get_intersection_size(ctx: PsiClientContext, server_setup: PsiClientBuffer, server_response: PsiClientBuffer, output: *mut i64, error_out: *mut *mut c_char) -> c_int;
    fn psi_client_get_intersection(ctx: PsiClientContext, server_setup: PsiClientBuffer, server_response: PsiClientBuffer, output: *mut *mut i64, output_len: *mut size_t, error_out: *mut *mut c_char) -> c_int;
    fn psi_client_get_private_key_bytes(ctx: PsiClientContext, output: *mut *mut c_char, output_len: *mut size_t, error_out: *mut *mut c_char) -> c_int;
}

impl PsiClient {
    /// Creates a new `PsiClient` instance with a fresh private key.
    pub fn create_with_new_key(reveal_intersection: bool) -> ClientResult<Self> {
        unsafe {
            let mut client = Self { ctx: ptr::null_mut(), reveal_intersection: reveal_intersection };
            let mut null_ptr: *mut c_char = ptr::null_mut();
            let error_ptr = &mut null_ptr;

            let res_code = psi_client_create_with_new_key(reveal_intersection, &mut client.ctx, error_ptr);

            if res_code != 0 {
                let error_str = CStr::from_ptr(*error_ptr).to_str().unwrap().to_owned();
                free(*error_ptr as *mut c_void);
                return Err(ClientError::new(format!("Failed to create client context: {} ({})", error_str, res_code)));
            }

            if client.ctx.is_null() {
                return Err(ClientError::new("Failed to create client context: Context is NULL. This should never happen.".to_string()));
            }

            Ok(client)
        }
    }

    /// Creates a new `PsiClient` instance with the provided private key.
    ///
    /// **Warning: this function should be used with caution, since reusing the client key
    /// for multiple requests can reveal information about the input sets. If in doubt,
    /// use `PsiClient::create_with_new_key`.**
    pub fn create_from_key(key: &[u8], reveal_intersection: bool) -> ClientResult<Self> {
        unsafe {
            let mut client = Self { ctx: ptr::null_mut(), reveal_intersection: reveal_intersection };
            let mut null_ptr: *mut c_char = ptr::null_mut();
            let error_ptr = &mut null_ptr;
            let key_bytes = PsiClientBuffer {
                ptr: mem::ManuallyDrop::new(key.to_owned()).as_ptr() as *const c_char,
                len: key.len() as size_t
            };

            let res_code = psi_client_create_from_key(key_bytes, reveal_intersection, &mut client.ctx, error_ptr);

            if res_code != 0 {
                let error_str = CStr::from_ptr(*error_ptr).to_str().unwrap().to_owned();
                free(*error_ptr as *mut c_void);
                return Err(ClientError::new(format!("Failed to create client context: {} ({})", error_str, res_code)));
            }

            if client.ctx.is_null() {
                return Err(ClientError::new("Failed to create client context: Context is NULL. This should never happen.".to_string()));
            }

            Ok(client)
        }
    }

    /// Creates a request protobuf that can be serialized and sent to the server.
    ///
    /// For each input element `x`, this computes `H(x)^c`, where `c` is the client's
    /// secret key for `ec_cipher`.
    pub fn create_request<T: AsRef<str>>(&self, raw_input: &[T]) -> ClientResult<Request> {
        unsafe {
            let input: Vec<PsiClientBuffer> = raw_input.iter().map(|s| PsiClientBuffer {
                ptr: s.as_ref().as_ptr() as *const c_char,
                len: s.as_ref().len() as size_t
            }).collect();

            let mut null_ptr: *mut c_char = ptr::null_mut();
            let error_ptr = &mut null_ptr;
            let mut null_ptr: *mut c_char = ptr::null_mut();
            let out_ptr = &mut null_ptr;
            let mut out_len = 0 as size_t;

            let res_code = psi_client_create_request(self.ctx, input.as_ptr(), input.len() as size_t, out_ptr, &mut out_len, error_ptr);

            if res_code != 0 {
                let error_str = CStr::from_ptr(*error_ptr).to_str().unwrap().to_owned();
                free(*error_ptr as *mut c_void);
                return Err(ClientError::new(format!("Failed to create request: {} ({})", error_str, res_code)));
            }

            // give ownership of the output to a vector, so it will be automatically freed
            let res = Vec::from_raw_parts(*out_ptr as *mut u8, out_len as usize, out_len as usize);
            let request: Request = match protobuf::parse_from_bytes(&res) {
                Ok(r) => r,
                Err(e) => return Err(ClientError::new(e.to_string()))
            };

            Ok(request)
        }
    }

    /// Gets the size of the intersection between the bloom filter of encrypted server
    /// elements sent by the server during the setup phase, and the response given
    /// by the server after sending the request created using `PsiClient::create_request`.
    ///
    /// This should be used if this `PsiClient` instance was created with
    /// `reveal_intersection = false`.
    pub fn get_intersection_size(&self, server_setup: &ServerSetup, response_proto: &Response) -> ClientResult<usize> {
        if self.reveal_intersection {
            return Err(ClientError::new("reveal_intersection must be false!".to_string()));
        }

        unsafe {
            let setup = match server_setup.write_to_bytes() {
                Ok(s) => s,
                Err(e) => return Err(ClientError::new(e.to_string()))
            };
            let response = match response_proto.write_to_bytes() {
                Ok(r) => r,
                Err(e) => return Err(ClientError::new(e.to_string()))
            };

            let mut null_ptr: *mut c_char = ptr::null_mut();
            let error_ptr = &mut null_ptr;
            let mut out_res = 0i64;
            let setup_buf = PsiClientBuffer {
                ptr: setup.as_ptr() as *const c_char,
                len: setup.len() as size_t
            };
            let response_buf = PsiClientBuffer {
                ptr: response.as_ptr() as *const c_char,
                len: response.len() as size_t
            };

            let res_code = psi_client_get_intersection_size(self.ctx, setup_buf, response_buf, &mut out_res, error_ptr);

            if res_code != 0 {
                let error_str = CStr::from_ptr(*error_ptr).to_str().unwrap().to_owned();
                free(*error_ptr as *mut c_void);
                return Err(ClientError::new(format!("Failed to get intersection size: {} ({})", error_str, res_code)));
            }

            Ok(out_res as usize)
        }
    }

    /// Gets the intersection between the bloom filter of encrypted server
    /// elements sent by the server during the setup phase, and the response given
    /// by the server after sending the request created using `PsiClient::create_request`.
    ///
    /// This should be used if this `PsiClient` instance was created with
    /// `reveal_intersection = true`.
    pub fn get_intersection(&self, server_setup: &ServerSetup, response_proto: &Response) -> ClientResult<Vec<i64>> {
        if !self.reveal_intersection {
            return Err(ClientError::new("reveal_intersection must be true!".to_string()));
        }

        unsafe {
            let setup = match server_setup.write_to_bytes() {
                Ok(s) => s,
                Err(e) => return Err(ClientError::new(e.to_string()))
            };
            let response = match response_proto.write_to_bytes() {
                Ok(r) => r,
                Err(e) => return Err(ClientError::new(e.to_string()))
            };

            let mut null_ptr: *mut c_char = ptr::null_mut();
            let error_ptr = &mut null_ptr;
            let mut null_ptr: *mut i64 = ptr::null_mut();
            let out_ptr = &mut null_ptr;
            let mut out_len = 0 as size_t;
            let setup_buf = PsiClientBuffer {
                ptr: setup.as_ptr() as *const c_char,
                len: setup.len() as size_t
            };
            let response_buf = PsiClientBuffer {
                ptr: response.as_ptr() as *const c_char,
                len: response.len() as size_t
            };

            let res_code = psi_client_get_intersection(self.ctx, setup_buf, response_buf, out_ptr, &mut out_len, error_ptr);

            if res_code != 0 {
                let error_str = CStr::from_ptr(*error_ptr).to_str().unwrap().to_owned();
                free(*error_ptr as *mut c_void);
                return Err(ClientError::new(format!("Failed to get intersection: {} ({})", error_str, res_code)));
            }

            // give ownership of the output to a vector, so it will be automatically freed
            let res = Vec::from_raw_parts(*out_ptr as *mut i64, out_len as usize, out_len as usize);

            Ok(res.clone())
        }
    }

    /// Returns this `PsiClient` instance's private key. This key should only be used to create
    /// other `PsiClient` instances.
    ///
    /// **Do not send this key to any other party!**
    pub fn get_private_key_bytes(&self) -> ClientResult<Vec<u8>> {
        unsafe {
            let mut null_ptr: *mut c_char = ptr::null_mut();
            let error_ptr = &mut null_ptr;
            let mut null_ptr: *mut c_char = ptr::null_mut();
            let out_ptr = &mut null_ptr;
            let mut out_len = 0 as size_t;

            let res_code = psi_client_get_private_key_bytes(self.ctx, out_ptr, &mut out_len, error_ptr);

            if res_code != 0 {
                let error_str = CStr::from_ptr(*error_ptr).to_str().unwrap().to_owned();
                free(*error_ptr as *mut c_void);
                return Err(ClientError::new(format!("Failed to get private key bytes: {} ({})", error_str, res_code)));
            }

            // private key is 32 bytes long
            assert_eq!(out_len as usize, 32);

            // do not free the private key bytes
            let mut res = vec![0u8; out_len as usize];
            ptr::copy_nonoverlapping(*out_ptr as *const u8, res.as_mut_ptr(), out_len as usize);

            Ok(res)
        }
    }
}

impl Drop for PsiClient {
    fn drop(&mut self) {
        unsafe {
            psi_client_delete(&mut self.ctx);
        }
    }
}

/// Result of a fallible PSI client function.
pub type ClientResult<T> = Result<T, ClientError>;

/// Error type typically used to indicate an encryption or C interface error for PSI clients.
#[derive(Debug)]
pub struct ClientError {
    msg: String
}

impl ClientError {
    fn new(msg: String) -> Self {
        ClientError { msg: msg }
    }
}

impl fmt::Display for ClientError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.msg)
    }
}

impl error::Error for ClientError {
    fn description(&self) -> &str {
        &self.msg
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_create() {
        for &reveal in &[false, true] {
            let client = PsiClient::create_with_new_key(reveal).unwrap();
            client.create_request::<String>(&vec![]).unwrap();

            let client = PsiClient::create_with_new_key(reveal).unwrap();
            let new_client = PsiClient::create_from_key(&client.get_private_key_bytes().unwrap(), reveal).unwrap();
            assert_eq!(client.get_private_key_bytes().unwrap(), new_client.get_private_key_bytes().unwrap());

            let client = PsiClient::create_from_key(&vec![1u8; 32], reveal).unwrap();
            assert_eq!(client.get_private_key_bytes().unwrap(), vec![1u8; 32]);
        }
    }

    #[test]
    fn test_error() {
        for &reveal in &[false, true] {
            assert!(PsiClient::create_from_key(&vec![0u8; 32], reveal).is_err());
        }
    }
}
