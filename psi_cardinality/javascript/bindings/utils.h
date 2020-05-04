#ifndef PSI_CARDINALITY_JAVASCRIPT_BINDINGS_UTILS_H_
#define PSI_CARDINALITY_JAVASCRIPT_BINDINGS_UTILS_H_

#include <emscripten/val.h>
#include <vector>

namespace psi_cardinality {

template <typename T>
std::vector<T> VecFromJSArray(const emscripten::val &v) {
  auto l = v["length"].as<unsigned>();

  std::vector<T> rv;
  for (unsigned i = 0; i < l; ++i) {
    rv.push_back(v[i].as<T>());
  }

  return rv;
};

};  // namespace psi_cardinality

#endif  // PSI_CARDINALITY_JAVASCRIPT_BINDINGS_UTILS_H_
