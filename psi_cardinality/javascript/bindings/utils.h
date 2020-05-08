#ifndef PSI_CARDINALITY_JAVASCRIPT_BINDINGS_UTILS_H_
#define PSI_CARDINALITY_JAVASCRIPT_BINDINGS_UTILS_H_

#include <emscripten/val.h>

#include <vector>

#include "util/statusor.h"

namespace psi_cardinality {

// Converts a StatusOr<T> to a Javascript object with the following structure:
// {
//   Value: T
//   Status: {
//     StatusCode: int,
//     Message: String
//   }
// }
//
// Exactly one of `Value` or `Status` is NULL.
template <typename T>
emscripten::val ToJSObject(private_join_and_compute::StatusOr<T> statusor) {
  auto result = emscripten::val::object();
  if (statusor.ok()) {
    result.set("Value", statusor.ValueOrDie());
    result.set("Status", emscripten::val::null());
  } else {
    result.set("Value", emscripten::val::null());
    auto status = emscripten::val::object();
    status.set("StatusCode", statusor.status().error_code());
    status.set("Message", statusor.status().message());
    result.set("Status", status);
  }
  return result;
}

// Converts a StatusOr<std::unique_ptr<T>> to a StatusOr<std::shared_ptr<T>>,
// taking ownership of the object pointed to.
template <typename T>
private_join_and_compute::StatusOr<std::shared_ptr<T>> ToShared(
    private_join_and_compute::StatusOr<std::unique_ptr<T>> statusor) {
  if (!statusor.ok()) {
    return statusor.status();
  }
  return std::shared_ptr<T>(std::move(statusor.ValueOrDie()));
}

};  // namespace psi_cardinality

#endif  // PSI_CARDINALITY_JAVASCRIPT_BINDINGS_UTILS_H_
