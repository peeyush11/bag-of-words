#ifndef CPP_FINAL_PROJECT_WEB_WEB_HPP_
#define CPP_FINAL_PROJECT_WEB_WEB_HPP_

#include <iostream>
#include <boost/filesystem.hpp>

#include "dataset/image_item.hpp"


namespace igg {

namespace fs = boost::filesystem;

/**
 * Plot the histogram representation associated with this image in the dataset.
 *
 * Throws an instance of std::runtime_error if the histogram cannot be loaded.
 *
 * @return The plot as cv::Mat, three channels.
 */
cv::Mat MakeHistogramPlot
  (const std::shared_ptr<const ImageItem> kImageItem,
   const int kPlotHeight, const int kPlotWidth);

/**
 * Generate a .css style file to format the web output.
 *
 * @param kStyleFilePath Where to write the file.
 *
 * @return True if writing was successful.
 */
bool WriteStyleFile(const fs::path& kStyleFilePath);

/**
 * Rescale an image proportionally and write the result to a file.
 *
 * @param kNewWidth The desired width of the image.
 * @param kPath Where to save the image.
 */
void RescaleAndSave
  (const cv::Mat& kImage, const int kNewWidth, const fs::path kPath);

} // namespace igg

#endif // CPP_FINAL_PROJECT_WEB_WEB_HPP_
