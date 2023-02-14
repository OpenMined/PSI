package datastructure

/*
#include "private_set_intersection/cpp/datastructure/datastructure.h"
#include <stdlib.h>
*/
import "C"

// DataStructure enum type used to specify which backing datastructure to use
type DataStructure int

// Golang's way to define enums that are compatible with our C bindings
const (
	Raw         DataStructure = C.Raw
	Gcs                       = C.Gcs
	BloomFilter               = C.BloomFilter
)

func (ds DataStructure) String() string {
	switch ds {
	case Raw:
		return "raw"
	case Gcs:
		return "gcs"
	case BloomFilter:
		return "bloomfilter"
	default:
		panic("impossible")
	}
}
