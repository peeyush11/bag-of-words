#ifndef CPP_FINAL_PROJECT_DATASET_DATASET_HPP_
#define CPP_FINAL_PROJECT_DATASET_DATASET_HPP_

#include <vector>
#include <string>
#include <memory>
#include <boost/filesystem.hpp>

#include "image_item.hpp"


namespace igg {

namespace fs = boost::filesystem;

/**
 * This class abstract as set of images files.
 *
 * It provides the filepaths to the images itself as well as to various intermediate results.
 *
 * The default dataset is expected to be defined by setting an environment
 * variable named CPP_FINAL_PROJECT_DATA_DIR, e.g:
 *
 *   export CPP_FINAL_PROJECT_DATA_DIR=/home/someone/data/<dataset-root-dir>
 *
 * Usage:
 *
 *   const auto kDataset = igg::Dataset::Default();
 *   kDataset->Items()[0]->ImagePath(); // Get the path to the first image
 *   const auto kImage = kDataset->Items()[1]->LoadImage(); // Load the second image
 *
 * The underlying filestructure looks like:
 *
 *   <dataset-root-dir>/
 *       |_ images/
 *       |    |_ <images in .png format>
 *       |
 *       |_ results/
 *       |    |_ centroids.binary
 *       |    |_ histogram_weights.binary
 *       |    |_ <one binary file with extracted features for each image>
 *       |
 *       |_ web/
 *            |_ images/
 *            |    |_ <rescaled images and histogram plots in .jpg format>
 *            |
 *            |_ index.html
 *            |_ style.css
 *
 * Where initially the only important thing is to place the images in .png format
 * in the images/ directory.
 */
class Dataset {
public:

  /**
   * Construct a Dataset abstraction of a given directory.
   *
   * An instance of std::runtime_error is thrown if the directory does not exists
   * or does not have the required structure.
   *
   * Please refer to the class description  for details.
   */
  Dataset (std::string dir);

  /**
   * Provide representations of all images in this dataset.
   */
  std::vector<std::shared_ptr<const ImageItem>> Items() const {return this->items_;}

  /**
   * Path to the subdirectory where the image files are stored.
   */
  std::string ImagesDir() const {return this->kImagesDir_.string();}

  /**
   * Path to the subdirectory where intermediate results are stored are stored.
   */
  std::string ResultsDir() const {return this->kResultsDir_.string();}

  /**
   * Path to the subdirectory where the final web output is stored.
   */
  std::string WebDir() const {return this->kWebDir_.string();}

  /**
   * Check if for all items in this dataset there exists a corresponding binary file with
   * the extracted image features.
   */
  bool AllItemsHaveFeatures() const;

  /**
   * Check if for all items in this dataset there exists a corresponding binary file with
   * the histogram computed after clustering the image features.
   */
  bool AllItemsHaveHistograms() const;

  /**
   * Path to the file where the cluster centroids are stored.
   *
   * Note that this file does not necessarily exist yet.
   */
  std::string CentroidsPath() const {return this->kCentroidsPath_.string();}

  /**
   * Check if a binary file with clustering centroids exists.
   */
  bool HasCentroids() const {return FileExists(this->kCentroidsPath_.string());}

  /**
   * Loads the cluster centroid from the binary file.
   *
   * Throws a std::runtime_error in case the file cannot be read.
   */
  std::vector<FeaturePoint<float>> LoadCentroids() const;

  /**
   * Path where histogram weights for the overall dataset are stored.
   *
   * Note that this file does not necessarily exist yet.
   */
  std::string HistogramWeightsPath() const {return this->kHistogramWeightsPath_.string();}

  /**
   * Check if a binary file with histogram weights exists.
   */
  bool HasHistogramWeights() const {return FileExists(this->kHistogramWeightsPath_.string());}

  /**
   * Loads the histogram weights from the binary file.
   *
   * Throws a std::runtime_error in case the file cannot be read.
   */
  std::vector<float> LoadHistogramWeights() const;


  /**
   * Provide a pointer to the defined default dataset.
   *
   * Note the Dataset is only loaded when required and only provided once for each program running.
   *
   * The default dataset is automatically loaded in the executables provided by this project.
   *
   * Please refer to the class description for details.
   */
  static std::shared_ptr<const Dataset> Default();

private:
  const fs::path kImagesDir_;
  const fs::path kResultsDir_;
  const fs::path kCentroidsPath_;
  const fs::path kHistogramWeightsPath_;
  const fs::path kWebDir_;
  std::vector<std::shared_ptr<const ImageItem>> items_;

  static std::shared_ptr<Dataset> default_dataset_;

  // Used to sort items by filename in lexicographical order
  static bool CompareItems
    (const std::shared_ptr<const ImageItem> item_1,
     const std::shared_ptr<const ImageItem> item_2);

};

} // namespace igg

#endif // CPP_FINAL_PROJECT_DATASET_DATASET_HPP_
