#include "bloom_filter_c.h"

#include <string.h>

#include "absl/strings/str_cat.h"
#include "bloom_filter.h"
#include "util/statusor.h"

using namespace psi_cardinality;

bloom_filter_ctx bloom_filter_create(double fpr, int64_t max_elements) {
    auto result = BloomFilter::Create(fpr, max_elements);
    if (result.ok()) {
        auto value = std::move(result).ValueOrDie();
        return value.release();
    }
    return nullptr;
}

bloom_filter_ctx bloom_filter_create_from_json(const char* encoded_filter) {
    auto result = BloomFilter::CreateFromJSON(encoded_filter);
    if (result.ok()) {
        auto value = std::move(result).ValueOrDie();
        return value.release();
    }
    return nullptr;
}
void bloom_filter_delete(bloom_filter_ctx ctx) {
    auto bloom = static_cast<BloomFilter*>(ctx);
    if (bloom == nullptr) {
        return;
    }
    delete bloom;
    bloom = nullptr;
}

void bloom_filter_add(bloom_filter_ctx ctx, const char* input) {
    auto bloom = static_cast<BloomFilter*>(ctx);
    if (bloom == nullptr) {
        return;
    }
    bloom->Add(input);
}

void bloom_filter_add_array(bloom_filter_ctx ctx, const char** input,
                            size_t len) {
    auto bloom = static_cast<BloomFilter*>(ctx);
    if (bloom == nullptr) {
        return;
    }

    std::vector<std::string> in;
    for (size_t idx = 0; idx < len; ++idx) {
        in.push_back(input[idx]);
    }
    bloom->Add(in);
}

uint8_t bloom_filter_check(const bloom_filter_ctx ctx, const char* input) {
    auto bloom = static_cast<BloomFilter*>(ctx);
    if (bloom == nullptr) {
        return 0;
    }

    return (uint8_t)bloom->Check(input);
}

void bloom_filter_to_json(const bloom_filter_ctx ctx, char* out,
                          size_t out_max_len) {
    auto bloom = static_cast<BloomFilter*>(ctx);
    if (bloom == nullptr) {
        return;
    }

    auto json = bloom->ToJSON();
    size_t len = out_max_len < json.size() ? out_max_len : json.size();
    strncpy(out, json.c_str(), len);
}

int bloom_filter_num_hash_functions(const bloom_filter_ctx ctx) {
    auto bloom = static_cast<BloomFilter*>(ctx);
    if (bloom == nullptr) {
        return -1;
    }

    return bloom->NumHashFunctions();
}
