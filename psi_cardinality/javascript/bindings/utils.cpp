#include "utils.h"
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>

EMSCRIPTEN_BINDINGS(PSICardinality_Utils) {
  emscripten::function(
      "VecFromJSArray",
      emscripten::select_overload<std::vector<std::string>(
          const emscripten::val &)>(&psi_cardinality::VecFromJSArray));

  emscripten::register_vector<std::string>("std::vector<std::string>");
}