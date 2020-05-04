#include "psi_cardinality_server_c.h"

#include "psi_cardinality_server.h"

using namespace psi_cardinality;

psi_cardinality_server_ctx psi_cardinality_server_create_with_new_key() {
    auto result = PSICardinalityServer::CreateWithNewKey();
    if (result.ok()) {
        auto value = std::move(result).ValueOrDie();
        return value.release();
    }
    return nullptr;
}

psi_cardinality_server_ctx psi_cardinality_server_create_from_key(
    server_buffer_t key_bytes) {
    auto result = PSICardinalityServer::CreateFromKey(
        std::string(key_bytes.buff, key_bytes.buff_len));
    if (result.ok()) {
        auto value = std::move(result).ValueOrDie();
        return value.release();
    }
    return nullptr;
}

void psi_cardinality_server_delete(psi_cardinality_server_ctx* ctx) {
    auto server = static_cast<PSICardinalityServer*>(*ctx);
    if (server == nullptr) {
        return;
    }
    delete server;
    *ctx = nullptr;
}

int psi_cardinality_server_create_setup_message(psi_cardinality_server_ctx ctx,
                                                double fpr,
                                                int64_t num_client_inputs,
                                                server_buffer_t* input,
                                                size_t input_len, char** output,
                                                size_t* output_len) {
    auto server = static_cast<PSICardinalityServer*>(ctx);
    if (server == nullptr) {
        return -1;
    }

    std::vector<std::string> in;
    for (size_t idx = 0; idx < input_len; ++idx) {
        in.push_back(std::string(input[idx].buff, input[idx].buff_len));
    }

    auto result = server->CreateSetupMessage(fpr, num_client_inputs, in);
    if (!result.ok()) {
        return -1;
    }

    auto value = result.ValueOrDie();
    size_t len = value.size() + 1;
    *output = new char[len];
    strncpy(*output, value.c_str(), len);
    *output_len = len;

    return 0;
}

int psi_cardinality_server_process_request(psi_cardinality_server_ctx ctx,
                                           server_buffer_t client_request,
                                           char** output, size_t* output_len) {
    auto server = static_cast<PSICardinalityServer*>(ctx);
    if (server == nullptr) {
        return -1;
    }

    auto result = server->ProcessRequest(
        std::string(client_request.buff, client_request.buff_len));
    if (!result.ok()) {
        return -1;
    }

    auto value = result.ValueOrDie();
    size_t len = value.size() + 1;
    *output = new char[len];
    strncpy(*output, value.c_str(), len);
    *output_len = len;

    return 0;
}

int psi_cardinality_server_get_private_key_bytes(psi_cardinality_server_ctx ctx,
                                                 char** output,
                                                 size_t* output_len) {
    auto server = static_cast<PSICardinalityServer*>(ctx);
    if (server == nullptr) {
        return -1;
    }
    auto value = server->GetPrivateKeyBytes();
    size_t len = value.size() + 1;
    *output = new char[len];
    strncpy(*output, value.c_str(), len);
    *output_len = len;

    return 0;
}

void psi_cardinality_server_delete_buffer(psi_cardinality_server_ctx ctx,
                                          char** input) {
    delete[] * input;
    *input = nullptr;
}
