//Package client for the client-side of the Private Set Intersection protocol.
//
// In PSI, two parties (client and server) each hold a dataset, and at
// the end of the protocol the client learns the size of the intersection of
// both datasets, while no party learns anything beyond that.
//
// This variant of PSI introduces a small false-positive rate (i.e.,
// the reported cardinality will be slightly larger than the actual cardinality.
// The false positive rate can be tuned by the server.
//
// The protocol works as follows.
//
//
// 1. Setup phase
//
// The server encrypts all its elements x under a commutative encryption scheme,
// computing H(x)^s where s is its secret key. The encrypted elements are then
// inserted in a Bloom filter, which is sent to the client encoded as JSON. The
// message has the following form:
//
//   {
//     "num_hash_functions": <int>,
//     "bits": <string>
//   }
//
// Here, `bits` is a Base64-encoded string.
//
// 2. Client request
//
// The client encrypts all their elements x using the commutative encryption
// scheme, computing H(x)^c, where c is the client's secret key. The encoded
// elements are sent to the server as a JSON array of Base64 strings, together
// with a boolean reveal_intersection that indicates whether the client wants to
// learn the elements in the intersection or only its size.
//
//
//   {
//     "reveal_intersection": <bool>,
//     "encrypted_elements": [ Base64(H(x_1)^c), Base64(H(x_2)^c), ... ]
//   }
//
//
// 3. Server response
//
// For each encrypted element H(x)^c received from the client, the server
// encrypts it again under the commutative encryption scheme with its secret
// key s, computing (H(x)^c)^s = H(x)^(cs). The result is sent back to the
// client as a JSON array of strings:
//
//   {
//     "encrypted_elements": [ Base64(H(x_1)^c), Base64(H(x_2)^c), ... ]
//   }
//
// If reveal_intersection is false, the array is sorted to hide the order of
// entries from the client.
//
// 4. Client computes intersection
//
// The client decrypts each element received from the server's response using
// its secret key c, computing (H(x)^(cs))^(1/c) = H(x)^s. It then checks if
// each element is present in the Bloom filter, and reports the number of
// matches as the intersection size.
package client

/*
#include "private_set_intersection/c/psi_client.h"
#include <stdlib.h>
*/
import "C"
import (
	"errors"
	"fmt"
	"github.com/openmined/psi/version"
	"runtime"
	"unsafe"
)

//PsiClient context for the client side of a Private Set Intersection protocol.
type PsiClient struct {
	context C.psi_client_ctx
}

//Create returns a new PSI client
func Create(revealIntersection bool) (*PsiClient, error) {
	psiClient := new(PsiClient)

	var err *C.char
	rcode := C.psi_client_create(C.bool(revealIntersection), &psiClient.context, &err)
	if rcode != 0 {
		return nil, fmt.Errorf("failed to create client context: %v(%v)", psiClient.loadCString(&err), rcode)
	}
	if psiClient.context == nil {
		return nil, errors.New("failed to create client context: Context is NULL. This should never happen")
	}

	runtime.SetFinalizer(psiClient, func(c *PsiClient) { c.Destroy() })
	return psiClient, nil
}

//CreateRequest generates a request message to be sent to the server. For each input
//element x, computes H(x)^c, where c is the secret key of ec_cipher. The
//result is sorted to hide the initial ordering of `rawInput` and encoded as
//a JSON array.
//
//Returns an error if the context is invalid or if the encryption fails.
func (c *PsiClient) CreateRequest(rawInput []string) (string, error) {
	if c.context == nil {
		return "", errors.New("invalid context")
	}

	inputs := []C.struct_psi_client_buffer_t{}
	for idx := range rawInput {
		inputs = append(inputs, C.struct_psi_client_buffer_t{
			buff:     C.CString(rawInput[idx]),
			buff_len: C.size_t(len(rawInput[idx])),
		})
	}

	var out *C.char
	var outlen C.size_t
	var err *C.char

	rcode := C.psi_client_create_request(c.context, &inputs[0], C.size_t(len(inputs)), &out, &outlen, &err)

	for idx := range inputs {
		C.free(unsafe.Pointer(inputs[idx].buff))
	}

	if rcode != 0 {
		return "", fmt.Errorf("create request failed %v(%v)", c.loadCString(&err), rcode)
	}

	return c.loadCString(&out), nil
}

//GetIntersection processes the server's response and returns the intersection of the client
//and server inputs. Use this function if this instance was created with
//`reveal_intersection = true`. The first argument, `server_setup`, is a
//bloom filter that encodes encrypted server elements and is sent by the
//server in a setup phase. The second argument, `server_response`, is the
//response received from the server after sending the result of
//`CreateRequest`.
//
//Returns INVALID_ARGUMENT if any input messages are malformed, or INTERNAL
//if decryption fails.
func (c *PsiClient) GetIntersection(serverSetup, serverResponse string) ([]int64, error) {
	if c.context == nil {
		return nil, errors.New("invalid context")
	}

	var out *C.int64_t
	var outlen C.size_t
	var err *C.char

	csetup := C.CString(serverSetup)
	defer C.free(unsafe.Pointer(csetup))

	cresponse := C.CString(serverResponse)
	defer C.free(unsafe.Pointer(cresponse))

	rcode := C.psi_client_get_intersection(c.context, csetup, cresponse, &out, &outlen, &err)

	if rcode != 0 {
		return nil, fmt.Errorf("process response failed: %v(%v)", c.loadCString(&err), rcode)
	}

	resultPtr := unsafe.Pointer(out)
	resultLen := int64(outlen)

	var result []int64
	var idx int64
	var ref C.int64_t

	for idx = 0; idx < resultLen; idx++ {
		val := (*int64)(unsafe.Pointer(uintptr(resultPtr) + uintptr(idx)*unsafe.Sizeof(&ref)))
		result = append(result, *val)
	}
	C.free(unsafe.Pointer(out))
	return result, nil
}

// GetIntersectionSize reveals the size of the intersection. Use
// this function if this instance was created with `reveal_intersection =
// false`.
//
// Returns INVALID_ARGUMENT if any input messages are malformed, or INTERNAL
// if decryption fails.
func (c *PsiClient) GetIntersectionSize(serverSetup, serverResponse string) (int64, error) {
	if c.context == nil {
		return 0, errors.New("invalid context")
	}

	var result C.int64_t
	var err *C.char

	csetup := C.CString(serverSetup)
	defer C.free(unsafe.Pointer(csetup))

	cresponse := C.CString(serverResponse)
	defer C.free(unsafe.Pointer(cresponse))

	rcode := C.psi_client_get_intersection_size(c.context, csetup, cresponse, &result, &err)

	if rcode != 0 {
		return 0, fmt.Errorf("process response failed: %v(%v)", c.loadCString(&err), rcode)
	}
	return int64(result), nil
}

//Destroy frees the C context.
func (c *PsiClient) Destroy() {
	if c.context == nil {
		return
	}
	C.psi_client_delete(&c.context)
	c.context = nil
}

//Version of the library.
func (c *PsiClient) Version() string {
	return version.Version()
}

func (c *PsiClient) loadCString(buff **C.char) string {
	str := C.GoString(*buff)
	C.free(unsafe.Pointer(*buff))
	return str
}
