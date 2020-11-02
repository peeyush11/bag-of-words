#include <gtest/gtest.h>
#include <vector>

#include "histogram/histogram.hpp"


namespace igg {

TEST(HistogramTest, HistogramCompare) {
  const std::vector<float> kHistogram{1.0f, 2.0f, 3.0f};

  const std::vector<float> kHistogramToCompare1{1.0f, 2.0f, 3.0f};
  const std::vector<float> kHistogramToCompare2{3.0f, 2.0f, 2.0f};

  const std::vector<std::vector<float>> kHistogramsToCompare
    {kHistogramToCompare1, kHistogramToCompare2};

  const auto kSimilarities = ComputeSimilarities<float>
    (kHistogram, kHistogramsToCompare);

  EXPECT_FLOAT_EQ(kSimilarities[0], 1.0f);
  EXPECT_FLOAT_EQ(kSimilarities[1], 0.84266484f);
}


TEST(HistogramTest, HistogramCompareDimensionMismatch) {
  const std::vector<float> kHistogram{1.0f, 2.0f, 3.0f};

  const std::vector<float> kHistogramToCompare1{1.0f, 2.0f, 3.0f};
  const std::vector<float> kHistogramToCompare2{3.0f, 2.0f};

  const std::vector<std::vector<float>> kHistogramsToCompare
    {kHistogramToCompare1, kHistogramToCompare2};

  EXPECT_THROW
    (ComputeSimilarities(kHistogram, kHistogramsToCompare),
     std::invalid_argument);
}




} // namespace igg

