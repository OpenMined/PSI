//Package server for the server-side of the Private Set Intersection-Cardinality protocol.
//
//In PSI-Cardinality, two parties (client and server) each hold a dataset, and at
//the end of the protocol the client learns the size of the intersection of
//both datasets, while no party learns anything beyond that.
//
//This variant of PSI-Cardinality introduces a small false-positive rate (i.e.,
//the reported cardinality will be slightly larger than the actual cardinality.
//The false positive rate can be tuned by the server.
//
//The protocol works as follows:
//
//
//Setup phase
//
//The server encrypts all its elements x under a commutative encryption scheme,
//computing H(x)^s where s is its secret key. The encrypted elements are then
//inserted in a Bloom filter, which is sent to the client encoded as JSON. The
//message has the following form:
//
//   {
//     "num_hash_functions": <int>,
//     "bits": <string>
//   }
//
//Here, `bits` is a Base64-encoded string.
//
//Client request
//
//The client encrypts all their elements x using the commutative encryption
//scheme, computing H(x)^c, where c is the client's secret key. The encoded
//elements are sent to the server as a JSON array of Base64 strings in sorted
//order:
//
//   sort([ Base64(H(x_1)^c), Base64(H(x_2)^c), ... ])
//
//
//Server response
//
//For each encrypted element H(x)^c received from the client, the server
//encrypts it again under the commutative encryption scheme with its secret
//key s, computing (H(x)^c)^s = H(x)^(cs). The result is sent back to the
//client as a JSON array of strings in sorted order:
//
//   sort([ Base64(H(x_1)^(cs)), Base64(H(x_2)^(cs)), ... ])
//
//
//Client computes intersection
//
//The client decrypts each element received from the server's response using
//its secret key c, computing (H(x)^(cs))^(1/c) = H(x)^s. It then checks if
//each element is present in the Bloom filter, and reports the number of
//matches as the intersection size.
package server

/*
#include "psi_cardinality/c/psi_cardinality_server_c.h"
*/
import "C"
import (
	"errors"
	"fmt"
	"runtime"
)

//PSICardinalityServer context for the server side of a Private Set Intersection-Cardinality protocol.
type PSICardinalityServer struct {
	context C.psi_cardinality_server_ctx
}

//CreateWithNewKey creates and returns a new server instance with a fresh private key.
//
//Returns an error if any crypto operations fail.
func CreateWithNewKey() (*PSICardinalityServer, error) {
	psiServer := new(PSICardinalityServer)

	var err *C.char
	rcode := C.psi_cardinality_server_create_with_new_key(&psiServer.context, &err)
	if rcode != 0 {
		return nil, fmt.Errorf("failed to create server context: %v(%v)", psiServer.loadCString(&err), rcode)
	}
	if psiServer.context == nil {
		return nil, errors.New("failed to create server context: Context is NULL. This should never happen")
	}

	runtime.SetFinalizer(psiServer, func(s *PSICardinalityServer) { s.Destroy() })
	return psiServer, nil
}

//CreateFromKey creates and returns a new server instance with the provided private key.
//
//Returns an error if any crypto operations fail.
func CreateFromKey(key string) (*PSICardinalityServer, error) {
	psiServer := new(PSICardinalityServer)

	var err *C.char
	rcode := C.psi_cardinality_server_create_from_key(C.struct_server_buffer_t{
		buff:     C.CString(key),
		buff_len: C.ulong(len(key)),
	},
		&psiServer.context, &err)

	if rcode != 0 {
		return nil, fmt.Errorf("failed to create server context: %v(%v)", psiServer.loadCString(&err), rcode)
	}
	if psiServer.context == nil {
		return nil, errors.New("failed to create server context: Context is NULL. This should never happen")
	}

	runtime.SetFinalizer(psiServer, func(s *PSICardinalityServer) { s.Destroy() })
	return psiServer, nil
}

//CreateSetupMessage creates a setup message from the server's dataset to be sent to the
//client. The setup message is a JSON-encoded Bloom filter containing
//H(x)^s for each element x in `rawInput`, where s is the server's secret
//key. The message has the following form:
//
//   {
//     "num_hash_functions": <int>,
//     "bits": <string>
//   }
//
//`bits` is encoded as Base64.
//The false-positive rate `fpr` is the probability that any query of size
//`num_client_inputs` will result in a false positive.
//
//Returns an error if the context is invalid or if the encryption fails.
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
	var err *C.char
	var outlen C.ulong

	rcode := C.psi_cardinality_server_create_setup_message(s.context, C.double(fpr), C.long(inputCount), &input[0], C.ulong(len(input)), &out, &outlen, &err)

	if rcode != 0 {
		return "", fmt.Errorf("setup_message failed: %v(%v)", s.loadCString(&err), rcode)
	}
	result := s.loadCString(&out)

	return result, nil
}

//ProcessRequest processes a client query and returns the corresponding server response to
//be sent to the client.
//
//For each encrytped element H(x)^c in the decoded `client_request`, computes
//(H(x)^c)^s = H(X)^(cs) and returns these as a
//sorted JSON array. Sorting the output prevents the client from matching
//the individual response elements to the ones in the request, ensuring
//that they can only learn the intersection size but not individual
//elements in the intersection.
//
//Returns an error if the context is invalid.
func (s *PSICardinalityServer) ProcessRequest(request string) (string, error) {
	if s.context == nil {
		return "", errors.New("invalid context")
	}

	var out *C.char
	var err *C.char
	var outlen C.ulong

	rcode := C.psi_cardinality_server_process_request(s.context, C.struct_server_buffer_t{
		buff:     C.CString(request),
		buff_len: C.ulong(len(request)),
	}, &out, &outlen, &err)

	if rcode != 0 {
		return "", fmt.Errorf("process request failed: %v(%v)", s.loadCString(&err), rcode)
	}

	result := s.loadCString(&out)
	return result, nil

}

//GetPrivateKeyBytes returns this instance's private key. This key should only be used to
//create other server instances. DO NOT SEND THIS KEY TO ANY OTHER PARTY!
func (s *PSICardinalityServer) GetPrivateKeyBytes() (string, error) {
	if s.context == nil {
		return "", errors.New("invalid context")
	}

	var out *C.char
	var outlen C.ulong
	var err *C.char

	rcode := C.psi_cardinality_server_get_private_key_bytes(s.context, &out, &outlen, &err)

	if rcode != 0 {
		return "", fmt.Errorf("get private keys failed: %v(%v)", s.loadCString(&err), rcode)
	}

	result := s.loadCString(&out)

	return result, nil
}

//Destroy frees the C context.
func (s *PSICardinalityServer) Destroy() {
	if s.context == nil {
		return
	}
	C.psi_cardinality_server_delete(&s.context)
	s.context = nil
}

func (c *PSICardinalityServer) loadCString(buff **C.char) string {
	str := C.GoString(*buff)
	C.psi_cardinality_server_delete_buffer(c.context, buff)
	return str
}
