#ifndef CPP_FINAL_PROJECT_CLUSTERING_FEATURE_POINT_HPP_
#define CPP_FINAL_PROJECT_CLUSTERING_FEATURE_POINT_HPP_

/**
 * @file feature_point.hpp
 *
 * The purpose of this file is to provide some function templates for common
 * operations on feature points (usually 128 dimensional vectors) as
 * extracted from images in the dataset.
 *
 * In particular this involves conversion from and to cv::Mat instances.
 *
 * We introduce FeaturePoint<T> as an alias for an std::vector<T>, where T
 * is usually a floating point type. This way expressions like
 * std::vector<std::vector<float>> turn into std::vector<FeaturePoint<float>>,
 * which we regard as more self-explaining.
 */

#include <vector>
#include <opencv2/opencv.hpp>


namespace igg {

template <class T>
using FeaturePoint = std::vector<T>;

/*
 * Get the index of the nearest neighbor in an ordered set of points,
 * where the distance measure is the L2 norm.
 */
template <class T>
size_t NearestNeighbor
  (const FeaturePoint<T>& kQueryPoint,
   const std::vector<FeaturePoint<T>>& kPointSet);

/*
 * Turn a std::vector of std::vector s of feature points into
 * a std::vector of feature points.
 */
template <class T>
std::vector<T> Flatten
  (std::vector<std::vector<T>>&& vector_of_vectors);

/*
 * Turn a cv::Mat into a std::vector of feature points, where each row
 * is expected to represent one point.
 */
template <class T>
std::vector<FeaturePoint<T>> FromMat(cv::Mat&& mat);

/*
 * Turn a std::vector of feature points into an instance of cv::Mat.
 *
 * Note all points are expected to have the same number of dimensions.
 */
template <class T>
cv::Mat ToMat(std::vector<FeaturePoint<T>>&& point_set);

/*
 * Turn a std::vector of feature points into an instance of cv::Mat.
 *
 * Variant that accepts an rvalue.
 *
 * Note all points are expected to have the same number of dimensions.
 */
template <class T>
cv::Mat ToMat(const std::vector<FeaturePoint<T>>& kPointSet);

/*
 * Get vector of pointers from vector feature points.
 */
template <class T>
std::vector<FeaturePoint<T> const *> GetPointers
  (const std::vector<FeaturePoint<T>>& kPointSet);

} // namespace igg

#include "feature_point.ipp"

#endif // CPP_FINAL_PROJECT_CLUSTERING_FEATURE_POINT_HPP_
