#include "absl/strings/str_cat.h"
#include "benchmark/benchmark.h"
#include "private_set_intersection/cpp/psi_client.h"
#include "private_set_intersection/cpp/psi_server.h"

namespace private_set_intersection {
namespace {

void BM_ServerSetup(benchmark::State& state, double fpr,
                    bool reveal_intersection) {
  auto server = PsiServer::CreateWithNewKey(reveal_intersection).ValueOrDie();
  int num_inputs = state.range(0);
  int num_client_inputs = 10000;
  std::vector<std::string> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs[i] = absl::StrCat("Element", i);
  }
  std::string setup;
  int64_t elements_processed = 0;
  for (auto _ : state) {
    setup =
        server->CreateSetupMessage(fpr, num_client_inputs, inputs).ValueOrDie();
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
// positive rate for 10k client queries.
BENCHMARK_CAPTURE(BM_ServerSetup, 0.001 size, 0.001, false)
    ->RangeMultiplier(100)
    ->Range(1, 1000000);
BENCHMARK_CAPTURE(BM_ServerSetup, 0.000001 size, 0.000001, false)
    ->RangeMultiplier(100)
    ->Range(1, 1000000);
BENCHMARK_CAPTURE(BM_ServerSetup, 0.001 intersection, 0.001, true)
    ->RangeMultiplier(100)
    ->Range(1, 1000000);
BENCHMARK_CAPTURE(BM_ServerSetup, 0.000001 intersection, 0.000001, true)
    ->RangeMultiplier(100)
    ->Range(1, 1000000);

void BM_ClientCreateRequest(benchmark::State& state, bool reveal_intersection) {
  auto client = PsiClient::Create(reveal_intersection).ValueOrDie();
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
BENCHMARK_CAPTURE(BM_ClientCreateRequest, size, false)
    ->RangeMultiplier(10)
    ->Range(1, 10000);
BENCHMARK_CAPTURE(BM_ClientCreateRequest, intersection, true)
    ->RangeMultiplier(10)
    ->Range(1, 10000);

void BM_ServerProcessRequest(benchmark::State& state,
                             bool reveal_intersection) {
  auto client = PsiClient::Create(reveal_intersection).ValueOrDie();
  auto server = PsiServer::CreateWithNewKey(reveal_intersection).ValueOrDie();
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
BENCHMARK_CAPTURE(BM_ServerProcessRequest, size, false)
    ->RangeMultiplier(10)
    ->Range(1, 10000);
BENCHMARK_CAPTURE(BM_ServerProcessRequest, intersection, true)
    ->RangeMultiplier(10)
    ->Range(1, 10000);

void BM_ClientProcessResponse(benchmark::State& state,
                              bool reveal_intersection) {
  auto client = PsiClient::Create(reveal_intersection).ValueOrDie();
  auto server = PsiServer::CreateWithNewKey(reveal_intersection).ValueOrDie();
  int num_inputs = state.range(0);
  double fpr = 1. / (1000000);
  std::vector<std::string> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs[i] = absl::StrCat("Element", i);
  }
  std::string setup =
      server->CreateSetupMessage(fpr, num_inputs, inputs).ValueOrDie();
  std::string request = client->CreateRequest(inputs).ValueOrDie();
  std::string response = server->ProcessRequest(request).ValueOrDie();
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
BENCHMARK_CAPTURE(BM_ClientProcessResponse, size, false)
    ->RangeMultiplier(10)
    ->Range(1, 10000);
BENCHMARK_CAPTURE(BM_ClientProcessResponse, intersection, true)
    ->RangeMultiplier(10)
    ->Range(1, 10000);

}  // namespace
}  // namespace private_set_intersection
