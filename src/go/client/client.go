package client

/*
#cgo LDFLAGS: -L${SRCDIR}/../extern/lib/
#cgo CFLAGS: -I${SRCDIR}/../extern/include/
#cgo LDFLAGS:  -lpsi_cardinality_client -lec_commutative_cipher -lec_util -lelgamal -lbloom_filter -lbn_util -lcrypto -lopenssl_init -lstatus -lglog -lgflags -lstrings -linternal -lraw_logging_internal -lthrow_delegate -lint128

#include "psi_cardinality_client_c.h"
*/
import "C"
import (
	"errors"
)

//PSICardinalityClient context
type PSICardinalityClient struct {
	context C.psi_cardinality_client_ctx
}

//Create returns a new PSI client
func Create() (*PSICardinalityClient, error) {
	psiClient := new(PSICardinalityClient)
	psiClient.context = C.psi_cardinality_client_create()
	if psiClient.context == nil {
		return nil, errors.New("failed to create client context")
	}

	return psiClient, nil
}

//CreateRequest generates a new client request
func (c *PSICardinalityClient) CreateRequest(rawInput []string) (string, error) {
	if c.context == nil {
		return "", errors.New("invalid context")
	}

	inputs := []C.struct_client_buffer_t{}
	for idx := range rawInput {
		inputs = append(inputs, C.struct_client_buffer_t{
			buff:     C.CString(rawInput[idx]),
			buff_len: C.ulong(len(rawInput[idx])),
		})
	}

	var out *C.char
	var outlen C.ulong

	ret := C.psi_cardinality_client_create_request(c.context, &inputs[0], C.ulong(len(inputs)), &out, &outlen)
	if ret < 0 {
		return "", errors.New("create request failed")
	}

	result := C.GoString(out)

	C.psi_cardinality_client_delete_buffer(c.context, &out)

	return result, nil
}

//ProcessResponse returns the intersection size
func (c *PSICardinalityClient) ProcessResponse(serverSetup, serverResponse string) (int64, error) {
	if c.context == nil {
		return 0, errors.New("invalid context")
	}

	result := C.psi_cardinality_client_process_response(c.context, C.CString(serverSetup), C.CString(serverResponse))

	if result < 0 {
		return 0, errors.New("process response failed")
	}
	return int64(result), nil
}

//Destroy frees the context
func (c *PSICardinalityClient) Destroy() error {
	if c.context == nil {
		return errors.New("invalid context")
	}
	C.psi_cardinality_client_delete(&c.context)
	c.context = nil

	return nil
}
