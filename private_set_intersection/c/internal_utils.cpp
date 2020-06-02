#include "internal_utils.h"

#include <string.h>

namespace private_set_intersection {
namespace c_bindings_internal {
int generate_error(private_join_and_compute::Status status, char **error_out) {
  if (error_out != NULL) {
    size_t error_length = status.message().size();
    *error_out =
        reinterpret_cast<char *>(malloc((error_length + 1) * sizeof(char)));
    strncpy(*error_out, status.message().c_str(), error_length + 1);
  }
  return status.error_code();
}
}  // namespace c_bindings_internal
}  // namespace private_set_intersection
