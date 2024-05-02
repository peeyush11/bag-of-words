
#include "web.hpp"

namespace igg {

cv::Mat MakeHistogramPlot
  (const std::shared_ptr<const ImageItem> kImageItem,
   const int kPlotHeight, const int kPlotWidth)
{
  if (!kImageItem->HasHistogram()) {
    throw std::runtime_error("Histogram of this image was not yet computed.");
  }

  const auto kHistogram = kImageItem->LoadHistogram();
  const auto kNumBins = kHistogram.size();
  const float kMaxValue = *std::max_element(kHistogram.begin(), kHistogram.end())+1e-5f;
  // Add epsilon to prevent zero division

  const float kBinWidth = static_cast<float>(kPlotWidth)/kNumBins;

  cv::Mat plot(kPlotHeight, kPlotWidth, CV_8UC3, cv::Scalar(220, 220, 220));

  for (size_t bin_index=0; bin_index<kNumBins; bin_index++) {
    const int kPositionLeft = static_cast<int>(bin_index*kBinWidth);
    const int kPositionRight = static_cast<int>((bin_index+1)*kBinWidth);
    const int kBarHeight = static_cast<int>(kHistogram[bin_index]/kMaxValue*kPlotHeight);
    const cv::Point kPoint1(kPositionLeft, kPlotHeight-kBarHeight);
    const cv::Point kPoint2(kPositionRight, kPlotHeight);
    cv::rectangle(plot, kPoint1, kPoint2, cv::Scalar(50, 50, 50), -1);
  }

  return plot;
}


bool WriteStyleFile(const fs::path& kStyleFilePath) {
  std::ofstream file(kStyleFilePath.string(), std::ofstream::out);
  if (!file.is_open()) {
    std::cerr << "Failed to open " << kStyleFilePath << "./n";
    return false;
  }

  file << ".dataset_item {\n";
  file << "  padding: 0px;\n";
  file << "  overflow: hidden;\n";
  file << "  margin-bottom: 50px;\n";
  file << "}\n";

  file << ".image_group {\n";
  file << "  padding: 0px;\n";
  file << "  overflow: hidden;\n";
  file << "  margin-bottom: 10px;\n";
  file << "}\n";

  file << ".image {\n";
  file << "  padding: 0px;\n";
  file << "  padding-right: 10px;\n";
  file << "  padding-top: 5px;\n";
  file << "  padding-bottom: 5px;\n";
  file << "  float: left;\n";
  file << "}\n";

  file << ".caption {\n";
  file << "  font-size: x-small;\n";
  file << "  font-style: normal;\n";
  file << "  font-weight: normal;\n";
  file << "  font-family: sans-serif;\n";
  file << "  margin: 0px;\n";
  file << "  padding: 0px;\n";
  file << "}\n";

  file << ".subtitle {\n";
  file << "  font-size: medium;\n";
  file << "  font-style: normal;\n";
  file << "  font-weight: normal;\n";
  file << "  font-family: sans-serif;\n";
  file << "  margin: 0px;\n";
  file << "  padding: 0px;\n";
  file << "}\n";

  file << ".title {\n";
  file << "  font-size: large;\n";
  file << "  font-style: normal;\n";
  file << "  font-weight: normal;\n";
  file << "  font-family: sans-serif;\n";
  file << "  margin: 0px;\n";
  file << "  margin-bottom: 10px;\n";
  file << "  padding: 0px;\n";
  file << "}\n";

  return true;
}


void RescaleAndSave
  (const cv::Mat& kImage, const int kNewWidth, const fs::path kPath)
{
  const int kScaledCols = kNewWidth;
  const int kScaledRows = static_cast<int>
    (static_cast<float>(kNewWidth)/kImage.cols*kImage.rows);

  cv::Mat image_scaled;
  cv::resize(kImage, image_scaled, cv::Size(kScaledCols, kScaledRows));

  cv::imwrite(kPath.string(), image_scaled);
}

} // namespace igg
