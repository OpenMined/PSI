package client

/*
#include "psi_cardinality/c/psi_cardinality_client_c.h"
*/
import "C"
import (
	"errors"
	"fmt"
	"runtime"
)

//PSICardinalityClient context
type PSICardinalityClient struct {
	context C.psi_cardinality_client_ctx
}

//Create returns a new PSI client
func Create() (*PSICardinalityClient, error) {
	psiClient := new(PSICardinalityClient)

	var err *C.char
	ret := C.psi_cardinality_client_create(&psiClient.context, &err)
	if ret != 0 {
		return nil, fmt.Errorf("failed to create client context: %v(%v)", C.GoString(err), ret)
	}
	if psiClient.context == nil {
		return nil, errors.New("failed to create client context: null")
	}

	runtime.SetFinalizer(psiClient, destroy)
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

func destroy(c *PSICardinalityClient) error {
	if c.context == nil {
		return errors.New("invalid context")
	}
	C.psi_cardinality_client_delete(&c.context)
	c.context = nil
	return nil
}
