#include <gtest/gtest.h>
#include <vector>

#include "tools/linalg.hpp"


namespace igg {

using VectorType = std::vector<float>;

TEST(LinalgTest, Difference) {
  const VectorType kMinuend{1.0f, 5.0f, 2.2f};
  const VectorType kSubtrahend{2.0f, 3.0f, 1.3f};

  const auto kDifference = Difference(kMinuend, kSubtrahend);
  EXPECT_FLOAT_EQ(kDifference[0], -1.0f);
  EXPECT_FLOAT_EQ(kDifference[1], 2.0f);
  EXPECT_FLOAT_EQ(kDifference[2], 0.9f);
}


TEST(LinalgTest, L2Norm) {
  const VectorType kVector{3.0f, 4.0f};
  EXPECT_FLOAT_EQ(L2Norm(kVector), 5.0f);
}


TEST(LinalgTest, SquaredL2Norm) {
  const VectorType kVector{1.0f, 2.0f, 3.0f};
  EXPECT_FLOAT_EQ(SquaredL2Norm(kVector), 14.0f);
}


TEST(LinalgTest, Centroid) {
  const VectorType kVector1{1.3f, 2.0f, 3.0f};
  const VectorType kVector2{2.0f, 4.6f, 5.0f};
  const VectorType kVector3{3.0f, 3.0f, 1.9f};

  const std::vector<VectorType const *> kVectorSet
    {&kVector1, &kVector2, &kVector3};

  const auto kCentroid = Centroid(kVectorSet);

  EXPECT_FLOAT_EQ(kCentroid[0], 2.1f);
  EXPECT_FLOAT_EQ(kCentroid[1], 3.2f);
  EXPECT_FLOAT_EQ(kCentroid[2], 3.3f);
}


TEST(LinalgTest, Variance) {
  const VectorType kVector1{1.3f, 2.0f, 3.0f};
  const VectorType kVector2{2.0f, 4.6f, 5.0f};
  const VectorType kVector3{3.0f, 3.0f, 1.9f};

  const std::vector<VectorType const *> kVectorSet
    {&kVector1, &kVector2, &kVector3};

  const auto kVariance = Variance(kVectorSet);

  EXPECT_FLOAT_EQ(kVariance[0], 0.48666667f);
  EXPECT_FLOAT_EQ(kVariance[1], 1.14666667f);
  EXPECT_FLOAT_EQ(kVariance[2], 1.64666667f);
}


} // namespace igg
