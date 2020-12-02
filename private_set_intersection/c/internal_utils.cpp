#include "internal_utils.h"

#include <string.h>

namespace private_set_intersection {
namespace c_bindings_internal {
int generate_error(absl::Status status, char **error_out) {
  if (error_out != NULL) {
    size_t error_length = status.message().size();
    *error_out =
        reinterpret_cast<char *>(malloc((error_length + 1) * sizeof(char)));
    strncpy(*error_out, status.message().data(), error_length + 1);
  }
  return status.raw_code();
}
}  // namespace c_bindings_internal
}  // namespace private_set_intersection
