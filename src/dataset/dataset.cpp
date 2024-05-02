
#include "dataset.hpp"

#include <iostream>

#include "histogram/histogram.hpp"
#include "clustering/feature_point.hpp"


namespace igg {

namespace fs = boost::filesystem;

Dataset::Dataset(const std::string kDir):
  kImagesDir_{fs::path(kDir)/"images/"},
  kResultsDir_{fs::path(kDir)/"results/"},
  kCentroidsPath_(fs::path(kDir)/"results"/"centroids.binary"),
  kHistogramWeightsPath_(fs::path(kDir)/"results"/"histogram_weights.binary"),
  kWebDir_{fs::path(kDir)/"web/"}
{
  if (!fs::exists(fs::path(kDir))) {
    throw std::runtime_error
      ("Cannot create dataset. The provided directory does not exist.");
  }

  if (!fs::exists(kImagesDir_)) {
    throw std::runtime_error
      ("Cannot create dataset. The does not seem to be a "
       "subdirectors images/ in the provided directory.");
  }

  // Iterate through images subdirectory and add all png files
  std::string image_path;
  std::string image_stem; // Filename without suffix
  // Generate paths based on the image name to store intermediate results
  std::string features_binary_path;
  std::string histogram_binary_path;
  for (const auto& kDirEntry : fs::recursive_directory_iterator(kImagesDir_)) {
    if (!fs::is_directory(kDirEntry) && kDirEntry.path().extension()==".png") {
      image_path = kDirEntry.path().string();
      image_stem = kDirEntry.path().stem().string();
      features_binary_path = this->kResultsDir_.string();
      features_binary_path.append(image_stem).append("_features.binary");
      histogram_binary_path = this->kResultsDir_.string();
      histogram_binary_path.append(image_stem).append("_histogram.binary");
      items_.emplace_back(std::make_shared<ImageItem>
        (image_path,
         features_binary_path,
         histogram_binary_path));
    }
  }

  // Sort by image paths
  std::sort(items_.begin(), items_.end(), CompareItems);

  // Create separate subdirectories for intermediate results and web output
  if (!fs::exists(kResultsDir_)) {fs::create_directory(kResultsDir_);}
  if (!fs::exists(kWebDir_)) {fs::create_directory(kWebDir_);}
}


bool Dataset::CompareItems(const std::shared_ptr<const ImageItem> item_1,
                           const std::shared_ptr<const ImageItem> item_2) {
  return item_1->ImagePath()<item_2->ImagePath();
}


bool Dataset::AllItemsHaveFeatures() const {
  for (const auto& kItem: this->items_) {
    if (!kItem->HasFeatures()) {return false;}
  }
  return true;
}


bool Dataset::AllItemsHaveHistograms() const {
  for (const auto& kItem: this->items_) {
    if (!kItem->HasHistogram()) {return false;}
  }
  return true;
}


// Initialize pointer to default dataset as nullptr
// default dataset is loaded when Dataset::Default()
// is called for the first time
std::shared_ptr<Dataset> Dataset::default_dataset_ = nullptr;

std::shared_ptr<const Dataset> Dataset::Default() {
  // if default dataset already loaded return pointer
  if (Dataset::default_dataset_) {return Dataset::default_dataset_;}

  // get path to data dir from environment variables
  const char* kCstrDataDir = std::getenv("CPP_FINAL_PROJECT_DATA_DIR");

  // if environment varibale not set print an error and return nullptr
  if(!kCstrDataDir) {
    std::cerr <<
      "Cannot load default dataset. "
      "Environment variable 'CPP_FINAL_PROJECT_DATA_DIR' not set. "
      "Return nullptr." << "\n";
    return nullptr;
  }

  // load default dataset
  Dataset::default_dataset_ = std::make_shared<Dataset>(std::string(kCstrDataDir));
  return Dataset::default_dataset_;
}


std::vector<FeaturePoint<float>> Dataset::LoadCentroids() const {
  return ReadCentroidsFromBinary<float>(this->kCentroidsPath_.string());
}


std::vector<float> Dataset::LoadHistogramWeights() const {
  return ReadFromBinary<float>(this->kHistogramWeightsPath_.string());
}

} // namespace igg

