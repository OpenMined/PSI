package version

/*
#include "private_set_intersection/c/package.h"
*/
import "C"

// Version of the library.
func Version() string {
	return C.GoString(C.psi_version())
}
