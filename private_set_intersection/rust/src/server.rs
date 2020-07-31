use libc::*;

use std::{fmt, error, ptr};
use std::ffi::CStr;

type PsiServerContext = *mut c_void;

pub struct PsiServer {
    ctx: PsiServerContext
}

#[repr(C)]
struct PsiServerBuffer {
    ptr: *const c_char,
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
    pub fn create_with_new_key(reveal_intersection: bool) -> Result<Self, ServerError> {
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

    pub fn create_from_key(key: &[u8], reveal_intersection: bool) -> Result<Self, ServerError> {
        unsafe {
            let mut server = Self { ctx: ptr::null_mut() };
            let mut null_ptr: *mut c_char = ptr::null_mut();
            let error_ptr = &mut null_ptr;
            let key_bytes = PsiServerBuffer {
                ptr: key.as_ptr() as *const c_char,
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
}

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
    fn test_create_with_new_key() {
        PsiServer::create_with_new_key(true).unwrap();
        PsiServer::create_from_key(&vec![1u8; 16], true).unwrap();
    }
}
