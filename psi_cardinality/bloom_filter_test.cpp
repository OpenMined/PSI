#include "bloom_filter.h"
#include "absl/strings/str_cat.h"
#include "gtest/gtest.h"
#include "util/status_matchers.h"

namespace psi_cardinality {
namespace {

class BloomFilterTest : public ::testing::Test {
 protected:
  void SetUp() { return SetUp(0.001, 1 << 10); }
  void SetUp(double fpr, int max_elements) {
    PSI_ASSERT_OK_AND_ASSIGN(filter_, BloomFilter::Create(fpr, max_elements));
  }

  std::unique_ptr<BloomFilter> filter_;
};

TEST_F(BloomFilterTest, TestAdd) {
  std::vector<std::string> elements = {"a", "b", "c", "d"};

  // Test both variants of Add.
  filter_->Add(elements[0]);
  filter_->Add(absl::MakeConstSpan(&elements[1], elements.size() - 1));

  // Check if all elements are present.
  for (const auto& element : elements) {
    EXPECT_TRUE(filter_->Check(element));
  }
  EXPECT_FALSE(filter_->Check("not present"));
}

TEST_F(BloomFilterTest, TestFPR) {
  for (int max_elements = 1 << 10; max_elements < (1 << 20);
       max_elements *= 2) {
    double target_fpr = 0.1;
    SetUp(target_fpr, max_elements);
    // Insert `max_elements` elements.
    for (int i = 0; i < max_elements; i++) {
      filter_->Add(absl::StrCat("Element ", i));
    }
    // Test 10k elements to measure FPR.
    double count = 0;
    int num_tests = 10000;
    for (int i = 0; i < num_tests; i++) {
      if (filter_->Check(absl::StrCat("Test ", i))) {
        count++;
      }
    }
    // Check if actual FPR matches the target FPR, allowing for 20% error.
    double actual_fpr = count / num_tests;
    EXPECT_LT(actual_fpr, 1.2 * target_fpr)
        << absl::StrCat("max_elements: ", max_elements);
  }
}

}  // namespace
}  // namespace psi_cardinality
