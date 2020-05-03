package bloomfilter

/*
#cgo LDFLAGS: -L${SRCDIR}/../extern/lib/
#cgo CFLAGS: -I${SRCDIR}/../extern/include/
#cgo LDFLAGS: -lbloom_filter -lbn_util -lcrypto -lopenssl_init -lstatus -lglog -lgflags -lstrings -linternal -lraw_logging_internal -lthrow_delegate -lint128

#include "bloom_filter_c.h"
*/
import "C"
import (
	"errors"
)

//BloomFilter context
type BloomFilter struct {
	context C.bloom_filter_ctx
}

//Create returns a new BloomFilter
func Create(fpr float64, maxElements int64) (*BloomFilter, error) {
	bf := new(BloomFilter)
	bf.context = C.bloom_filter_create(C.double(fpr), C.long(maxElements))
	if bf.context == nil {
		return nil, errors.New("failed to init bloom filter")
	}
	return bf, nil
}

//CreateFromJSON returns a new BloomFilter from a JSON
func CreateFromJSON(encoded string) (*BloomFilter, error) {
	bf := new(BloomFilter)
	bf.context = C.bloom_filter_create_from_json(C.CString(encoded))
	if bf.context == nil {
		return nil, errors.New("failed to init bloom filter")
	}
	return bf, nil
}

//Destroy cleans up the context
func (b *BloomFilter) Destroy() error {
	if b.context == nil {
		return errors.New("Destroy on nil context")
	}
	C.bloom_filter_delete(&b.context)
	b.context = nil

	return nil
}

//Add inserts a new element in the bloom filter
func (b *BloomFilter) Add(input string) error {
	if b.context == nil {
		return errors.New("Add on nil context")
	}

	C.bloom_filter_add(b.context, C.struct_bloom_buffer_t{
		buff:     C.CString(input),
		buff_len: C.ulong(len(input)),
	})
	return nil
}

//AddArray inserts every element in an array in the bloom filter
func (b *BloomFilter) AddArray(rawInput []string) error {
	if b.context == nil {
		return errors.New("Add on nil context")
	}

	input := []C.struct_bloom_buffer_t{}
	for idx := range rawInput {
		input = append(input, C.struct_bloom_buffer_t{
			buff:     C.CString(rawInput[idx]),
			buff_len: C.ulong(len(rawInput[idx])),
		})
	}
	C.bloom_filter_add_array(b.context, &input[0], C.ulong(len(input)))
	return nil
}

//Check checks for an element in the BloomFilter
func (b *BloomFilter) Check(input string) (bool, error) {
	if b.context == nil {
		return false, errors.New("Check on nil context")
	}

	found := C.bloom_filter_check(b.context, C.struct_bloom_buffer_t{
		buff:     C.CString(input),
		buff_len: C.ulong(len(input)),
	})
	res := false
	if found != 0 {
		res = true
	}
	return res, nil
}

//ToJSON dumps the BloomFilter to a JSON string
func (b *BloomFilter) ToJSON() (string, error) {
	if b.context == nil {
		return "", errors.New("ToJSON on nil context")
	}

	var out *C.char
	var outlen C.ulong

	C.bloom_filter_to_json(b.context, &out, &outlen)

	return C.GoString(out), nil
}

//NumHashFunctions returns the number of hash functions of the BloomFilter
func (b *BloomFilter) NumHashFunctions() int {
	if b.context == nil {
		return 0
	}

	return int(C.bloom_filter_num_hash_functions(b.context))
}
