#include <emscripten/bind.h>
#include "psi_cardinality/cpp/psi_cardinality_server.h"
#include "psi_cardinality/javascript/bindings/utils.h"
#include <iostream>

EMSCRIPTEN_BINDINGS(PSI_Server) {
  using emscripten::optional_override;
  using psi_cardinality::PSICardinalityServer;
  using psi_cardinality::ToJSObject;
  using psi_cardinality::ToShared;

  emscripten::class_<PSICardinalityServer>("PSICardinalityServer")
      .smart_ptr<std::shared_ptr<PSICardinalityServer>>(
          "std::shared_ptr<PSICardinalityServer>")
      .class_function("CreateWithNewKey", optional_override([]() {
                        return ToJSObject(
                            ToShared(PSICardinalityServer::CreateWithNewKey()));
                      }))
      .class_function(
          "CreateFromKey", optional_override([](const emscripten::val &byte_array) {

            std::vector<std::uint8_t> byte_vector;

            const auto l = byte_array["length"].as<unsigned>();
            byte_vector.reserve(l);

            for (auto i = 0; i < l; ++i) {
              std::uint8_t byte = byte_array[i].as<std::uint8_t>();
              byte_vector.push_back(byte);
            }
              
            const std::string key(byte_vector.begin(), byte_vector.end());

            return ToJSObject(
                ToShared(PSICardinalityServer::CreateFromKey(key)));
          }))
      .function(
          "CreateSetupMessage",
          optional_override(
              [](const PSICardinalityServer &self, const double fpr,
                 const int32_t num_client_inputs, const emscripten::val &v) {
                std::vector<std::string> inputs;
                const auto l = v["length"].as<unsigned>();
                inputs.reserve(l);

                for (auto i = 0; i < l; ++i) {
                  inputs.push_back(v[i].as<std::string>());
                }
                return ToJSObject(
                    self.CreateSetupMessage(fpr, num_client_inputs, inputs));
              }))
      .function("ProcessRequest",
                optional_override([](const PSICardinalityServer &self,
                                     const std::string &client_request) {
                  return ToJSObject(self.ProcessRequest(client_request));
                }))
      .function("GetPrivateKeyBytes",
                optional_override([](const PSICardinalityServer &self) {
                  const std::string bytes = self.GetPrivateKeyBytes();
                  const std::vector<std::uint8_t> key_vector(bytes.begin(), bytes.end());
                  emscripten::val key_bytes  = emscripten::val::array(key_vector.begin(), key_vector.end());
                  return key_bytes;
                }));
}