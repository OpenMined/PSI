#include <emscripten/bind.h>
#include "psi_cardinality_client.h"

using namespace emscripten;
using namespace psi_cardinality;

int main() {
    // TODO: remove after working...
    printf("main client running!\n");
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

EMSCRIPTEN_BINDINGS(PSI_Client) {
    function("getException", &get_exception);
    function("vecFromJSArray", select_overload<std::vector<std::string>(const val &)>(&vecFromJSArray));

    register_vector<std::string>("std::vector<std::string>");

    class_<PSICardinalityClient>("PSICardinalityClient")
        .class_function("Create", &PSICardinalityClient::Create)
        .function("CreateRequest", &PSICardinalityClient::CreateRequest)
        .function("ProcessResponse", &PSICardinalityClient::ProcessResponse);
}