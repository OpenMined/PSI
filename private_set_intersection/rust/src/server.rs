use libc::*;

use std::{fmt, error, ptr};
use std::ffi::CStr;

use protobuf::{self, Message};

use psi_rust_proto::*;

type PsiServerContext = *mut c_void;

pub struct PsiServer {
    ctx: PsiServerContext
}

#[repr(C)]
struct PsiServerBuffer {
    ptr: *mut c_char,
    len: size_t
}

extern {
    fn psi_server_create_with_new_key(reveal_intersection: bool, ctx: *mut PsiServerContext, error_out: *mut *mut c_char) -> c_int;
    fn psi_server_create_from_key(key_bytes: PsiServerBuffer, reveal_intersection: bool, ctx: *mut PsiServerContext, error_out: *mut *mut c_char) -> c_int;
    fn psi_server_delete(ctx: *mut PsiServerContext);
    fn psi_server_create_setup_message(ctx: PsiServerContext, fpr: c_double, num_client_inputs: i64, input: *mut PsiServerBuffer, input_len: size_t, output: *mut *mut c_char, output_len: *mut size_t, error_out: *mut *mut c_char) -> c_int;
    fn psi_server_process_request(ctx: PsiServerContext, client_request: PsiServerBuffer, output: *mut *mut c_char, output_len: *mut size_t, error_out: *mut *mut c_char) -> c_int;
    fn psi_server_get_private_key_bytes(ctx: PsiServerContext, output: *mut *mut c_char, output_len: *mut size_t, error_out: *mut *mut c_char) -> c_int;
}

impl PsiServer {
    pub fn create_with_new_key(reveal_intersection: bool) -> ServerResult<Self> {
        unsafe {
            let mut server = Self { ctx: ptr::null_mut() };
            let mut null_ptr: *mut c_char = ptr::null_mut();
            let error_ptr = &mut null_ptr;

            let res_code = psi_server_create_with_new_key(reveal_intersection, &mut server.ctx, error_ptr);

            if res_code != 0 {
                let error_str = CStr::from_ptr(*error_ptr).to_str().unwrap().to_owned();
                free(*error_ptr as *mut c_void);
                return Err(ServerError::new(format!("Failed to create server context: {} ({})", error_str, res_code)));
            }

            if server.ctx.is_null() {
                return Err(ServerError::new("Failed to create server context: Context is NULL. This should never happen.".to_string()));
            }

            Ok(server)
        }
    }

    pub fn create_from_key(key: &[u8], reveal_intersection: bool) -> ServerResult<Self> {
        unsafe {
            let mut server = Self { ctx: ptr::null_mut() };
            let mut null_ptr: *mut c_char = ptr::null_mut();
            let error_ptr = &mut null_ptr;
            let key_bytes = PsiServerBuffer {
                ptr: key.as_ptr() as *mut c_char,
                len: key.len() as size_t
            };

            let res_code = psi_server_create_from_key(key_bytes, reveal_intersection, &mut server.ctx, error_ptr);

            if res_code != 0 {
                let error_str = CStr::from_ptr(*error_ptr).to_str().unwrap().to_owned();
                free(*error_ptr as *mut c_void);
                return Err(ServerError::new(format!("Failed to create server context: {} ({})", error_str, res_code)));
            }

            if server.ctx.is_null() {
                return Err(ServerError::new("Failed to create server context: Context is NULL. This should never happen.".to_string()));
            }

            Ok(server)
        }
    }

    pub fn create_setup_message(&self, fpr: f64, input_count: usize, raw_input: &[&[u8]]) -> ServerResult<ServerSetup> {
        if raw_input.is_empty() {
            return Err(ServerError::new("raw_input cannot be empty".to_string()));
        }

        unsafe {
            let mut input: Vec<PsiServerBuffer> = raw_input.iter().map(|&s| PsiServerBuffer {
                ptr: s.as_ptr() as *mut c_char,
                len: s.len() as size_t
            }).collect();

            let mut null_ptr: *mut c_char = ptr::null_mut();
            let error_ptr = &mut null_ptr;
            let mut null_ptr: *mut c_char = ptr::null_mut();
            let out_ptr = &mut null_ptr;
            let mut out_len = 0 as size_t;

            let res_code = psi_server_create_setup_message(self.ctx, fpr as c_double, input_count as i64, input.as_mut_ptr(), input.len() as size_t, out_ptr, &mut out_len, error_ptr);

            if res_code != 0 {
                let error_str = CStr::from_ptr(*error_ptr).to_str().unwrap().to_owned();
                free(*error_ptr as *mut c_void);
                return Err(ServerError::new(format!("Failed to create setup message: {} ({})", error_str, res_code)));
            }

            // give ownership of the output to a vector, so it will be automatically freed
            let res = Vec::from_raw_parts(*out_ptr as *mut u8, out_len as usize, out_len as usize);
            let server_setup: ServerSetup = match protobuf::parse_from_bytes(&res) {
                Ok(s) => s,
                Err(e) => return Err(ServerError::new(e.to_string()))
            };

            Ok(server_setup)
        }
    }

    pub fn process_request(&self, request_proto: &Request) -> ServerResult<Response> {
        unsafe {
            let mut request = match request_proto.write_to_bytes() {
                Ok(r) => r,
                Err(e) => return Err(ServerError::new(e.to_string()))
            };

            let mut null_ptr: *mut c_char = ptr::null_mut();
            let error_ptr = &mut null_ptr;
            let mut null_ptr: *mut c_char = ptr::null_mut();
            let out_ptr = &mut null_ptr;
            let mut out_len = 0 as size_t;
            let request_buf = PsiServerBuffer {
                ptr: request.as_mut_ptr() as *mut c_char,
                len: request.len() as size_t
            };

            let res_code = psi_server_process_request(self.ctx, request_buf, out_ptr, &mut out_len, error_ptr);

            if res_code != 0 {
                let error_str = CStr::from_ptr(*error_ptr).to_str().unwrap().to_owned();
                free(*error_ptr as *mut c_void);
                return Err(ServerError::new(format!("Failed to process request: {} ({})", error_str, res_code)));
            }

            // give ownership of the output to a vector, so it will be automatically freed
            let res = Vec::from_raw_parts(*out_ptr as *mut u8, out_len as usize, out_len as usize);
            let response: Response = match protobuf::parse_from_bytes(&res) {
                Ok(r) => r,
                Err(e) => return Err(ServerError::new(e.to_string()))
            };

            Ok(response)
        }
    }

    pub fn get_private_key_bytes(&self) -> ServerResult<Vec<u8>> {
        unsafe {
            let mut null_ptr: *mut c_char = ptr::null_mut();
            let error_ptr = &mut null_ptr;
            let mut null_ptr: *mut c_char = ptr::null_mut();
            let out_ptr = &mut null_ptr;
            let mut out_len = 0 as size_t;

            let res_code = psi_server_get_private_key_bytes(self.ctx, out_ptr, &mut out_len, error_ptr);

            if res_code != 0 {
                let error_str = CStr::from_ptr(*error_ptr).to_str().unwrap().to_owned();
                free(*error_ptr as *mut c_void);
                return Err(ServerError::new(format!("Failed to get private key bytes: {} ({})", error_str, res_code)));
            }

            // give ownership of the output to a vector, so it will be automatically freed
            let res = Vec::from_raw_parts(*out_ptr as *mut u8, out_len as usize, out_len as usize);

            // private key is 32 bytes long
            assert_eq!(out_len as usize, 32);

            Ok(res.clone())
        }
    }
}

impl Drop for PsiServer {
    fn drop(&mut self) {
        unsafe {
            psi_server_delete(&mut self.ctx);
        }
    }
}

type ServerResult<T> = Result<T, ServerError>;

#[derive(Debug)]
pub struct ServerError {
    msg: String
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
    fn test_create() {
        PsiServer::create_with_new_key(true).unwrap();
        let server = PsiServer::create_from_key(&vec![1u8; 32], true).unwrap();
        assert_eq!(server.get_private_key_bytes().unwrap(), vec![1u8; 32]);
    }
}
