// Package server for the server-side of the Private Set Intersection protocol.
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
package server

/*
#include "private_set_intersection/c/psi_server.h"
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

// PsiServer context for the server side of a Private Set Intersection protocol.
type PsiServer struct {
	context C.psi_server_ctx
}

// CreateWithNewKey creates and returns a new server instance with a fresh private key.
//
// Returns an error if any crypto operations fail.
func CreateWithNewKey(revealIntersection bool) (*PsiServer, error) {
	psiServer := new(PsiServer)

	var err *C.char
	rcode := C.psi_server_create_with_new_key(C.bool(revealIntersection), &psiServer.context, &err)
	if rcode != 0 {
		return nil, fmt.Errorf("failed to create server context: %v(%v)", psiServer.loadCString(&err), rcode)
	}
	if psiServer.context == nil {
		return nil, errors.New("failed to create server context: Context is NULL. This should never happen")
	}

	runtime.SetFinalizer(psiServer, func(s *PsiServer) { s.Destroy() })
	return psiServer, nil
}

// CreateFromKey creates and returns a new server instance with the provided private key.
//
// Returns an error if any crypto operations fail.
func CreateFromKey(key []byte, revealIntersection bool) (*PsiServer, error) {
	psiServer := new(PsiServer)

	var err *C.char
	rcode := C.psi_server_create_from_key(C.struct_psi_server_buffer_t{
		buff:     (*C.char)(C.CBytes(key)),
		buff_len: C.size_t(len(key)),
	},
		C.bool(revealIntersection),
		&psiServer.context, &err)

	if rcode != 0 {
		return nil, fmt.Errorf("failed to create server context: %v(%v)", psiServer.loadCString(&err), rcode)
	}
	if psiServer.context == nil {
		return nil, errors.New("failed to create server context: Context is NULL. This should never happen")
	}

	runtime.SetFinalizer(psiServer, func(s *PsiServer) { s.Destroy() })
	return psiServer, nil
}

// CreateSetupMessage creates a setup message from the server's dataset to be sent to the
// client. The setup message is a JSON-encoded Bloom filter containing
// H(x)^s for each element x in `rawInput`, where s is the server's secret
// key. The message has the following form:
//
//   {
//     "num_hash_functions": <int>,
//     "bits": <string>
//   }
//
// `bits` is encoded as Base64.
// The false-positive rate `fpr` is the probability that any query of size
// `num_client_inputs` will result in a false positive.
//
// Returns an error if the context is invalid or if the encryption fails.
func (s *PsiServer) CreateSetupMessage(fpr float64, inputCount int64, rawInput []string) (string, error) {
	if s.context == nil {
		return "", errors.New("invalid context")
	}

	input := []C.struct_psi_server_buffer_t{}
	for idx := range rawInput {
		input = append(input, C.struct_psi_server_buffer_t{
			buff:     C.CString(rawInput[idx]),
			buff_len: C.size_t(len(rawInput[idx])),
		})
	}

	var out *C.char
	var err *C.char
	var outlen C.size_t

	rcode := C.psi_server_create_setup_message(s.context, C.double(fpr), C.int64_t(inputCount), &input[0], C.size_t(len(input)), &out, &outlen, &err)

	for idx := range input {
		C.free(unsafe.Pointer(input[idx].buff))
	}
	if rcode != 0 {
		return "", fmt.Errorf("setup_message failed: %v(%v)", s.loadCString(&err), rcode)
	}
	result := s.loadCString(&out)

	return result, nil
}

// ProcessRequest processes a client query and returns the corresponding server response to
// be sent to the client.
//
// For each encrypted element H(x)^c in the decoded `client_request`, computes
// (H(x)^c)^s = H(X)^(cs) and returns these as a
// sorted JSON array. Sorting the output prevents the client from matching
// the individual response elements to the ones in the request, ensuring
// that they can only learn the intersection size but not individual
// elements in the intersection.
//
// Returns an error if the context is invalid.
func (s *PsiServer) ProcessRequest(request string) (string, error) {
	if s.context == nil {
		return "", errors.New("invalid context")
	}

	var out *C.char
	var err *C.char
	var outlen C.size_t

	crequest := C.CString(request)
	defer C.free(unsafe.Pointer(crequest))

	rcode := C.psi_server_process_request(s.context, C.struct_psi_server_buffer_t{
		buff:     crequest,
		buff_len: C.size_t(len(request)),
	}, &out, &outlen, &err)

	if rcode != 0 {
		return "", fmt.Errorf("process request failed: %v(%v)", s.loadCString(&err), rcode)
	}

	result := s.loadCString(&out)
	return result, nil

}

// GetPrivateKeyBytes returns this instance's private key. This key should only be used to
// create other server instances. DO NOT SEND THIS KEY TO ANY OTHER PARTY!
func (s *PsiServer) GetPrivateKeyBytes() ([]byte, error) {
	if s.context == nil {
		return nil, errors.New("invalid context")
	}

	var out *C.char
	var outlen C.size_t
	var err *C.char

	rcode := C.psi_server_get_private_key_bytes(s.context, &out, &outlen, &err)

	if rcode != 0 {
		return nil, fmt.Errorf("get private keys failed: %v(%v)", s.loadCString(&err), rcode)
	}

	// Convert C array to a Go slice. Private Keys are guaranteed to be 32 bytes long.
	result := C.GoBytes(unsafe.Pointer(out), 32)

	return result, nil
}

// Destroy frees the C context.
func (s *PsiServer) Destroy() {
	if s.context == nil {
		return
	}
	C.psi_server_delete(&s.context)
	s.context = nil
}

// Version of the library.
func (s *PsiServer) Version() string {
	return version.Version()
}

func (s *PsiServer) loadCString(buff **C.char) string {
	str := C.GoString(*buff)
	C.free(unsafe.Pointer(*buff))
	return str
}
