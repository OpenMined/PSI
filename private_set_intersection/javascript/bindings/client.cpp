#include "emscripten/bind.h"
#include "private_set_intersection/javascript/bindings/utils.h"
#include "psi_client.h"

EMSCRIPTEN_BINDINGS(PSI_Client) {
  using emscripten::optional_override;
  using private_join_and_compute::StatusOr;
  using private_set_intersection::PsiClient;
  using private_set_intersection::ToJSObject;
  using private_set_intersection::ToShared;

  emscripten::class_<PsiClient>("PsiClient")
      .smart_ptr<std::shared_ptr<PsiClient>>("std::shared_ptr<PsiClient>")
      .class_function("Create", optional_override([](bool reveal_intersection) {
                        return ToJSObject(ToShared(PsiClient::Create(reveal_intersection)));
                      }))
      .function("CreateRequest",
                optional_override([](const PsiClient &self,
                                     const emscripten::val &string_array) {
                  std::vector<std::string> string_vector;
                  const std::uint32_t l =
                      string_array["length"].as<std::uint32_t>();
                  string_vector.reserve(l);

                  for (std::uint32_t i = 0; i < l; ++i) {
                    string_vector.push_back(string_array[i].as<std::string>());
                  }

                  return ToJSObject(self.CreateRequest(string_vector));
                }))
      .function(
          "GetIntersection",
          optional_override([](const PsiClient &self, const std::string& server_setup,
                               const std::string& server_response) {
            // We need to convert to an int32 explicitly because JS
            // doesn't have 64-bit integers.
            StatusOr<std::vector<int64_t>> result;
            auto status = self.GetIntersection(server_setup, server_response);
            if (status.ok()) {
              result = status.ValueOrDie();
            } else {
              result = status.status();
            }
            return ToJSObject(result);
          }))
      .function(
          "GetIntersectionSize",
          optional_override([](const PsiClient &self, const std::string& server_setup,
                               const std::string& server_response) {
            // We need to convert to an int32 explicitly because JS
            // doesn't have 64-bit integers.
            StatusOr<int32_t> result;
            auto status = self.GetIntersectionSize(server_setup, server_response);
            if (status.ok()) {
              result = status.ValueOrDie();
            } else {
              result = status.status();
            }
            return ToJSObject(result);
          }));
}