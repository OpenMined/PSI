#ifndef PRIVATE_SET_INTERSECTION_JAVASCRIPT_BINDINGS_UTILS_H_
#define PRIVATE_SET_INTERSECTION_JAVASCRIPT_BINDINGS_UTILS_H_

#include <vector>

#include "emscripten/val.h"
#include "util/status.inc"

namespace private_set_intersection {

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
    result.set("Value", *statusor);
    result.set("Status", emscripten::val::null());
  } else {
    result.set("Value", emscripten::val::null());
    auto status = emscripten::val::object();
    status.set("StatusCode", statusor.status().raw_code());
    status.set("Message", statusor.status().message());
    result.set("Status", status);
  }
  return result;
}

template <typename T>
emscripten::val ToSerializedJSObject(
    private_join_and_compute::StatusOr<T> statusor) {
  auto result = emscripten::val::object();
  if (statusor.ok()) {
    const T protobuf = *statusor;
    const size_t size = protobuf.ByteSizeLong();
    std::vector<std::uint8_t> byte_vector(size);
    protobuf.SerializeToArray(byte_vector.data(), size);
    emscripten::val byte_array = emscripten::val::array(byte_vector);
    result.set("Value", byte_array);
    result.set("Status", emscripten::val::null());
  } else {
    result.set("Value", emscripten::val::null());
    auto status = emscripten::val::object();
    status.set("StatusCode", statusor.status().raw_code());
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
  return std::shared_ptr<T>(std::move(*statusor));
}

};  // namespace private_set_intersection

#endif  // PRIVATE_SET_INTERSECTION_JAVASCRIPT_BINDINGS_UTILS_H_
