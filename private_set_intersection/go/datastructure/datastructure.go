package datastructure

/*
#include "private_set_intersection/cpp/datastructure/datastructure.h"
#include <stdlib.h>
*/
import "C"

type DataStructure int

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
