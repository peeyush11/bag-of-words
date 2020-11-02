#include <gtest/gtest.h>
#include <memory>
#include <opencv2/opencv.hpp>

#include "features/features.hpp"
#include "get_tests_data_path.hpp"


namespace igg {

TEST(FeaturesTest, ExtractFeatures) {
  const auto kImagePath = GetTestsDataPath()/"lenna.png";

  // Load a test image
  auto image = cv::imread(kImagePath.string(), CV_LOAD_IMAGE_COLOR);

  const auto kFeatures = ComputeFeatures(image);

  EXPECT_EQ(kFeatures.cols, 128);
}

}
