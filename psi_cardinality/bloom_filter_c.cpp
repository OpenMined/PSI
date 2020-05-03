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
void bloom_filter_delete(bloom_filter_ctx* ctx) {
    auto bloom = static_cast<BloomFilter*>(*ctx);
    if (bloom == nullptr) {
        return;
    }
    delete bloom;
    *ctx = nullptr;
}

void bloom_filter_add(bloom_filter_ctx ctx, struct bloom_buffer_t input) {
    auto bloom = static_cast<BloomFilter*>(ctx);
    if (bloom == nullptr) {
        return;
    }
    std::string buff(input.buff, input.buff_len);
    bloom->Add(buff);
}

void bloom_filter_add_array(bloom_filter_ctx ctx,
                            const struct bloom_buffer_t* input, size_t len) {
    auto bloom = static_cast<BloomFilter*>(ctx);
    if (bloom == nullptr) {
        return;
    }

    std::vector<std::string> in;
    for (size_t idx = 0; idx < len; ++idx) {
        std::string buff(input[idx].buff, input[idx].buff_len);
        in.push_back(buff);
    }
    bloom->Add(in);
}

uint8_t bloom_filter_check(const bloom_filter_ctx ctx,
                           struct bloom_buffer_t input) {
    auto bloom = static_cast<BloomFilter*>(ctx);
    if (bloom == nullptr) {
        return 0;
    }

    std::string buff(input.buff, input.buff_len);
    return (uint8_t)bloom->Check(buff);
}

void bloom_filter_to_json(const bloom_filter_ctx ctx, char** out,
                          size_t* out_len) {
    auto bloom = static_cast<BloomFilter*>(ctx);
    if (bloom == nullptr) {
        return;
    }

    auto json = bloom->ToJSON();

    size_t len = json.size() + 1;
    *out = new char[len];
    strncpy(*out, json.c_str(), len);
    *out_len = len;
}

void bloom_filter_delete_json(const bloom_filter_ctx ctx, char** json) {
    if (json == nullptr) return;

    delete[] * json;
    *json = nullptr;
}

int bloom_filter_num_hash_functions(const bloom_filter_ctx ctx) {
    auto bloom = static_cast<BloomFilter*>(ctx);
    if (bloom == nullptr) {
        return -1;
    }

    return bloom->NumHashFunctions();
}
