#include <emscripten/bind.h>
#include "psi_cardinality/cpp/psi_cardinality_client.h"

EMSCRIPTEN_BINDINGS(PSI_Client) {
  using emscripten::optional_override;
  using psi_cardinality::PSICardinalityClient;

  emscripten::class_<PSICardinalityClient>("PSICardinalityClient")
      .smart_ptr<std::shared_ptr<PSICardinalityClient>>(
          "std::shared_ptr<PSICardinalityClient>")
      .class_function("Create", optional_override([]() {
                        std::shared_ptr<PSICardinalityClient> client =
                            PSICardinalityClient::Create().ValueOrDie();
                        return client;
                      }))
      .function("CreateRequest",
                optional_override([](const PSICardinalityClient &self,
                                     const std::vector<std::string> &vect) {
                  std::string request = self.CreateRequest(vect).ValueOrDie();
                  return request;
                }))
      .function("ProcessResponse",
                optional_override([](const PSICardinalityClient &self,
                                     const std::string &setup,
                                     const std::string &response) {
                  std::int64_t result =
                      self.ProcessResponse(setup, response).ValueOrDie();
                  const std::int32_t intersection = (int32_t)result;
                  return intersection;
                }));
}