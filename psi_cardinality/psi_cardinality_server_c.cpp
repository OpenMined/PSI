#include "psi_cardinality_server_c.h"
#include "psi_cardinality_server.h"

using namespace psi_cardinality;

psi_cardinality_server_ctx psi_cardinality_server_create_with_new_key() {
    auto result = PSICardinalityServer::CreateWithNewkey();
    if (result.ok()) {
        auto value = std::move(result).ValueOrDie();
        return value.release();
    }
    return nullptr;
}

psi_cardinality_server_ctx psi_cardinality_server_create_from_key(const char* key_bytes) {
    auto result = PSICardinalityServer::CreateFromKey(key_bytes);
    if (result.ok()) {
        auto value = std::move(result).ValueOrDie();
        return value.release();
    }
    return nullptr;
}

void psi_cardinality_server_delete(psi_cardinality_server_ctx ctx) {
    auto server = static_cast<PSICardinalityServer*>(ctx);
    if (server == nullptr) {
        return;
    }
    delete server;
    server = nullptr;
}
void psi_cardinality_server_create_setup_message(
    psi_cardinality_server_ctx ctx, double fpr, int64_t num_client_inputs,
    const char** inputs, size_t input_len, char* output, size_t out_max_size){

    auto server = static_cast<PSICardinalityServer*>(ctx);
    if (server == nullptr) {
        return;
    }
    
    std::vector<std::string> in;
    for (size_t idx = 0; idx < input_len; ++idx) {
        in.push_back(inputs[idx]);
    }

    auto result = server->CreateSetupMessage(fpr, num_client_inputs, in);
    if (!result.ok()) {
        return;
    }

    auto value = result.ValueOrDie();
    size_t len = out_max_size < value.size() ? out_max_size : value.size();
    strncpy(output, value.c_str(), len);
}

void psi_cardinality_server_process_request(psi_cardinality_server_ctx ctx,
                                            const char* client_request,
                                            char* output,
                                            size_t out_max_size){

    auto server = static_cast<PSICardinalityServer*>(ctx);
    if (server == nullptr) {
        return;
    }
    
    auto result = server->ProcessRequest(client_request);
    if (!result.ok()) {
        return;
    }

    auto value = result.ValueOrDie();
    size_t len = out_max_size < value.size() ? out_max_size : value.size();
    strncpy(output, value.c_str(), len);
}

void psi_cardinality_server_get_private_key_bytes(
    psi_cardinality_server_ctx ctx, char* output, size_t out_max_size){
    auto server = static_cast<PSICardinalityServer*>(ctx);
    if (server == nullptr) {
        return;
    }
    auto value = server->GetPrivateKeyBytes(); 
    size_t len = out_max_size < value.size() ? out_max_size : value.size();
    strncpy(output, value.c_str(), len);
}
