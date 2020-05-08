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
	rcode := C.psi_cardinality_client_create(&psiClient.context, &err)
	if rcode != 0 {
		return nil, fmt.Errorf("failed to create client context: %v(%v)", psiClient.loadCString(&err), rcode)
	}
	if psiClient.context == nil {
		return nil, errors.New("failed to create client context: Context is NULL. This should never happen")
	}

	runtime.SetFinalizer(psiClient, func(c *PSICardinalityClient) { c.Destroy() })
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
			buff_len: C.size_t(len(rawInput[idx])),
		})
	}

	var out *C.char
	var outlen C.size_t
	var err *C.char

	rcode := C.psi_cardinality_client_create_request(c.context, &inputs[0], C.size_t(len(inputs)), &out, &outlen, &err)
	if rcode != 0 {
		return "", fmt.Errorf("create request failed %v(%v)", c.loadCString(&err), rcode)
	}

	return c.loadCString(&out), nil
}

//ProcessResponse returns the intersection size
func (c *PSICardinalityClient) ProcessResponse(serverSetup, serverResponse string) (int64, error) {
	if c.context == nil {
		return 0, errors.New("invalid context")
	}

	var result C.int64_t
	var err *C.char

	rcode := C.psi_cardinality_client_process_response(c.context, C.CString(serverSetup), C.CString(serverResponse), &result, &err)

	if rcode != 0 {
		return 0, fmt.Errorf("process response failed: %v(%v)", c.loadCString(&err), rcode)
	}
	return int64(result), nil
}

//Destroy the C context
func (c *PSICardinalityClient) Destroy() {
	if c.context == nil {
		return
	}
	C.psi_cardinality_client_delete(&c.context)
	c.context = nil
}

func (c *PSICardinalityClient) loadCString(buff **C.char) string {
	str := C.GoString(*buff)
	C.psi_cardinality_client_delete_buffer(c.context, buff)
	return str
}
