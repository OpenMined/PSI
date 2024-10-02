#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <memory>
#include <vector>

#include "absl/status/statusor.h"
#include "private_set_intersection/cpp/package.h"
#include "private_set_intersection/cpp/psi_client.h"
#include "private_set_intersection/cpp/psi_server.h"
#include "private_set_intersection/proto/psi.pb.h"

namespace {
namespace psi = private_set_intersection;
namespace py = pybind11;
}  // namespace

template <class T>
T throwOrReturn(const absl::StatusOr<T>& in) {
  if (!in.ok()) {
    py::gil_scoped_acquire acquire;
    throw std::runtime_error(std::string(in.status().message()));
  }
  return *in;
}

template <class T>
auto saveProto(const T& obj) {
  return py::bytes(obj.SerializeAsString());
}

template <class T>
auto loadProto(T& obj, const py::bytes& data) {
  if (!obj.ParseFromString(data)) {
    py::gil_scoped_acquire acquire;
    throw std::invalid_argument("failed to parse proto data");
  }
}

void bind(pybind11::module& m) {
  m.doc() =
      "Private Set Intersection protocol based on ECDH and Bloom "
      "Filters";

  m.attr("__version__") = ::private_set_intersection::Package::kVersion;

  py::enum_<psi::DataStructure>(m, "data_structure", py::arithmetic())
      .value("Raw", psi::DataStructure::Raw)
      .value("GCS", psi::DataStructure::Gcs)
      .value("BloomFilter", psi::DataStructure::BloomFilter);

  py::class_<psi_proto::ServerSetup>(m, "cpp_proto_server_setup")
      .def(py::init<>())
      .def(
          "load",
          [](psi_proto::ServerSetup& obj, const py::bytes& data) {
            return loadProto(obj, data);
          },
          py::call_guard<py::gil_scoped_release>())
      .def("save",
           [](const psi_proto::ServerSetup& obj) { return saveProto(obj); })
      .def_static(
          "Load",
          [](const py::bytes& data) {
            psi_proto::ServerSetup obj;
            loadProto(obj, data);
            return obj;
          },
          py::call_guard<py::gil_scoped_release>());
  py::class_<psi_proto::Request>(m, "cpp_proto_request")
      .def(py::init<>())
      .def(
          "load",
          [](psi_proto::Request& obj, const py::bytes& data) {
            return loadProto(obj, data);
          },
          py::call_guard<py::gil_scoped_release>())
      .def("save", [](const psi_proto::Request& obj) { return saveProto(obj); })
      .def_static(
          "Load",
          [](const py::bytes& data) {
            psi_proto::Request obj;
            loadProto(obj, data);
            return obj;
          },
          py::call_guard<py::gil_scoped_release>());
  py::class_<psi_proto::Response>(m, "cpp_proto_response")
      .def(py::init<>())
      .def(
          "load",
          [](psi_proto::Response& obj, const py::bytes& data) {
            return loadProto(obj, data);
          },
          py::call_guard<py::gil_scoped_release>())
      .def("save",
           [](const psi_proto::Response& obj) { return saveProto(obj); })
      .def_static(
          "Load",
          [](const py::bytes& data) {
            psi_proto::Response obj;
            loadProto(obj, data);
            return obj;
          },
          py::call_guard<py::gil_scoped_release>());

  py::class_<psi::PsiClient>(m, "cpp_client")
      .def_static(
          "CreateWithNewKey",
          [](bool reveal_intersection) {
            auto client = psi::PsiClient::CreateWithNewKey(reveal_intersection);
            if (!client.ok()) {
              py::gil_scoped_acquire acquire;
              throw std::runtime_error(std::string(client.status().message()));
            }
            return std::move(*client);
          },
          py::call_guard<py::gil_scoped_release>())
      .def_static(
          "CreateFromKey",
          [](const std::string& key_bytes, bool reveal_intersection) {
            auto client =
                psi::PsiClient::CreateFromKey(key_bytes, reveal_intersection);
            if (!client.ok()) {
              py::gil_scoped_acquire acquire;
              throw std::runtime_error(std::string(client.status().message()));
            }
            return std::move(*client);
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "CreateRequest",
          [](const psi::PsiClient& obj,
             const std::vector<std::string>& inputs) {
            return throwOrReturn(obj.CreateRequest(absl::MakeSpan(inputs)));
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
      .def("GetPrivateKeyBytes", [](const psi::PsiClient& obj) {
        return py::bytes(obj.GetPrivateKeyBytes());
      });

  py::class_<psi::PsiServer>(m, "cpp_server")
      .def_static(
          "CreateWithNewKey",
          [](bool reveal_intersection) {
            auto server = psi::PsiServer::CreateWithNewKey(reveal_intersection);
            if (!server.ok()) {
              py::gil_scoped_acquire acquire;
              throw std::runtime_error(std::string(server.status().message()));
            }
            return std::move(*server);
          },
          py::call_guard<py::gil_scoped_release>())
      .def_static(
          "CreateFromKey",
          [](const std::string& key_bytes, bool reveal_intersection) {
            auto server =
                psi::PsiServer::CreateFromKey(key_bytes, reveal_intersection);
            if (!server.ok()) {
              py::gil_scoped_acquire acquire;
              throw std::runtime_error(std::string(server.status().message()));
            }
            return std::move(*server);
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "CreateSetupMessage",
          [](const psi::PsiServer& obj, double fpr, int64_t num_client_inputs,
             const std::vector<std::string>& inputs, psi::DataStructure ds) {
            return throwOrReturn(obj.CreateSetupMessage(
                fpr, num_client_inputs, absl::MakeSpan(inputs), ds));
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "ProcessRequest",
          [](const psi::PsiServer& obj,
             const psi_proto::Request& client_request) {
            return throwOrReturn(obj.ProcessRequest(client_request));
          },
          py::call_guard<py::gil_scoped_release>())
      .def("GetPrivateKeyBytes", [](const psi::PsiServer& obj) {
        return py::bytes(obj.GetPrivateKeyBytes());
      });
}

PYBIND11_MODULE(openmined_psi, m) { bind(m); }

PYBIND11_MODULE(_openmined_psi, m) { bind(m); }
