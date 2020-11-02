#ifndef CPP_FINAL_PROJECT_BINARYIO_BINARYIO_HPP_
#define CPP_FINAL_PROJECT_BINARYIO_BINARYIO_HPP_

/**
 * @file binaryio.hpp
 *
 * The purpose of this file is to provide reading and writing functionality
 * for various intermediate results related to the project.
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <fstream>
#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>

#include "clustering/feature_point.hpp"
#include "histogram/histogram.hpp"


namespace igg {

/*
 * Write a cv::Mat instance to a binary file.
 *
 * In case the given file already exists it is overwritten.
 *
 * @param kPath Where to write the file.
 * @param kMat The cv::Mat instance to write.
 *
 * @return True, if writing was successful.
 */
bool WriteMatToBinary (const std::string& kPath, const cv::Mat& kMat);

/*
 * Read a cv::Mat from a binary file. Number of rows, columns and
 * type is inferred from the file header.
 *
 * Throws a std::runtime_error in case the file cannot be read.
 *
 * @param kPath Where find the file.
 *
 * @return The cv::Mat object.
 */
cv::Mat ReadMatFromBinary(const std::string& kPath);

/*
 * Write centroids as obtained from the clustering to a binary file.
 *
 * In case the given file already exists it is overwritten.
 *
 * @param kPath Where write the file.
 * @param kCentroids The centroids to write. All with the same number of dimensions.
 *
 * @return True, if writing was successful.
 */
template <class T>
bool WriteCentroidsToBinary
  (const std::string& kPath, const std::vector<FeaturePoint<T>>& kCentroids);

/*
 * Read centroids as obtained from the clustering from a binary file.
 *
 * In case the given file already exists it is overwritten.
 *
 * Throws a std::runtime_error in case the file cannot be read.
 *
 * @param kPath Where to find the file.
 *
 * @return The centroids.
 */
template <class T>
std::vector<FeaturePoint<T>> ReadCentroidsFromBinary(const std::string& kPath);

/*
 * Write a std::vector of integral or floating point types to a binary.
 *
 * In case the given file already exists it is overwritten.
 *
 * @param kPath Where to write the file.
 * @param kValues The std::vector to write.
 *
 * @return True, if writing was successful.
 */
template <class T>
bool WriteToBinary(const std::string& kPath, const std::vector<T>& kValues);

/*
 * Read a std::vector of integral or floating point types to a binary.
 *
 * Note that no extra test is performed to check whether the actual file
 * meets the provided type and the behaviour is undefined if this is the case.
 *
 * Throws a std::runtime_error in case the file cannot be read.
 *
 * @param kPath Where to find the file.
 *
 * @return The vector.
 */
template <class T>
std::vector<T> ReadFromBinary(const std::string& kPath);

/*
 * Determine the size of a certain file in bytes. This requires the
 * file to be readable.
 *
 * Throws a std::runtime_error in case the file cannot be read.
 *
 * This function was added to let some components of this project
 * work without boost::filesystem.
 *
 * @param kPath Where to find the file.
 *
 * @return The size in bytes.
 */
std::streamsize FileSize(const std::string& kPath);

/*
 * Determine if a certain file exists and is readable.
 *
 * This function was added to let some components of this project
 * work without boost::filesystem.
 *
 * @param kPath The file to check.
 *
 * @return True, if the file exists.
 */
bool FileExists(const std::string& kPath);

} // namespace igg

#include "binaryio.ipp"

#endif // CPP_FINAL_PROJECT_BINARYIO_BINARYIO_HPP_

