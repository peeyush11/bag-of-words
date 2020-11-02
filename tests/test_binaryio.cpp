#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <array>

#include "binaryio/binaryio.hpp"

#include "get_tests_data_path.hpp"


namespace igg {

TEST(BinaryioTest, WriteReadImage) {
  const auto kBinaryPath = GetTestsOutputPath()/"lenna.binary";
  if (fs::exists(kBinaryPath)) {fs::remove(kBinaryPath);}

  // Load two test image
  const auto kImagePath = GetTestsDataPath()/"lenna.png";
  const auto kImage = cv::imread(kImagePath.string(), CV_LOAD_IMAGE_COLOR);

  // Write to binary
  WriteMatToBinary(kBinaryPath.string(), kImage);

  // Read again
  const auto kImageFromBinary = ReadMatFromBinary(kBinaryPath.string());

  // Should show the image
  //cv::imshow("image", image_from_binary);
  //cv::waitKey(0);

  // Check the size and type of the image just read from binary
  EXPECT_EQ(kImageFromBinary.cols, kImage.cols);
  EXPECT_EQ(kImageFromBinary.rows, kImage.rows);
  EXPECT_EQ(kImageFromBinary.type(), kImage.type());

  // Check some pixel values
  const auto kPixelFromBinary = kImageFromBinary.at<cv::Vec<uchar, 3>>(20, 10);
  const auto kPixelFromImage = kImage.at<cv::Vec<uchar, 3>>(20, 10);
  EXPECT_EQ(kPixelFromBinary, kPixelFromImage);
}


TEST(BinaryioTest, WriteReadMatOfFloats) {
  const auto kBinaryPath = GetTestsOutputPath()/"mat_of_floats.binary";
  if (fs::exists(kBinaryPath)) {fs::remove(kBinaryPath);}

  //std::cout << GetTestsOutputPath() << "\n";
  //std::cout << kBinaryPath << "\n";

  // Create two test matrices contaning floats
  const std::vector<float> kData {0.0f, 1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
  // Arguments of cv::Mat::reshape: number of channels, number of rows
  const auto kMat = cv::Mat_<float>(kData, true).reshape(0, 2); // True to copy data

  // << 0.0f, 1.1f, 2.2f, 3.3f, 4.4f, 5.5f);//.reshape(0, 2);
  const std::vector<float> kOtherData {6.6f, 7.7f, 8.8f, 9.9f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f};
  const auto kAnotherMat = cv::Mat_<float>(kOtherData, true).reshape(0, 3); // True to copy data

  //std::cout << "one: " << kMat << "\n";
  //std::cout << "another: " << kAnotherMat << "\n";

  // Write the first matrix to binary
  WriteMatToBinary(kBinaryPath.string(), kMat);

  // Read again
  const auto kMatFromBinary = ReadMatFromBinary(kBinaryPath.string());

  // Check some values
  EXPECT_FLOAT_EQ(kMatFromBinary.at<float>(1, 0), 3.3f);
  EXPECT_FLOAT_EQ(kMatFromBinary.at<float>(0, 2), 2.2f);

  // Write the second matrix to binary (same file)
  WriteMatToBinary(kBinaryPath.string(), kAnotherMat);

  // Read again
  const auto kAnotherMatFromBinary = ReadMatFromBinary(kBinaryPath.string());

  //std::cout << "one from binary: " << kMatFromBinary << "\n";
  //std::cout << "another from binary: " << kAnotherMatFromBinary << "\n";

  // Check some values
  EXPECT_FLOAT_EQ(kAnotherMatFromBinary.at<float>(1, 0), 9.9f);
  EXPECT_FLOAT_EQ(kAnotherMatFromBinary.at<float>(2, 1), 13.13f);
}


TEST(BinaryioTest, WriteReadCentroids) {
  const FeaturePoint<float> kCentroid1{1.3f, 2.0f, 3.0f};
  const FeaturePoint<float> kCentroid2{2.0f, 4.6f, 5.0f};
  const FeaturePoint<float> kCentroid3{3.0f, -3.0f, 1.9f};
  const FeaturePoint<float> kCentroid4{5.0f, 8.0f, -3.1f};

  const std::vector<FeaturePoint<float>> kCentroids
    {kCentroid1, kCentroid2, kCentroid3, kCentroid4};

  const auto kBinaryPath = GetTestsOutputPath()/"test_centroids.binary";
  if (fs::exists(kBinaryPath)) {fs::remove(kBinaryPath);}

  WriteCentroidsToBinary(kBinaryPath.string(), kCentroids);

  const auto kCentroidsFromBinary = ReadCentroidsFromBinary<float>(kBinaryPath.string());

  // Check some values
  EXPECT_FLOAT_EQ(kCentroidsFromBinary[0][1], 2.0f);
  EXPECT_FLOAT_EQ(kCentroidsFromBinary[2][1], -3.0f);
  EXPECT_FLOAT_EQ(kCentroidsFromBinary[3][2], -3.1f);
}


TEST(BinaryioTest, FileExist) {
  const auto kImagePath = GetTestsDataPath()/"lenna.png";
  EXPECT_EQ(FileExists(kImagePath.string()), true);

  const std::string kAnotherPath = "xyz/xyz.xyz";
  EXPECT_EQ(FileExists(kAnotherPath), false);
}

}
