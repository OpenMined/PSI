// Copyright 2019 Google LLC
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

#ifndef UTIL_STATUS_MATCHERS_H_
#define UTIL_STATUS_MATCHERS_H_

#include <type_traits>

#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "absl/types/optional.h"
#include "gmock/gmock.h"

#define PSI_ASSERT_OK_AND_ASSIGN(lhs, rexpr) \
  PSI_ASSERT_OK_AND_ASSIGN_IMPL(             \
      PSI_STATUS_MACROS_IMPL_CONCAT_(status_or_value, __LINE__), lhs, rexpr)

#define PSI_ASSERT_OK_AND_ASSIGN_IMPL(statusor, lhs, rexpr)           \
  auto statusor = (rexpr);                                            \
  ASSERT_THAT(statusor.status(), ::private_set_intersection::IsOk()); \
  lhs = std::move(statusor).value();

// Internal helper for concatenating macro values.
#define PSI_STATUS_MACROS_IMPL_CONCAT_INNER_(x, y) x##y
#define PSI_STATUS_MACROS_IMPL_CONCAT_(x, y) \
  PSI_STATUS_MACROS_IMPL_CONCAT_INNER_(x, y)

namespace private_set_intersection {
namespace internal {

class IsOkMatcher {
 public:
  template <typename StatusT>
  bool MatchAndExplain(const StatusT& status_container,
                       ::testing::MatchResultListener* listener) const {
    if (!status_container.ok()) {
      *listener << "which is not OK";
      return false;
    }
    return true;
  }

  void DescribeTo(std::ostream* os) const { *os << "is OK"; }

  void DescribeNegationTo(std::ostream* os) const { *os << "is not OK"; }
};

class StatusIsMatcher {
 public:
  StatusIsMatcher(const StatusIsMatcher&) = default;
  StatusIsMatcher& operator=(const StatusIsMatcher&) = default;

  StatusIsMatcher(absl::StatusCode code,
                  absl::optional<absl::string_view> message)
      : code_(code), message_(message) {}

  template <typename T>
  bool MatchAndExplain(const T& value,
                       ::testing::MatchResultListener* listener) const {
    auto status = GetStatus(value);
    if (code_ != status.code()) {
      *listener << "whose error code is " << status.code();
      return false;
    }
    if (message_.has_value() && status.message() != message_.value()) {
      *listener << "whose error message is '" << status.message() << "'";
      return false;
    }
    return true;
  }

  void DescribeTo(std::ostream* os) const {
    *os << "has a status code that is " << code_;
    if (message_.has_value()) {
      *os << ", and has an error message that is '" << message_.value() << "'";
    }
  }

  void DescribeNegationTo(std::ostream* os) const {
    *os << "has a status code that is not " << code_;
    if (message_.has_value()) {
      *os << ", and has an error message that is not '" << message_.value()
          << "'";
    }
  }

 private:
  template <typename StatusT,
            typename std::enable_if<
                !std::is_void<decltype(std::declval<StatusT>().code())>::value,
                int>::type = 0>
  static const StatusT& GetStatus(const StatusT& status) {
    return status;
  }

  template <typename StatusOrT,
            typename StatusT = decltype(std::declval<StatusOrT>().status())>
  static StatusT GetStatus(const StatusOrT& status_or) {
    return status_or.status();
  }

  const absl::StatusCode code_;
  const absl::optional<std::string> message_;
};

}  // namespace internal

inline ::testing::PolymorphicMatcher<internal::IsOkMatcher> IsOk() {
  return ::testing::MakePolymorphicMatcher(internal::IsOkMatcher{});
}

inline ::testing::PolymorphicMatcher<internal::StatusIsMatcher> StatusIs(
    absl::StatusCode code,
    absl::optional<absl::string_view> message = absl::nullopt) {
  return ::testing::MakePolymorphicMatcher(
      internal::StatusIsMatcher(code, message));
}

}  // namespace private_set_intersection

#endif  // UTIL_STATUS_MATCHERS_H_
