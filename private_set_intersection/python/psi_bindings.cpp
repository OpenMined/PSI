#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <memory>
#include <vector>

#include "private_set_intersection/cpp/package.h"
#include "private_set_intersection/cpp/psi_client.h"
#include "private_set_intersection/cpp/psi_server.h"
#include "private_set_intersection/proto/psi.pb.h"
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
  py::class_<psi_proto::ServerSetup>(m, "PsiProtoServerSetup")
      .def("bits", &psi_proto::ServerSetup::bits)
      .def("set_bits", py::overload_cast<const std::string&>(
                           &psi_proto::ServerSetup::set_bits))
      .def("set_bits",
           py::overload_cast<const char*>(&psi_proto::ServerSetup::set_bits))
      .def("set_bits", py::overload_cast<const void*, size_t>(
                           &psi_proto::ServerSetup::set_bits))
      .def("clear_bits", &psi_proto::ServerSetup::clear_bits)
      .def("num_hash_functions", &psi_proto::ServerSetup::num_hash_functions)
      .def("set_num_hash_functions",
           &psi_proto::ServerSetup::set_num_hash_functions)
      .def("clear_num_hash_functions",
           &psi_proto::ServerSetup::clear_num_hash_functions);
  py::class_<psi_proto::Request>(m, "PsiProtoRequest")
      .def("encrypted_elements_size",
           &psi_proto::Request::encrypted_elements_size)
      .def("encrypted_elements",
           py::overload_cast<int>(&psi_proto::Request::encrypted_elements,
                                  py::const_))
      .def("add_encrypted_elements",
           py::overload_cast<const std::string&>(
               &psi_proto::Request::add_encrypted_elements))
      .def("clear_encrypted_elements",
           &psi_proto::Request::clear_encrypted_elements)
      .def("reveal_intersection", &psi_proto::Request::reveal_intersection)
      .def("set_reveal_intersection",
           &psi_proto::Request::set_reveal_intersection)
      .def("clear_reveal_intersection",
           &psi_proto::Request::clear_reveal_intersection);
  py::class_<psi_proto::Response>(m, "PsiProtoResponse")
      .def("encrypted_elements_size",
           &psi_proto::Response::encrypted_elements_size)
      .def("encrypted_elements",
           py::overload_cast<int>(&psi_proto::Response::encrypted_elements,
                                  py::const_))
      .def("add_encrypted_elements",
           py::overload_cast<const std::string&>(
               &psi_proto::Response::add_encrypted_elements))
      .def("clear_encrypted_elements",
           &psi_proto::Response::clear_encrypted_elements);

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
          [](const psi::PsiClient& obj,
             const psi_proto::ServerSetup& server_setup,
             const psi_proto::Response& server_response) {
            return throwOrReturn(
                obj.GetIntersection(server_setup, server_response));
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "GetIntersectionSize",
          [](const psi::PsiClient& obj,
             const psi_proto::ServerSetup& server_setup,
             const psi_proto::Response& server_response) {
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
          [](const psi::PsiServer& obj,
             const psi_proto::Request& client_request) {
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
