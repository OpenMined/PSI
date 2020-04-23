//
// Copyright 2020 the authors listed in CONTRIBUTORS.md
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "bloom_filter.h"
#include <cmath>
#include "absl/memory/memory.h"
#include "absl/strings/escaping.h"
#include "absl/strings/str_cat.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "util/canonical_errors.h"

namespace psi_cardinality {

BloomFilter::BloomFilter(
    int num_hash_functions, std::string bits,
    std::unique_ptr<::private_join_and_compute::Context> context)
    : num_hash_functions_(num_hash_functions),
      bits_(std::move(bits)),
      context_(std::move(context)) {}

StatusOr<std::unique_ptr<BloomFilter>> BloomFilter::Create(
    double fpr, int64_t max_elements) {
  if (fpr <= 0 || fpr >= 1) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`fpr` must be in (0,1)");
  }
  if (max_elements <= 0) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`max_elements` must be positive");
  }
  int num_hash_functions = static_cast<int>(std::ceil(-std::log2(fpr)));
  int64_t num_bytes = static_cast<int64_t>(
      std::ceil(-max_elements * std::log2(fpr) / std::log(2) / 8));
  std::string bits(num_bytes, '\0');
  auto context = absl::make_unique<::private_join_and_compute::Context>();
  return absl::WrapUnique(
      new BloomFilter(num_hash_functions, std::move(bits), std::move(context)));
}

StatusOr<std::unique_ptr<BloomFilter>> BloomFilter::CreateFromJSON(
    const std::string& encoded_filter) {
  rapidjson::Document json_filter;
  if (json_filter.Parse(encoded_filter.data(), encoded_filter.size())
          .HasParseError()) {
    return ::private_join_and_compute::InvalidArgumentError(
        absl::StrCat("Error parsing `encoded_filter`: ",
                     rapidjson::GetParseError_En(json_filter.GetParseError()),
                     "(", json_filter.GetErrorOffset(), ")"));
  }
  if (!json_filter.HasMember("num_hash_functions") ||
      !json_filter["num_hash_functions"].IsInt() ||
      !json_filter.HasMember("bits") || !json_filter["bits"].IsString()) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`encoded_filter` does encode a valid Bloom filter");
  }
  int num_hash_functions = json_filter["num_hash_functions"].GetInt();
  if (num_hash_functions < 0) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`num_hash_functions` must be positive");
  }
  std::string base64_bits(json_filter["bits"].GetString(),
                          json_filter["bits"].GetStringLength());
  std::string bits;
  if (!absl::Base64Unescape(base64_bits, &bits)) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`bits` is not a valid Base64 string");
  }
  if (bits.empty()) {
    return ::private_join_and_compute::InvalidArgumentError(
        "`bits` must not be empty");
  }
  auto context = absl::make_unique<::private_join_and_compute::Context>();
  return absl::WrapUnique(
      new BloomFilter(num_hash_functions, std::move(bits), std::move(context)));
}

void BloomFilter::Add(const std::string& input) {
  Add(absl::MakeConstSpan(&input, 1));
}

void BloomFilter::Add(absl::Span<const std::string> inputs) {
  for (const std::string& input : inputs) {
    for (int64_t index : Hash(input)) {
      bits_[index / 8] |= (1 << (index % 8));
    }
  }
}

bool BloomFilter::Check(const std::string& input) const {
  bool result = true;
  for (int64_t index : Hash(input)) {
    result &= ((bits_[index / 8] >> (index % 8)) & 1);
  }
  return result;
}

std::string BloomFilter::ToJSON() const {
  // Encode bits_ using Base64.
  std::string base64_bits = absl::Base64Escape(bits_);
  // Encode Bloom filter as JSON.
  rapidjson::Document json_filter;
  json_filter.SetObject();
  json_filter.AddMember("num_hash_functions",
                        rapidjson::Value().SetInt(NumHashFunctions()),
                        json_filter.GetAllocator());
  json_filter.AddMember(
      "bits",
      rapidjson::Value().SetString(base64_bits.data(), base64_bits.size()),
      json_filter.GetAllocator());
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<decltype(buffer)> writer(buffer);
  json_filter.Accept(writer);
  return std::string(buffer.GetString());
}

int BloomFilter::NumHashFunctions() const { return num_hash_functions_; }

std::vector<int64_t> BloomFilter::Hash(const std::string& x) const {
  // Compute the number of bits (= size of the output domain) as an OpenSSL
  // BigNum.
  const int64_t num_bits = 8 * bits_.size();
  const auto bn_num_bits = context_->CreateBigNum(num_bits);

  // Compute the i-th hash function as SHA256(1 || x) + i * SHA256(2 || x)
  // (modulo num_bits).
  std::vector<int64_t> result(num_hash_functions_);
  const int64_t h1 =
      context_->CreateBigNum(context_->Sha256String(absl::StrCat(1, x)))
          .Mod(bn_num_bits)
          .ToIntValue()
          .ValueOrDie();  // ValueOrDie is safe here since bn_num_bits fits in an int64.
  const int64_t h2 =
      context_->CreateBigNum(context_->Sha256String(absl::StrCat(2, x)))
          .Mod(bn_num_bits)
          .ToIntValue()
          .ValueOrDie();
  for (int i = 0; i < num_hash_functions_; i++) {
    result[i] = (h1 + i * h2) % num_bits;
  }
  return result;
}

}  // namespace psi_cardinality