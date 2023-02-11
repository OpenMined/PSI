// Package client for the client-side of the Private Set Intersection protocol.
//
// In PSI, two parties (client and server) each hold a dataset, and at
// the end of the protocol the client learns the size of the intersection of
// both datasets, while no party learns anything beyond that (cardinality mode).
//
// This variant of PSI introduces a small false-positive rate (i.e.,
// the reported cardinality will be slightly larger than the actual cardinality.
// The false positive rate can be tuned by the server.
//
// The protocol works as follows.
//
// 1. Setup phase
//
// The server encrypts all its elements x under a commutative encryption scheme,
// computing H(x)^s where s is its secret key. The encrypted elements are then
// inserted in a Bloom filter, which is sent to the client in the form of a serialized
// protobuf. The protobuf has the following form:
//
//	{
//	  "num_hash_functions": <int>,
//	  "bits": <string>
//	}
//
// Here, `bits` is a binary string.
//
// 2. Client request
//
// The client encrypts all their elements x using the commutative encryption
// scheme, computing H(x)^c, where c is the client's secret key. The encoded
// elements are sent to the server as an array together with a boolean reveal_intersection
// that indicates whether the client wants to learn the elements in the
// intersection or only its size. The payload is sent as a serialized protobuf
// to the client and holds the following form:
//
//	{
//	  "reveal_intersection": <bool>,
//	  "encrypted_elements": [ H(x_1)^c, H(x_2)^c, ... ]
//	}
//
// 3. Server response
//
// For each encrypted element H(x)^c received from the client, the server
// encrypts it again under the commutative encryption scheme with its secret
// key s, computing (H(x)^c)^s = H(x)^(cs). The result is sent back to the
// client as a serialized protobuf holding the following form:
//
//	{
//	  "encrypted_elements": [ H(x_1)^c, H(x_2)^c, ... ]
//	}
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
	"runtime"
	"unsafe"

	psi_proto "github.com/openmined/psi/pb"
	"github.com/openmined/psi/version"
	"google.golang.org/protobuf/proto"
)

// PsiClient context for the client side of a Private Set Intersection protocol.
type PsiClient struct {
	context C.psi_client_ctx
}

// CreateWithNewKey creates and returns a new client instance with a fresh private key.
// WARNING: This function should be used with caution, since reusing the
// client key for multiple requests can reveal information about the input
// sets. If in doubt, use `CreateWithNewKey`.
//
// Returns an error if any crypto operations fail.
func CreateWithNewKey(revealIntersection bool) (*PsiClient, error) {
	psiClient := new(PsiClient)

	var err *C.char
	rcode := C.psi_client_create_with_new_key(C.bool(revealIntersection), &psiClient.context, &err)
	if rcode != 0 {
		return nil, fmt.Errorf("failed to create client context: %v(%v)", psiClient.loadString(&err), rcode)
	}
	if psiClient.context == nil {
		return nil, errors.New("failed to create client context: Context is NULL. This should never happen")
	}

	runtime.SetFinalizer(psiClient, func(c *PsiClient) { c.Destroy() })
	return psiClient, nil
}

// CreateFromKey creates and returns a new client instance with the provided private key.
//
// Returns an error if any crypto operations fail.
func CreateFromKey(key []byte, revealIntersection bool) (*PsiClient, error) {
	psiClient := new(PsiClient)

	var err *C.char
	rcode := C.psi_client_create_from_key(C.struct_psi_client_buffer_t{
		buff:     (*C.char)(C.CBytes(key)),
		buff_len: C.size_t(len(key)),
	},
		C.bool(revealIntersection),
		&psiClient.context, &err)

	if rcode != 0 {
		return nil, fmt.Errorf("failed to create client context: %v(%v)", psiClient.loadString(&err), rcode)
	}
	if psiClient.context == nil {
		return nil, errors.New("failed to create client context: Context is NULL. This should never happen")
	}

	runtime.SetFinalizer(psiClient, func(s *PsiClient) { s.Destroy() })
	return psiClient, nil
}

// CreateRequest - Creates a request protobuf to be serialized and sent
// to the server. For each input element x, computes H(x)^c, where c
// is the secret key of ec_cipher_.
//
// Returns an error if the context is invalid or if the encryption fails.
func (c *PsiClient) CreateRequest(rawInput []string) (*psi_proto.Request, error) {
	if c.context == nil {
		return nil, errors.New("invalid context")
	}

	inputs := []C.struct_psi_client_buffer_t{}
	for idx := range rawInput {
		inputs = append(inputs, C.struct_psi_client_buffer_t{
			buff:     C.CString((rawInput[idx])),
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
		return nil, fmt.Errorf("create request failed %v(%v)", c.loadString(&err), rcode)
	}

	bytes := c.loadBytes(&out, C.int(outlen))

	var req psi_proto.Request
	parseErr := proto.Unmarshal(bytes, &req)
	if parseErr != nil {
		return nil, parseErr
	}
	return &req, nil
}

// GetIntersection - processes the server's response and returns the intersection of the client
// and server inputs. Use this function if this instance was created with
// `reveal_intersection = true`. The first argument, `server_setup`, is a
// bloom filter that encodes encrypted server elements and is sent by the
// server in a setup phase. The second argument, `server_response`, is the
// response received from the server after sending the result of
// `CreateRequest`.
//
// Returns INVALID_ARGUMENT if any input messages are malformed, or INTERNAL
// if decryption fails.
func (c *PsiClient) GetIntersection(serverSetupProto *psi_proto.ServerSetup, serverResponseProto *psi_proto.Response) ([]int64, error) {
	if c.context == nil {
		return nil, errors.New("invalid context")
	}

	serverSetup, parseErr := proto.Marshal(serverSetupProto)
	if parseErr != nil {
		return nil, parseErr
	}
	serverResponse, parseErr := proto.Marshal(serverResponseProto)
	if parseErr != nil {
		return nil, parseErr
	}

	var out *C.int64_t
	var outlen C.size_t
	var err *C.char

	csetup := (*C.char)(C.CBytes(serverSetup))
	defer C.free(unsafe.Pointer(csetup))

	cresponse := (*C.char)(C.CBytes(serverResponse))
	defer C.free(unsafe.Pointer(cresponse))

	rcode := C.psi_client_get_intersection(c.context, C.struct_psi_client_buffer_t{csetup, C.size_t(len(serverSetup))}, C.struct_psi_client_buffer_t{cresponse, C.size_t(len(serverResponse))}, &out, &outlen, &err)

	if rcode != 0 {
		return nil, fmt.Errorf("process response failed: %v(%v)", c.loadString(&err), rcode)
	}

	// Cast C pointer as a pointer to a Go array that is large enough to hold any reasonable size.
	// This array is not actually allocated, but it allows us to convert the C array to a Go slice
	// without doing pointer arithmetic by hand.
	resultArray := (*[int64(1) << 32]C.int64_t)(unsafe.Pointer(out))
	result := make([]int64, 0, outlen)
	for idx := int64(0); idx < int64(outlen); idx++ {
		result = append(result, int64((*resultArray)[idx]))
	}
	C.free(unsafe.Pointer(out))
	return result, nil
}

// GetIntersectionSize - reveals the size of the intersection. Use
// this function if this instance was created with `reveal_intersection =
// false`.
//
// Returns INVALID_ARGUMENT if any input messages are malformed, or INTERNAL
// if decryption fails.
func (c *PsiClient) GetIntersectionSize(serverSetupProto *psi_proto.ServerSetup, serverResponseProto *psi_proto.Response) (int64, error) {
	if c.context == nil {
		return 0, errors.New("invalid context")
	}

	serverSetup, parseErr := proto.Marshal(serverSetupProto)
	if parseErr != nil {
		return 0, parseErr
	}
	serverResponse, parseErr := proto.Marshal(serverResponseProto)
	if parseErr != nil {
		return 0, parseErr
	}

	var result C.int64_t
	var err *C.char

	csetup := (*C.char)(C.CBytes(serverSetup))
	defer C.free(unsafe.Pointer(csetup))

	cresponse := (*C.char)(C.CBytes(serverResponse))
	defer C.free(unsafe.Pointer(cresponse))

	rcode := C.psi_client_get_intersection_size(c.context, C.struct_psi_client_buffer_t{csetup, C.size_t(len(serverSetup))}, C.struct_psi_client_buffer_t{cresponse, C.size_t(len(serverResponse))}, &result, &err)

	if rcode != 0 {
		return 0, fmt.Errorf("process response failed: %v(%v)", c.loadString(&err), rcode)
	}
	return int64(result), nil
}

// GetPrivateKeyBytes - returns this instance's private key. This key should only be used to
// create other client instances. DO NOT SEND THIS KEY TO ANY OTHER PARTY!
func (c *PsiClient) GetPrivateKeyBytes() ([]byte, error) {
	if c.context == nil {
		return nil, errors.New("invalid context")
	}

	var out *C.char
	var outlen C.size_t
	var err *C.char

	rcode := C.psi_client_get_private_key_bytes(c.context, &out, &outlen, &err)

	if rcode != 0 {
		return nil, fmt.Errorf("get private keys failed: %v(%v)", c.loadString(&err), rcode)
	}

	// Convert C array to a Go slice. Private Keys are guaranteed to be 32 bytes long.
	result := C.GoBytes(unsafe.Pointer(out), 32)

	return result, nil
}

// Destroy frees the C context.
func (c *PsiClient) Destroy() {
	if c.context == nil {
		return
	}
	C.psi_client_delete(&c.context)
	c.context = nil
}

// Version of the library.
func (c *PsiClient) Version() string {
	return version.Version()
}

func (c *PsiClient) loadBytes(buff **C.char, buflen C.int) []byte {
	str := C.GoBytes(unsafe.Pointer(*buff), buflen)
	C.free(unsafe.Pointer(*buff))
	return str
}

func (c *PsiClient) loadString(buff **C.char) string {
	str := C.GoString(*buff)
	C.free(unsafe.Pointer(*buff))
	return str
}
