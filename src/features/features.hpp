#ifndef CPP_FINAL_PROJECT_FEATURES_FEATURES_HPP_
#define CPP_FINAL_PROJECT_FEATURES_FEATURES_HPP_

/*
 * @file features.hpp
 *
 * The purpose of this file is to provide a wrapper around feature extraction
 * as proved by OpenCV.
 *
 * We use SIFT features.
 *
 * Reference: https://www.cs.ubc.ca/~lowe/papers/ijcv04.pdf
 */

#include <opencv2/opencv.hpp>


namespace igg {

/*
 * Get SIFT feature descriptors as a cv::Mat from a given images.
 *
 * Each matrix row represents one feature, which is a 128 dimensionl vector.
 * The cv::Mat type is CV_32FC1 (single channel floating point).
 *
 * Feature locations are discarded as they are not of interest for
 * the bag of words approach.
 */
cv::Mat ComputeFeatures
  (const cv::Mat& kImage);

// This function is no longer used an therefore deprecated.
//cv::Mat VisualizeKeypoints
  //(const cv::Mat& kImage,
   //const int kOutputRows=-1,
   //const int kOutputCols=-1);

} // namespace igg

#endif // CPP_FINAL_PROJECT_FEATURES_FEATURES_HPP_

