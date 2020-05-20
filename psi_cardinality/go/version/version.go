package version

/*
#include "psi_cardinality/c/package.h"
*/
import "C"
import ()

//Version of the library
func Version() string {
	return C.GoString(C.psi_cardinality_version())
}
