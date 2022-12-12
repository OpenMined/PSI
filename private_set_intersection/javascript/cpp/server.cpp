#include "emscripten/bind.h"
#include "private_set_intersection/cpp/psi_server.h"
#include "private_set_intersection/javascript/cpp/utils.h"
#include "private_set_intersection/proto/psi.pb.h"

EMSCRIPTEN_BINDINGS(PSI_Server) {
  using absl::StatusOr;
  using emscripten::optional_override;
  using private_set_intersection::DataStructure;
  using private_set_intersection::PsiServer;
  using private_set_intersection::ToJSObject;
  using private_set_intersection::ToSerializedJSObject;
  using private_set_intersection::ToShared;

  emscripten::class_<PsiServer>("PsiServer")
      .smart_ptr<std::shared_ptr<PsiServer>>("std::shared_ptr<PsiServer>")
      .class_function(
          "CreateWithNewKey", optional_override([](bool reveal_intersection) {
            return ToJSObject(
                ToShared(PsiServer::CreateWithNewKey(reveal_intersection)));
          }))
      .class_function("CreateFromKey",
                      optional_override([](const emscripten::val &byte_array,
                                           bool reveal_intersection) {
                        const std::size_t l =
                            byte_array["length"].as<std::size_t>();
                        std::string byte_string(l, '\0');

                        for (std::size_t i = 0; i < l; i++) {
                          byte_string[i] = byte_array[i].as<std::uint8_t>();
                        }

                        return ToJSObject(ToShared(PsiServer::CreateFromKey(
                            byte_string, reveal_intersection)));
                      }))
      .function("CreateSetupMessage",
                optional_override([](const PsiServer &self, const double fpr,
                                     const std::size_t num_client_inputs,
                                     const emscripten::val &byte_array,
                                     const DataStructure ds) {
                  std::vector<std::string> string_vector;
                  const std::size_t l = byte_array["length"].as<std::size_t>();
                  string_vector.reserve(l);

                  for (std::size_t i = 0; i < l; ++i) {
                    string_vector.push_back(byte_array[i].as<std::string>());
                  }

                  StatusOr<psi_proto::ServerSetup> server_setup;
                  const auto status = self.CreateSetupMessage(
                      fpr, num_client_inputs, string_vector, ds);
                  if (status.ok()) {
                    server_setup = *status;
                  } else {
                    server_setup = status.status();
                  }
                  return ToSerializedJSObject(server_setup);
                }))
      .function("ProcessRequest",
                optional_override([](const PsiServer &self,
                                     const emscripten::val &byte_array) {
                  const std::size_t l = byte_array["length"].as<std::size_t>();
                  std::string byte_string(l, '\0');

                  for (std::size_t i = 0; i < l; i++) {
                    byte_string[i] = byte_array[i].as<std::uint8_t>();
                  }

                  psi_proto::Request client_request;
                  client_request.ParseFromString(byte_string);

                  StatusOr<psi_proto::Response> response;
                  const auto status = self.ProcessRequest(client_request);
                  if (status.ok()) {
                    response = *status;
                  } else {
                    response = status.status();
                  }
                  return ToSerializedJSObject(response);
                }))
      .function(
          "GetPrivateKeyBytes", optional_override([](const PsiServer &self) {
            const std::string byte_string = self.GetPrivateKeyBytes();
            const std::vector<std::uint8_t> byte_vector(byte_string.begin(),
                                                        byte_string.end());
            emscripten::val byte_array =
                emscripten::val::array(byte_vector.begin(), byte_vector.end());
            return byte_array;
          }));
}