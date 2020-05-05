#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <seal/seal.h>

#include <memory>
#include <vector>

#include "bloom_filter.h"
#include "psi_cardinality_client.h"
#include "psi_cardinality_server.h"

using namespace std;
namespace psi = psi_cardinality;
namespace py = pybind11;

PYBIND11_MODULE(_psi_cardinality_cpp, m) {
    m.doc() =
        "Private Set Intersection Cardinality protocol based on ECDH and Bloom "
        "Filters";

    py::class_<psi::PSICardinalityClient>(m, "PSICardinalityClient")
        .def("Create", &psi::PSICardinalityClient::Create)
        .def("CreateRequest", &psi::PSICardinalityClient::CreateRequest)
        .def("ProcessResponse", &psi::PSICardinalityClient::ProcessResponse);

    py::class_<psi::PSICardinalityServer>(m, "PSICardinalityServer")
        .def("CreateWithNewKey", &psi::PSICardinalityServer::CreateWithNewKey)
        .def("CreateFromKey", &psi::PSICardinalityServer::CreateFromKey)
        .def("CreateSetupMessage",
             &psi::PSICardinalityServer::CreateSetupMessage)
        .def("ProcessRequest", &psi::PSICardinalityServer::ProcessRequest)
        .def("GetPrivateKeyBytes",
             &psi::PSICardinalityServer::GetPrivateKeyBytes);

    py::class_<psi::BloomFilter>(m, "BloomFilter")
        .def("Createconst std::string&", &psi::BloomFilter::Create)
        .def("CreateFromJSON", &psi::BloomFilter::CreateFromJSON)
        .def("Add",
             py::overload_cast<const std::string&>(&psi::BloomFilter::Add))
        .def("Add", py::overload_cast<absl::Span<const std::string>>(
                        &psi::BloomFilter::Add))
        .def("Check", &psi::BloomFilter::Check)
        .def("ToJSON", &psi::BloomFilter::ToJSON);
}