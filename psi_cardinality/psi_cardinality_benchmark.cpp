#include "absl/strings/str_cat.h"
#include "benchmark/benchmark.h"
#include "psi_cardinality/psi_cardinality_client.h"
#include "psi_cardinality/psi_cardinality_server.h"

namespace psi_cardinality {
namespace {

void BM_ServerSetup(benchmark::State& state, double fpr) {
  auto server = PSICardinalityServer::CreateWithNewkey().ValueOrDie();
  int num_inputs = state.range(0);
  std::vector<std::string> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs[i] = absl::StrCat("Element", i);
  }
  std::string setup;
  int64_t elements_processed = 0;
  for (auto _ : state) {
    setup = server->CreateSetupMessage(fpr, inputs).ValueOrDie();
    ::benchmark::DoNotOptimize(setup);
    elements_processed += num_inputs;
  }
  state.counters["SetupSize"] = benchmark::Counter(
      static_cast<double>(setup.size()), benchmark::Counter::kDefaults,
      benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs, and the captured argument is the false
// positive rate.
BENCHMARK_CAPTURE(BM_ServerSetup, fpr = 0.001, 0.001)
    ->RangeMultiplier(100)
    ->Range(1, 1000000);
BENCHMARK_CAPTURE(BM_ServerSetup, fpr = 0.000001, 0.000001)
    ->RangeMultiplier(100)
    ->Range(1, 1000000);

void BM_ClientCreateRequest(benchmark::State& state) {
  auto client = PSICardinalityClient::Create().ValueOrDie();
  int num_inputs = state.range(0);
  std::vector<std::string> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs[i] = absl::StrCat("Element", i);
  }
  std::string request;
  int64_t elements_processed = 0;
  for (auto _ : state) {
    request = client->CreateRequest(inputs).ValueOrDie();
    ::benchmark::DoNotOptimize(request);
    elements_processed += num_inputs;
  }
  state.counters["RequestSize"] = benchmark::Counter(
      static_cast<double>(request.size()), benchmark::Counter::kDefaults,
      benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs.
BENCHMARK(BM_ClientCreateRequest)->RangeMultiplier(10)->Range(1, 10000);

void BM_ServerProcessRequest(benchmark::State& state) {
  auto client = PSICardinalityClient::Create().ValueOrDie();
  auto server = PSICardinalityServer::CreateWithNewkey().ValueOrDie();
  int num_inputs = state.range(0);
  std::vector<std::string> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs[i] = absl::StrCat("Element", i);
  }
  std::string request = client->CreateRequest(inputs).ValueOrDie();
  std::string response;
  int64_t elements_processed = 0;
  for (auto _ : state) {
    response = server->ProcessRequest(request).ValueOrDie();
    ::benchmark::DoNotOptimize(response);
    elements_processed += num_inputs;
  }
  state.counters["ResponseSize"] = benchmark::Counter(
      static_cast<double>(response.size()), benchmark::Counter::kDefaults,
      benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs.
BENCHMARK(BM_ServerProcessRequest)->RangeMultiplier(10)->Range(1, 10000);

void BM_ClientProcessResponse(benchmark::State& state) {
  auto client = PSICardinalityClient::Create().ValueOrDie();
  auto server = PSICardinalityServer::CreateWithNewkey().ValueOrDie();
  int num_inputs = state.range(0);
  double fpr = 1. / (1000000);
  std::vector<std::string> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs[i] = absl::StrCat("Element", i);
  }
  std::string setup = server->CreateSetupMessage(fpr, inputs).ValueOrDie();
  std::string request = client->CreateRequest(inputs).ValueOrDie();
  std::string response = server->ProcessRequest(request).ValueOrDie();
  int64_t elements_processed = 0;
  for (auto _ : state) {
    int64_t count = client->ProcessResponse(setup, response).ValueOrDie();
    ::benchmark::DoNotOptimize(count);
    elements_processed += num_inputs;
  }
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs.
BENCHMARK(BM_ClientProcessResponse)->RangeMultiplier(10)->Range(1, 10000);

}  // namespace
}  // namespace psi_cardinality
