#include <emscripten/bind.h>
#include "psi_cardinality/cpp/psi_cardinality_server.h"
#include "psi_cardinality/javascript/bindings/utils.h"

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
          "CreateFromKey", optional_override([](const std::string &key_bytes) {
            return ToJSObject(
                ToShared(PSICardinalityServer::CreateFromKey(key_bytes)));
          }))
      .function("CreateSetupMessage",
                optional_override([](const PSICardinalityServer &self,
                                     const double fpr,
                                     const int32_t num_client_inputs,
                                     const std::vector<std::string> &inputs) {
                  return ToJSObject(self.CreateSetupMessage(
                      fpr, (int64_t)num_client_inputs, inputs));
                }))
      .function("ProcessRequest",
                optional_override([](const PSICardinalityServer &self,
                                     const std::string &client_request) {
                  return ToJSObject(self.ProcessRequest(client_request));
                }))
      .function("GetPrivateKeyBytes",
                optional_override([](const PSICardinalityServer &self) {
                  std::string bytes = self.GetPrivateKeyBytes();
                  return bytes;
                }));
}