#include <emscripten/bind.h>
#include "psi_cardinality/cpp/psi_cardinality_client.h"
#include "psi_cardinality/javascript/bindings/utils.h"

EMSCRIPTEN_BINDINGS(PSI_Client) {
  using emscripten::optional_override;
  using private_join_and_compute::StatusOr;
  using psi_cardinality::PSICardinalityClient;
  using psi_cardinality::ToJSObject;
  using psi_cardinality::ToShared;

  emscripten::class_<PSICardinalityClient>("PSICardinalityClient")
      .smart_ptr<std::shared_ptr<PSICardinalityClient>>(
          "std::shared_ptr<PSICardinalityClient>")
      .class_function(
          "Create", optional_override([]() {
            return ToJSObject(ToShared(PSICardinalityClient::Create()));
          }))
      .function("CreateRequest",
                optional_override([](const PSICardinalityClient &self,
                                     const emscripten::val &string_array) {
                  std::vector<std::string> string_vector;
                  const std::uint32_t l = string_array["length"].as<std::uint32_t>();
                  string_vector.reserve(l);

                  for (std::uint32_t i = 0; i < l; ++i) {
                    string_vector.push_back(string_array[i].as<std::string>());
                  }

                  return ToJSObject(self.CreateRequest(string_vector));
                }))
      .function("ProcessResponse",
                optional_override([](const PSICardinalityClient &self,
                                     const std::string &setup,
                                     const std::string &response) {
                  // We need to convert to an int32 explicitly because JS
                  // doesn't have 64-bit integers.
                  StatusOr<int32_t> result;
                  auto status = self.ProcessResponse(setup, response);
                  if (status.ok()) {
                    result = status.ValueOrDie();
                  } else {
                    result = status.status();
                  }
                  return ToJSObject(result);
                }));
}