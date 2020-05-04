#include <emscripten/bind.h>
#include "psi_cardinality/cpp/psi_cardinality_server.h"
#include "psi_cardinality/javascript/bindings/utils.h"

EMSCRIPTEN_BINDINGS(PSI_Server) {
  using emscripten::optional_override;
  using emscripten::val;
  using psi_cardinality::PSICardinalityServer;

  emscripten::class_<PSICardinalityServer>("PSICardinalityServer")
      .smart_ptr<std::shared_ptr<PSICardinalityServer>>(
          "std::shared_ptr<PSICardinalityServer>")
      .class_function(
          "CreateWithNewKey", optional_override([]() {
            std::shared_ptr<PSICardinalityServer> server =
                PSICardinalityServer::CreateWithNewKey().ValueOrDie();
            return server;
          }))
      .class_function(
          "CreateFromKey", optional_override([](const std::string &key_bytes) {
            std::shared_ptr<PSICardinalityServer> server =
                PSICardinalityServer::CreateFromKey(key_bytes).ValueOrDie();
            return server;
          }))
      .function("CreateSetupMessage",
                optional_override([](const PSICardinalityServer &self,
                                     const double fpr,
                                     const int32_t num_client_inputs,
                                     const val &v) {
            
                    std::vector<std::string> inputs;
                    const auto l = v["length"].as<unsigned>();
                    inputs.reserve(l);

                    for (auto i = 0; i < l; ++i) {
                        inputs.push_back(v[i].as<std::string>());
                    }

                    std::string message = self.CreateSetupMessage(fpr, (int64_t)num_client_inputs, inputs).ValueOrDie();
                    return message;
                }))
      .function("ProcessRequest",
                optional_override([](const PSICardinalityServer &self,
                                     const std::string &client_request) {
                  std::string response =
                      self.ProcessRequest(client_request).ValueOrDie();
                  return response;
                }))
      .function("GetPrivateKeyBytes",
                optional_override([](const PSICardinalityServer &self) {
                  std::string bytes = self.GetPrivateKeyBytes();
                  return bytes;
                }));
}