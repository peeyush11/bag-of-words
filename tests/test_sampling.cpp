#include <gtest/gtest.h>
#include <random>

#include "tools/sampling.hpp"

namespace igg {

TEST(SamplingTest, SampleIndices) {
  const int kSeed = 0;
  std::mt19937 engine(kSeed);

  const size_t kNumElementsInSample = 100;
  const size_t kNumElementsTotal = 1000;

  const auto sample = igg::SampleIndicesWithoutReplacement<size_t>
    (kNumElementsInSample, kNumElementsTotal, engine);

  EXPECT_EQ(sample.size(), kNumElementsInSample);
}

} // namespace igg
