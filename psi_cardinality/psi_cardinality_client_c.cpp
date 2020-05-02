#include "psi_cardinality_client_c.h"

#include "psi_cardinality_client.h"

using namespace psi_cardinality;

psi_cardinality_client_ctx psi_cardinality_client_create() {
    auto result = PSICardinalityClient::Create();
    if (result.ok()) {
        auto value = std::move(result).ValueOrDie();
        return value.release();
    }
    return nullptr;
}

void psi_cardinality_client_delete(psi_cardinality_client_ctx ctx) {
    auto client = static_cast<PSICardinalityClient*>(ctx);
    if (client == nullptr) {
        return;
    }
    delete client;
    client = nullptr;
}

void psi_cardinality_client_create_request(psi_cardinality_client_ctx ctx,
                                           const char** inputs,
                                           size_t input_len, char* output,
                                           size_t out_max_size) {
    auto client = static_cast<PSICardinalityClient*>(ctx);
    if (client == nullptr) {
        return;
    }
    std::vector<std::string> in;
    for (size_t idx = 0; idx < input_len; ++idx) {
        in.push_back(inputs[idx]);
    }

    auto result = client->CreateRequest(in);
    if (!result.ok()) {
        return;
    }
    auto value = std::move(result).ValueOrDie();
    size_t len = out_max_size < value.size() ? out_max_size : value.size();
    strncpy(output, value.c_str(), len);
}

int64_t psi_cardinality_client_process_response(psi_cardinality_client_ctx ctx,
                                                const char* server_setup,
                                                const char* server_response) {
    auto client = static_cast<PSICardinalityClient*>(ctx);
    if (client == nullptr) {
        return -1;
    }
    auto result = client->ProcessResponse(server_setup, server_response);
    if (!result.ok()) {
        return -1;
    }
    return result.ValueOrDie();
}
