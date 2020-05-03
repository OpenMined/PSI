#include <emscripten/bind.h>
#include "psi_cardinality_server.h"
#include <iostream>

using namespace emscripten;
using namespace psi_cardinality;
using namespace private_join_and_compute;

int main() {
    // TODO: remove after working...
    printf("main server running!\n");
    return 0;
};

/*
  Transform a JS Array to a Vector
*/
template<typename T>
std::vector<T> vecFromJSArray(const val &v) {
    auto l = v["length"].as<unsigned>();

    std::vector<T> rv;
    for(unsigned i = 0; i < l; ++i) {
        rv.push_back(v[i].as<T>());
    }

    return rv;
};

/*
 Gets the exception string from the thrown pointer
*/
std::string get_exception(intptr_t ptr) {
    auto exception = reinterpret_cast<std::exception *>(ptr);
    std::string error_string = exception->what();
    return error_string;
}

EMSCRIPTEN_BINDINGS(PSI_Server) {
    function("getException", &get_exception);
    function("vecFromJSArray", select_overload<std::vector<std::string>(const val &)>(&vecFromJSArray));

    register_vector<std::string>("std::vector<std::string>");

    class_<PSICardinalityServer>("PSICardinalityServer")
        .smart_ptr<std::shared_ptr<PSICardinalityServer>>("std::shared_ptr<PSICardinalityServer>")
        .class_function("CreateWithNewKey", optional_override([]() {
            std::shared_ptr<PSICardinalityServer> server = PSICardinalityServer::CreateWithNewKey().ValueOrDie();
            return server;
        }))
        .class_function("CreateFromKey", optional_override([](const std::string &key_bytes) {
            std::shared_ptr<PSICardinalityServer> server = PSICardinalityServer::CreateFromKey(key_bytes).ValueOrDie();
            return server;
        }))
        .function("CreateSetupMessage", optional_override([](const PSICardinalityServer &self, 
            const double fpr, const int32_t num_client_inputs, const std::vector<std::string> &inputs) {
            std::string message = self.CreateSetupMessage(fpr, (int64_t)num_client_inputs, inputs).ValueOrDie();
            return message;
        }))
        .function("ProcessRequest", optional_override([](const PSICardinalityServer &self, 
            const std::string &client_request) {
            std::string response = self.ProcessRequest(client_request).ValueOrDie();
            return response;
        }))
        .function("GetPrivateKeyBytes", optional_override([](const PSICardinalityServer &self) {
            return self.GetPrivateKeyBytes();
        }));
}