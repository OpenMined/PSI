#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <memory>
#include <vector>

#include "private_set_intersection/cpp/package.h"
#include "private_set_intersection/cpp/psi_client.h"
#include "private_set_intersection/cpp/psi_server.h"
#include "util/canonical_errors.h"
#include "util/status_macros.h"

namespace {
namespace psi = private_set_intersection;
namespace py = pybind11;
}  // namespace

template <class T>
T throwOrReturn(const private_join_and_compute::StatusOr<T>& in) {
  if (!in.ok()) throw std::runtime_error(in.status().message());
  return in.ValueOrDie();
}

PYBIND11_MODULE(_psi_bindings, m) {
  m.doc() =
      "Private Set Intersection protocol based on ECDH and Bloom "
      "Filters";

  m.attr("__version__") = ::private_set_intersection::Package::kVersion;
  py::class_<psi::PsiClient>(m, "PsiClient")
      .def_static(
          "CreateWithNewKey",
          [](bool reveal_intersection) {
            auto client = psi::PsiClient::CreateWithNewKey(reveal_intersection);
            if (!client.ok())
              throw std::runtime_error(client.status().message());
            return std::move(client.ValueOrDie());
          },
          py::call_guard<py::gil_scoped_release>())
      .def_static(
          "CreateFromKey",
          [](const std::string& key_bytes, bool reveal_intersection) {
            auto client =
                psi::PsiClient::CreateFromKey(key_bytes, reveal_intersection);
            if (!client.ok())
              throw std::runtime_error(client.status().message());
            return std::move(client.ValueOrDie());
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "CreateRequest",
          [](const psi::PsiClient& obj,
             const std::vector<std::string>& inputs) {
            return throwOrReturn(obj.CreateRequest(inputs));
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "GetIntersection",
          [](const psi::PsiClient& obj, const std::string& server_setup,
             const std::string& server_response) {
            return throwOrReturn(
                obj.GetIntersection(server_setup, server_response));
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "GetIntersectionSize",
          [](const psi::PsiClient& obj, const std::string& server_setup,
             const std::string& server_response) {
            return throwOrReturn(
                obj.GetIntersectionSize(server_setup, server_response));
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "GetPrivateKeyBytes",
          [](const psi::PsiClient& obj) {
            return py::bytes(obj.GetPrivateKeyBytes());
          },
          py::call_guard<py::gil_scoped_release>());

  py::class_<psi::PsiServer>(m, "PsiServer")
      .def_static(
          "CreateWithNewKey",
          [](bool reveal_intersection) {
            auto server = psi::PsiServer::CreateWithNewKey(reveal_intersection);
            if (!server.ok())
              throw std::runtime_error(server.status().message());
            return std::move(server.ValueOrDie());
          },
          py::call_guard<py::gil_scoped_release>())
      .def_static(
          "CreateFromKey",
          [](const std::string& key_bytes, bool reveal_intersection) {
            auto server =
                psi::PsiServer::CreateFromKey(key_bytes, reveal_intersection);
            if (!server.ok())
              throw std::runtime_error(server.status().message());
            return std::move(server.ValueOrDie());
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "CreateSetupMessage",
          [](const psi::PsiServer& obj, double fpr, int64_t num_client_inputs,
             const std::vector<std::string>& inputs) {
            return throwOrReturn(
                obj.CreateSetupMessage(fpr, num_client_inputs, inputs));
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "ProcessRequest",
          [](const psi::PsiServer& obj, const std::string& client_request) {
            return throwOrReturn(obj.ProcessRequest(client_request));
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "GetPrivateKeyBytes",
          [](const psi::PsiServer& obj) {
            return py::bytes(obj.GetPrivateKeyBytes());
          },
          py::call_guard<py::gil_scoped_release>());
}
