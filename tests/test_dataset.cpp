#include <gtest/gtest.h>

#include "dataset/dataset.hpp"

#include "make_test_dataset.hpp"


namespace igg {

TEST(DatasetTest, LoadDataset) {
  const auto kDataset = MakeTestDataset();

  // Check if some files are found
  EXPECT_TRUE(kDataset.Items().size()>0);
}

TEST(DatasetTest, LoadDefault) {
  const auto kDataset = Dataset::Default();

  // Assert default dataset can be loaded
  ASSERT_TRUE(kDataset!=nullptr);

  // Check if some files are found
  EXPECT_TRUE(kDataset->Items().size()>0);
}

} // namespace igg

