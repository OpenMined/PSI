#include "emscripten/bind.h"
#include "private_set_intersection/cpp/psi_server.h"
#include "private_set_intersection/javascript/bindings/utils.h"

EMSCRIPTEN_BINDINGS(PSI_Server) {
  using emscripten::optional_override;
  using private_set_intersection::PsiServer;
  using private_set_intersection::ToJSObject;
  using private_set_intersection::ToShared;

  emscripten::class_<PsiServer>("PsiServer")
      .smart_ptr<std::shared_ptr<PsiServer>>(
          "std::shared_ptr<PsiServer>")
      .class_function("CreateWithNewKey", optional_override([]() {
                        return ToJSObject(
                            ToShared(PsiServer::CreateWithNewKey()));
                      }))
      .class_function(
          "CreateFromKey",
          optional_override([](const emscripten::val &byte_array) {
            const std::uint32_t l = byte_array["length"].as<std::uint32_t>();
            std::string byte_string(l, '\0');

            for (std::uint32_t i = 0; i < l; i++) {
              byte_string[i] = byte_array[i].as<std::uint8_t>();
            }

            return ToJSObject(
                ToShared(PsiServer::CreateFromKey(byte_string)));
          }))
      .function("CreateSetupMessage",
                optional_override([](const PsiServer &self,
                                     const double fpr,
                                     const int32_t num_client_inputs,
                                     const emscripten::val &string_array) {
                  std::vector<std::string> string_vector;
                  const std::uint32_t l =
                      string_array["length"].as<std::uint32_t>();
                  string_vector.reserve(l);

                  for (std::uint32_t i = 0; i < l; ++i) {
                    string_vector.push_back(string_array[i].as<std::string>());
                  }

                  return ToJSObject(self.CreateSetupMessage(
                      fpr, num_client_inputs, string_vector));
                }))
      .function("ProcessRequest",
                optional_override([](const PsiServer &self,
                                     const std::string &client_request) {
                  return ToJSObject(self.ProcessRequest(client_request));
                }))
      .function("GetPrivateKeyBytes",
                optional_override([](const PsiServer &self) {
                  const std::string byte_string = self.GetPrivateKeyBytes();
                  const std::vector<std::uint8_t> byte_vector(
                      byte_string.begin(), byte_string.end());
                  emscripten::val byte_array = emscripten::val::array(
                      byte_vector.begin(), byte_vector.end());
                  return byte_array;
                }));
}