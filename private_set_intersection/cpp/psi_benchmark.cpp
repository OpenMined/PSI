#include "absl/strings/str_cat.h"
#include "benchmark/benchmark.h"
#include "private_set_intersection/cpp/psi_client.h"
#include "private_set_intersection/cpp/psi_server.h"

namespace private_set_intersection {
namespace {

void BM_ServerSetup(benchmark::State& state, double fpr,
                    bool reveal_intersection, DataStructure ds) {
  auto server = PsiServer::CreateWithNewKey(reveal_intersection).ValueOrDie();
  int num_inputs = state.range(0);
  int num_client_inputs = 10000;
  std::vector<std::string> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs[i] = absl::StrCat("Element", i);
  }
  psi_proto::ServerSetup setup;
  int64_t elements_processed = 0;
  for (auto _ : state) {
    setup = server->CreateSetupMessage(fpr, num_client_inputs, inputs, ds)
                .ValueOrDie();
    ::benchmark::DoNotOptimize(setup);
    elements_processed += num_inputs;
  }
  state.counters["SetupSize"] = benchmark::Counter(
      static_cast<double>(setup.ByteSizeLong()), benchmark::Counter::kDefaults,
      benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs, and the captured argument is the false
// positive rate for 10k client queries.
BENCHMARK_CAPTURE(BM_ServerSetup, 0.001 size gcs, 0.001, false,
                  DataStructure::GCS)
    ->RangeMultiplier(10)
    ->Range(1, 100000);
BENCHMARK_CAPTURE(BM_ServerSetup, 0.000001 size gcs, 0.000001, false,
                  DataStructure::GCS)
    ->RangeMultiplier(10)
    ->Range(1, 100000);
BENCHMARK_CAPTURE(BM_ServerSetup, 0.001 intersection gcs, 0.001, true,
                  DataStructure::GCS)
    ->RangeMultiplier(10)
    ->Range(1, 100000);
BENCHMARK_CAPTURE(BM_ServerSetup, 0.000001 intersection gcs, 0.000001, true,
                  DataStructure::GCS)
    ->RangeMultiplier(10)
    ->Range(1, 100000);
BENCHMARK_CAPTURE(BM_ServerSetup, 0.001 size bloom, 0.001, false,
                  DataStructure::BloomFilter)
    ->RangeMultiplier(10)
    ->Range(1, 100000);
BENCHMARK_CAPTURE(BM_ServerSetup, 0.000001 size bloom, 0.000001, false,
                  DataStructure::BloomFilter)
    ->RangeMultiplier(10)
    ->Range(1, 100000);
BENCHMARK_CAPTURE(BM_ServerSetup, 0.001 intersection bloom, 0.001, true,
                  DataStructure::BloomFilter)
    ->RangeMultiplier(10)
    ->Range(1, 100000);
BENCHMARK_CAPTURE(BM_ServerSetup, 0.000001 intersection bloom, 0.000001, true,
                  DataStructure::BloomFilter)
    ->RangeMultiplier(10)
    ->Range(1, 100000);

void BM_ClientCreateRequest(benchmark::State& state, bool reveal_intersection) {
  auto client = PsiClient::CreateWithNewKey(reveal_intersection).ValueOrDie();
  int num_inputs = state.range(0);
  std::vector<std::string> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs[i] = absl::StrCat("Element", i);
  }
  psi_proto::Request request;
  int64_t elements_processed = 0;
  for (auto _ : state) {
    request = client->CreateRequest(inputs).ValueOrDie();
    ::benchmark::DoNotOptimize(request);
    elements_processed += num_inputs;
  }
  state.counters["RequestSize"] = benchmark::Counter(
      static_cast<double>(request.ByteSizeLong()),
      benchmark::Counter::kDefaults, benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs.
BENCHMARK_CAPTURE(BM_ClientCreateRequest, size, false)
    ->RangeMultiplier(10)
    ->Range(1, 10000);
BENCHMARK_CAPTURE(BM_ClientCreateRequest, intersection, true)
    ->RangeMultiplier(10)
    ->Range(1, 10000);

void BM_ServerProcessRequest(benchmark::State& state,
                             bool reveal_intersection) {
  auto client = PsiClient::CreateWithNewKey(reveal_intersection).ValueOrDie();
  auto server = PsiServer::CreateWithNewKey(reveal_intersection).ValueOrDie();
  int num_inputs = state.range(0);
  std::vector<std::string> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs[i] = absl::StrCat("Element", i);
  }
  psi_proto::Request request = client->CreateRequest(inputs).ValueOrDie();
  psi_proto::Response response;
  int64_t elements_processed = 0;
  for (auto _ : state) {
    response = server->ProcessRequest(request).ValueOrDie();
    ::benchmark::DoNotOptimize(response);
    elements_processed += num_inputs;
  }
  state.counters["ResponseSize"] = benchmark::Counter(
      static_cast<double>(response.ByteSizeLong()),
      benchmark::Counter::kDefaults, benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs.
BENCHMARK_CAPTURE(BM_ServerProcessRequest, size, false)
    ->RangeMultiplier(10)
    ->Range(1, 10000);
BENCHMARK_CAPTURE(BM_ServerProcessRequest, intersection, true)
    ->RangeMultiplier(10)
    ->Range(1, 10000);

void BM_ClientProcessResponse(benchmark::State& state, bool reveal_intersection,
                              DataStructure ds, double percentClientSize) {
  auto client = PsiClient::CreateWithNewKey(reveal_intersection).ValueOrDie();
  auto server = PsiServer::CreateWithNewKey(reveal_intersection).ValueOrDie();
  int num_inputs = state.range(0);
  int num_client_inputs =
      static_cast<int>(static_cast<double>(state.range(0)) * percentClientSize);
  num_client_inputs = num_client_inputs == 0 ? 1 : num_client_inputs;
  double fpr = 1. / (1000000);
  std::vector<std::string> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs[i] = absl::StrCat("Element", i);
  }
  std::vector<std::string> client_inputs(num_client_inputs);
  for (int i = 0; i < num_client_inputs; i++) {
    client_inputs[i] = absl::StrCat("Missing", i);
  }
  psi_proto::ServerSetup setup =
      server->CreateSetupMessage(fpr, num_inputs, inputs, ds).ValueOrDie();
  psi_proto::Request request =
      client->CreateRequest(client_inputs).ValueOrDie();
  psi_proto::Response response = server->ProcessRequest(request).ValueOrDie();
  int64_t elements_processed = 0;
  for (auto _ : state) {
    if (reveal_intersection) {
      auto intersection = client->GetIntersection(setup, response).ValueOrDie();
      ::benchmark::DoNotOptimize(intersection);
    } else {
      int64_t count = client->GetIntersectionSize(setup, response).ValueOrDie();
      ::benchmark::DoNotOptimize(count);
    }
    elements_processed += num_inputs;
  }
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs.
BENCHMARK_CAPTURE(BM_ClientProcessResponse, size gcs, false, DataStructure::GCS,
                  1.0)
    ->RangeMultiplier(10)
    ->Range(1, 10000);
BENCHMARK_CAPTURE(BM_ClientProcessResponse, intersection gcs, true,
                  DataStructure::GCS, 1.0)
    ->RangeMultiplier(10)
    ->Range(1, 10000);
BENCHMARK_CAPTURE(BM_ClientProcessResponse, size bloom, false,
                  DataStructure::BloomFilter, 1.0)
    ->RangeMultiplier(10)
    ->Range(1, 10000);
BENCHMARK_CAPTURE(BM_ClientProcessResponse, intersection bloom, true,
                  DataStructure::BloomFilter, 1.0)
    ->RangeMultiplier(10)
    ->Range(1, 10000);
BENCHMARK_CAPTURE(BM_ClientProcessResponse, size gcs asymmetric, false,
                  DataStructure::GCS, 0.001)
    ->RangeMultiplier(10)
    ->Range(10000, 100000);
BENCHMARK_CAPTURE(BM_ClientProcessResponse, intersection gcs asymmetric, true,
                  DataStructure::GCS, 0.001)
    ->RangeMultiplier(10)
    ->Range(10000, 100000);
BENCHMARK_CAPTURE(BM_ClientProcessResponse, size bloom asymmetric, false,
                  DataStructure::BloomFilter, 0.001)
    ->RangeMultiplier(10)
    ->Range(10000, 100000);
BENCHMARK_CAPTURE(BM_ClientProcessResponse, intersection bloom asymmetric, true,
                  DataStructure::BloomFilter, 0.001)
    ->RangeMultiplier(10)
    ->Range(10000, 100000);

}  // namespace
}  // namespace private_set_intersection
