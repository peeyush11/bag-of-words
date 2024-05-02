

#include "features.hpp"

#include <iostream>
#include <opencv2/xfeatures2d/nonfree.hpp>


namespace igg {

cv::Mat ComputeFeatures
  (const cv::Mat& kImage)
{
  // Find keypoints
  const auto kDetector = cv::xfeatures2d::SiftFeatureDetector::create();
  std::vector<cv::KeyPoint> keypoints;
  kDetector->detect(kImage, keypoints);

  // Get descriptior of each keypoint
  cv::Mat descriptors;
  const auto kExtractor = cv::xfeatures2d::SiftDescriptorExtractor::create();
  kExtractor->compute(kImage, keypoints, descriptors);
  // 128 values per keypoint, type is CV_32FC1
  //std::cout << "Size of descriptors: " << descriptors.size() << std::endl;
  //std::cout << "Type of descriptors: " << descriptors.type() << std::endl;

  return descriptors;
}


// This function is no longer used an therefore deprecated.
//cv::Mat VisualizeKeypoints
  //(const cv::Mat& kImage,
   //const int kOutputRows,
   //const int kOutputCols)
//{
  //// Find keypoints
  //auto detector = cv::xfeatures2d::SiftFeatureDetector::create();
  //std::vector<cv::KeyPoint> keypoints;
  //detector->detect(kImage, keypoints);

  //// Visualization output
  //cv::Mat output;

  //// Resize image to desired output size
  //cv::Size size;
  //if (kOutputRows<0 || kOutputCols<0) {size = kImage.size();}
  //else {size = cv::Size(kOutputCols, kOutputRows);}
  //cv::resize(kImage, output, size);

  //const float kScaleFactorRows = static_cast<float>(kOutputRows)/kImage.rows;
  //const float kScaleFactorCols = static_cast<float>(kOutputCols)/kImage.cols;

  //// Draw keypoints
  //for (auto& keypoint: keypoints) {
    //// Compute position in scaled output
    //cv::Point scaled_position
      //(static_cast<int>(keypoint.pt.x*kScaleFactorCols),
       //static_cast<int>(keypoint.pt.y*kScaleFactorRows));
    //cv::drawMarker
      //(output,
       //scaled_position,
       //cv::Scalar(255, 255, 255),
       //cv::MARKER_DIAMOND,
       //10);
  //}

  //return output;
//}

} // namespace igg

