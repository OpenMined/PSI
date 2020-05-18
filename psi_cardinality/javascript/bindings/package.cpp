#include <emscripten/bind.h>

#include "psi_cardinality/cpp/package.h"

EMSCRIPTEN_BINDINGS(Package) {
  using emscripten::optional_override;
  using psi_cardinality::Package;

  emscripten::class_<Package>("Package")
      // Using class_function instead of class_property
      // because emscripten cannot understand a static constexpr char[]
      .class_function(
        "version", optional_override([]() {
          const std::string version = Package::version;
          return version;
        }));
}