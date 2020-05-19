#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <memory>
#include <vector>

#include "bloom_filter.h"
#include "psi_cardinality_client.h"
#include "psi_cardinality_server.h"
#include "util/canonical_errors.h"
#include "util/status_macros.h"

namespace psi_cardinality {

using namespace std;
namespace psi = psi_cardinality;
namespace py = pybind11;

template <class T>
T throwOrReturn(const StatusOr<T>& in) {
  if (!in.ok()) throw std::runtime_error(in.status().message());
  return in.ValueOrDie();
}

PYBIND11_MODULE(_psi_cardinality_cpp, m) {
  m.doc() =
      "Private Set Intersection Cardinality protocol based on ECDH and Bloom "
      "Filters";

  py::class_<psi::PSICardinalityClient>(m, "PSICardinalityClient")
      .def_static("Create",
                  []() {
                    auto client = psi::PSICardinalityClient::Create();
                    if (!client.ok())
                      throw std::runtime_error(client.status().message());
                    return std::move(client.ValueOrDie());
                  })
      .def("CreateRequest",
           [](const psi::PSICardinalityClient& obj,
              const std::vector<std::string>& inputs) {
             return throwOrReturn(obj.CreateRequest(inputs));
           })
      .def("ProcessResponse", [](const psi::PSICardinalityClient& obj,
                                 const std::string& server_setup,
                                 const std::string& server_response) {
        return throwOrReturn(
            obj.ProcessResponse(server_setup, server_response));
      });

  py::class_<psi::PSICardinalityServer>(m, "PSICardinalityServer")
      .def_static("CreateWithNewKey",
                  []() {
                    auto server = psi::PSICardinalityServer::CreateWithNewKey();
                    if (!server.ok())
                      throw std::runtime_error(server.status().message());
                    return std::move(server.ValueOrDie());
                  })
      .def_static("CreateFromKey",
                  [](const std::string& key_bytes) {
                    auto server =
                        psi::PSICardinalityServer::CreateFromKey(key_bytes);
                    if (!server.ok())
                      throw std::runtime_error(server.status().message());
                    return std::move(server.ValueOrDie());
                  })
      .def("CreateSetupMessage",
           [](const psi::PSICardinalityServer& obj, double fpr,
              int64_t num_client_inputs,
              const std::vector<std::string>& inputs) {
             return throwOrReturn(
                 obj.CreateSetupMessage(fpr, num_client_inputs, inputs));
           })
      .def("ProcessRequest",
           [](const psi::PSICardinalityServer& obj,
              const std::string& client_request) {
             return throwOrReturn(obj.ProcessRequest(client_request));
           })
      .def("GetPrivateKeyBytes",
           &psi::PSICardinalityServer::GetPrivateKeyBytes);
}
}  // namespace psi_cardinality
