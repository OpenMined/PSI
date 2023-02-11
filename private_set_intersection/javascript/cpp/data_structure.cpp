#include "emscripten/bind.h"
#include "private_set_intersection/cpp/psi_server.h"

EMSCRIPTEN_BINDINGS(PSI_DataStructure) {
  using private_set_intersection::DataStructure;
  emscripten::enum_<DataStructure>("DataStructure")
      .value("Raw", DataStructure::Raw)
      .value("GCS", DataStructure::Gcs)
      .value("BloomFilter", DataStructure::BloomFilter);
}
