#include "absl/strings/str_cat.h"
#include "benchmark/benchmark.h"
#include "private_set_intersection/c/psi_client.h"
#include "private_set_intersection/c/psi_server.h"

namespace private_set_intersection {
namespace {

void BM_ServerSetup(benchmark::State &state, double fpr) {
  psi_server_ctx server_;
  bool reveal_intersection_ = false;
  char *err;
  psi_server_create_with_new_key(&server_, reveal_intersection_, &err);

  int num_inputs = state.range(0);
  int num_client_inputs = 10000;
  std::vector<std::string> orig_inputs(num_inputs);
  std::vector<psi_server_buffer_t> inputs(num_inputs);

  for (int i = 0; i < num_inputs; i++) {
    orig_inputs[i] = absl::StrCat("Element", i);
    inputs[i] = {orig_inputs[i].c_str(), orig_inputs[i].size()};
  }
  std::string setup;
  int64_t elements_processed = 0;
  for (auto _ : state) {
    char *server_setup = nullptr;
    size_t server_setup_buff_len = 0;
    psi_server_create_setup_message(server_, fpr, num_client_inputs,
                                    inputs.data(), inputs.size(), &server_setup,
                                    &server_setup_buff_len, &err);

    ::benchmark::DoNotOptimize(server_setup);
    ::benchmark::ClobberMemory();
    elements_processed += num_inputs;
    setup = server_setup;
    psi_server_delete_buffer(server_, &server_setup);
  }
  state.counters["SetupSize"] = benchmark::Counter(
      static_cast<double>(setup.size()), benchmark::Counter::kDefaults,
      benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}

// Range is for the number of inputs, and the captured argument is the false
// positive rate for 10k client queries.
BENCHMARK_CAPTURE(BM_ServerSetup, fpr = 0.001, 0.001)
    ->RangeMultiplier(100)
    ->Range(1, 1000000);
BENCHMARK_CAPTURE(BM_ServerSetup, fpr = 0.000001, 0.000001)
    ->RangeMultiplier(100)
    ->Range(1, 1000000);

void BM_ClientCreateRequest(benchmark::State &state) {
  psi_client_ctx client_;
  bool reveal_intersection_ = false;
  char *err;
  psi_client_create(&client_, reveal_intersection_, &err);

  int num_inputs = state.range(0);
  std::vector<std::string> inputs_orig(num_inputs);
  std::vector<psi_client_buffer_t> inputs(num_inputs);

  for (int i = 0; i < num_inputs; i++) {
    inputs_orig[i] = absl::StrCat("Element", i);
    inputs[i] = {inputs_orig[i].c_str(), inputs_orig[i].size()};
  }
  std::string request;
  int64_t elements_processed = 0;
  for (auto _ : state) {
    char *client_request = {0};
    size_t req_len = 0;
    char *err;
    psi_client_create_request(client_, inputs.data(), inputs.size(),
                              &client_request, &req_len, &err);

    ::benchmark::DoNotOptimize(client_request);
    ::benchmark::ClobberMemory();
    elements_processed += num_inputs;
    request = client_request;
    psi_client_delete_buffer(client_, &client_request);
  }
  state.counters["RequestSize"] = benchmark::Counter(
      static_cast<double>(request.size()), benchmark::Counter::kDefaults,
      benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs.
BENCHMARK(BM_ClientCreateRequest)->RangeMultiplier(10)->Range(1, 10000);

void BM_ServerProcessRequest(benchmark::State &state) {
  psi_client_ctx client_;
  psi_server_ctx server_;
  bool reveal_intersection_ = false;
  char *err;

  psi_client_create(&client_, reveal_intersection_, &err);
  psi_server_create_with_new_key(&server_, reveal_intersection_, &err);

  int num_inputs = state.range(0);
  std::vector<std::string> inputs_orig(num_inputs);
  std::vector<psi_client_buffer_t> inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs_orig[i] = absl::StrCat("Element", i);
    inputs[i] = {inputs_orig[i].c_str(), inputs_orig[i].size()};
  }

  char *client_request = nullptr;
  size_t req_len = 0;
  psi_client_create_request(client_, inputs.data(), inputs.size(),
                            &client_request, &req_len, &err);

  std::string response;

  int64_t elements_processed = 0;
  for (auto _ : state) {
    char *server_response = nullptr;
    size_t response_len = 0;
    psi_server_process_request(server_, {client_request, req_len},
                               &server_response, &response_len, &err);
    ::benchmark::DoNotOptimize(server_response);
    ::benchmark::ClobberMemory();

    elements_processed += num_inputs;
    response = server_response;
    psi_server_delete_buffer(server_, &server_response);
  }
  state.counters["ResponseSize"] = benchmark::Counter(
      static_cast<double>(response.size()), benchmark::Counter::kDefaults,
      benchmark::Counter::kIs1024);
  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);

  psi_client_delete_buffer(client_, &client_request);
}

// Range is for the number of inputs.
BENCHMARK(BM_ServerProcessRequest)->RangeMultiplier(10)->Range(1, 10000);

void BM_ClientProcessResponse(benchmark::State &state) {
  psi_client_ctx client_;
  psi_server_ctx server_;
  bool reveal_intersection_ = false;
  char *err;
  psi_client_create(&client_, reveal_intersection_, &err);
  psi_server_create_with_new_key(&server_, reveal_intersection_, &err);

  int num_inputs = state.range(0);
  double fpr = 1. / (1000000);
  std::vector<std::string> inputs_orig(num_inputs);
  std::vector<psi_server_buffer_t> srv_inputs(num_inputs);
  std::vector<psi_client_buffer_t> cl_inputs(num_inputs);
  for (int i = 0; i < num_inputs; i++) {
    inputs_orig[i] = absl::StrCat("Element", i);
    srv_inputs[i] = {inputs_orig[i].c_str(), inputs_orig[i].size()};
    cl_inputs[i] = {inputs_orig[i].c_str(), inputs_orig[i].size()};
  }

  char *server_setup = nullptr;
  size_t server_setup_buff_len = 0;
  psi_server_create_setup_message(server_, fpr, num_inputs, srv_inputs.data(),
                                  srv_inputs.size(), &server_setup,
                                  &server_setup_buff_len, &err);

  char *client_request = nullptr;
  size_t req_len = 0;
  psi_client_create_request(client_, cl_inputs.data(), cl_inputs.size(),
                            &client_request, &req_len, &err);

  char *server_response = nullptr;
  size_t response_len = 0;
  psi_server_process_request(server_, {client_request, req_len},
                             &server_response, &response_len, &err);

  int64_t elements_processed = 0;
  for (auto _ : state) {
    int64_t count = 0;
    psi_client_get_intersection_size(client_, server_setup, server_response,
                                     &count, &err);

    ::benchmark::DoNotOptimize(count);
    elements_processed += num_inputs;
  }

  psi_server_delete_buffer(server_, &server_setup);
  psi_server_delete_buffer(server_, &server_response);
  psi_client_delete_buffer(client_, &client_request);

  state.counters["ElementsProcessed"] = benchmark::Counter(
      static_cast<double>(elements_processed), benchmark::Counter::kIsRate);
}
// Range is for the number of inputs.
BENCHMARK(BM_ClientProcessResponse)->RangeMultiplier(10)->Range(1, 10000);

}  // namespace
}  // namespace private_set_intersection
