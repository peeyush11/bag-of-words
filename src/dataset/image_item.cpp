/**
 * @file image_item.cpp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include "image_item.hpp"
#include <boost/filesystem.hpp>

#include "binaryio/binaryio.hpp"


namespace igg {

namespace fs = boost::filesystem;

ImageItem::ImageItem
  (const std::string kImagePath,
   const std::string kFeaturesBinaryPath,
   const std::string kHistogramBinaryPath):
  kImagePath_{kImagePath},
  kFeaturesBinaryPath_{kFeaturesBinaryPath},
  kHistogramBinaryPath_{kHistogramBinaryPath} {}


std::string ImageItem::ImageFilename() const {
  const auto kBoostImagePath = fs::path(this->kImagePath_);
  return kBoostImagePath.filename().string();
}


std::string ImageItem::ImageFilestem() const {
  const auto kBoostImagePath = fs::path(this->kImagePath_);
  return kBoostImagePath.stem().string();
}


cv::Mat ImageItem::LoadImage() const {
  return cv::imread(this->kImagePath_, CV_LOAD_IMAGE_COLOR);
}


std::string ImageItem::FeaturesBinaryFilename() const {
  const auto kBoostFeaturesBinaryPath = fs::path(this->kFeaturesBinaryPath_);
  return kBoostFeaturesBinaryPath.filename().string();
}


cv::Mat ImageItem::LoadFeatures() const {
  return ReadMatFromBinary(this->kFeaturesBinaryPath_);
}


std::string ImageItem::HistogramBinaryFilename() const {
  const auto kBoostHistogramBinaryPath = fs::path(this->kHistogramBinaryPath_);
  return kBoostHistogramBinaryPath.filename().string();
}


Histogram<float> ImageItem::LoadHistogram() const {
  return ReadFromBinary<float>(this->kHistogramBinaryPath_);
}

} // namespace igg
