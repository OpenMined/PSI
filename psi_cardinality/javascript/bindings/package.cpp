#include "psi_cardinality/cpp/package.h"

#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(Package) {
  emscripten::class_<psi_cardinality::Package>("Package")
      // Using class_function instead of class_property
      // because emscripten cannot understand a static constexpr char[]
      .class_function("version", emscripten::optional_override([]() {
                        const std::string version =
                            psi_cardinality::Package::kVersion;
                        return version;
                      }));
}