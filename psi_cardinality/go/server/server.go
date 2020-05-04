package server

/*
#cgo LDFLAGS: -L${SRCDIR}/../extern/lib/
#cgo LDFLAGS: -Wl,-rpath,${SRCDIR}/../extern/lib
#cgo CFLAGS: -I${SRCDIR}/../../cpp -I${SRCDIR}/../../cpp/util
#cgo LDFLAGS:  -lpsi_cardinality_server -lec_commutative_cipher -lec_util -lelgamal -lbloom_filter -lbn_util -lcrypto -lopenssl_init -lstatus -lglog -lgflags -lstrings -linternal -lraw_logging_internal -lthrow_delegate -lint128

#include "psi_cardinality_server_c.h"
*/
import "C"
import (
	"errors"
)

//PSICardinalityServer context
type PSICardinalityServer struct {
	context C.psi_cardinality_server_ctx
}

//CreateWithNewKey returns a new PSI server
func CreateWithNewKey() (*PSICardinalityServer, error) {
	psiServer := new(PSICardinalityServer)
	psiServer.context = C.psi_cardinality_server_create_with_new_key()
	if psiServer.context == nil {
		return nil, errors.New("failed to create server context")
	}

	return psiServer, nil
}

//CreateFromKey returns a new PSI server
func CreateFromKey(key string) (*PSICardinalityServer, error) {
	psiServer := new(PSICardinalityServer)
	psiServer.context = C.psi_cardinality_server_create_from_key(C.struct_server_buffer_t{
		buff:     C.CString(key),
		buff_len: C.ulong(len(key)),
	})
	if psiServer.context == nil {
		return nil, errors.New("failed to create server context from key")
	}

	return psiServer, nil
}

//CreateSetupMessage returns a message from server's dataset to be sent to the client
func (s *PSICardinalityServer) CreateSetupMessage(fpr float64, inputCount int64, rawInput []string) (string, error) {
	if s.context == nil {
		return "", errors.New("invalid context")
	}

	input := []C.struct_server_buffer_t{}
	for idx := range rawInput {
		input = append(input, C.struct_server_buffer_t{
			buff:     C.CString(rawInput[idx]),
			buff_len: C.ulong(len(rawInput[idx])),
		})
	}

	var out *C.char
	var outlen C.ulong

	C.psi_cardinality_server_create_setup_message(s.context, C.double(fpr), C.long(inputCount), &input[0], C.ulong(len(input)), &out, &outlen)

	result := C.GoString(out)
	C.psi_cardinality_server_delete_buffer(s.context, &out)

	return result, nil
}

//ProcessRequest processes the client request
func (s *PSICardinalityServer) ProcessRequest(request string) (string, error) {
	if s.context == nil {
		return "", errors.New("invalid context")
	}

	var out *C.char
	var outlen C.ulong

	ret := C.psi_cardinality_server_process_request(s.context, C.struct_server_buffer_t{
		buff:     C.CString(request),
		buff_len: C.ulong(len(request)),
	}, &out, &outlen)

	if ret < 0 {
		return "", errors.New("process request failed")
	}

	result := C.GoString(out)
	C.psi_cardinality_server_delete_buffer(s.context, &out)

	return result, nil

}

//GetPrivateKeyBytes returns this instance's private key
func (s *PSICardinalityServer) GetPrivateKeyBytes() (string, error) {
	if s.context == nil {
		return "", errors.New("invalid context")
	}

	var out *C.char
	var outlen C.ulong

	ret := C.psi_cardinality_server_get_private_key_bytes(s.context, &out, &outlen)

	if ret < 0 {
		return "", errors.New("get private keys failed")
	}

	result := C.GoString(out)
	C.psi_cardinality_server_delete_buffer(s.context, &out)

	return result, nil
}

//Destroy context
func (s *PSICardinalityServer) Destroy() error {
	if s.context == nil {
		return errors.New("invalid context")
	}
	C.psi_cardinality_server_delete(&s.context)
	s.context = nil
	return nil
}
