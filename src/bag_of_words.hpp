#ifndef CPP_FINAL_PROJECT_BAG_OF_WORDS_HPP_
#define CPP_FINAL_PROJECT_BAG_OF_WORDS_HPP_

/**
 * @file bag_of_words.hpp
 *
 * Note this is one of two alternative implementations.
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include "dataset/dataset.hpp"
#include "clustering/clustering_strategy.hpp"


namespace igg {

/**
 * Provides a bag of visual words approach for similarity search over a set of images.
 *
 * This class wraps around all the lower-level functionality contained in the project.
 */
class BagOfWords {
public:
  /*
   * Constructor.
   *
   * @param kDataset The dataset to work with.
   * @param kVerbose If true, terminal output is provided for most function calls.
   */
  BagOfWords(const std::shared_ptr<const Dataset> kDataset, const bool kVerbose);

  /*
   * Exectue all necessary steps to create a visual dicitionary for the given dataset.
   * This is a prequisite to perform similarity queries.
   *
   * Steps performed:
   * 1. Feature extraction
   * 2. Clustering
   * 3. Histogram generation and re-weighting
   *
   * If previous results are available (e.g. features have already been extracted),
   * there are re-used by default.
   *
   * Note that this function may overwrite results associated with the dataset
   * on the harddisk (e.g. extracted features, clustering results).
   *
   * @param kRecompute All previous results will be discarded and re-computed.
   * Use this option to experiment with different parameters.
   */
  void CreateDictionary
    (const ClusteringStrategy<float>& kStrategy, const bool kRecompute) const;


  /*
   * Get the similarity of the query image to each image in the dataset.
   *
   * An execption of type igg::DictionaryIncomplete is thrown if the visual dictionary
   * was not completely created beforehand.
   *
   * @param kQueryImage The query image.
   *
   * @return A vector of similarity measures. Order corresponds to the order
   * of the items in the dataset.
   */
  std::vector<float> Similarities
    (const std::shared_ptr<const ImageItem> kQueryItem) const;

  /*
   * Order images in the dataset by provided similarities.
   *
   * @param kSimilarity A vector of similarity measures. Its size and order
   * is expected to be equal to the number and order of images in the the dataset.
   * In case of a size mismatch an exception of type std::invalid_argument is thrown.
   *
   * @return A pair of ordered images and corresponding similarities (most similar first).
   */
  std::pair
    <std::vector<std::shared_ptr<const ImageItem>>,
     std::vector<float>>
    OrderItemsBySimilarity (const std::vector<float>& kSimilarities) const;


  /*
   * Check if the visual dictionary for similarity queries is in place.
   *
   * @return If true, a visual dictionary is available.
   */
  bool DictionaryComplete() const;

  /*
   * Execute the feature extraction step to create a visual dictionary of the given dataset.
   *
   * Note that this function may overwrite results associated with the dataset
   * on the harddisk.
   */
  void ExtractFeatures() const;

  /*
   * Execute the clustering step to create a visual dictionary of the given dataset.
   *
   * Note that this function may overwrite results associated with the dataset
   * on the harddisk.
   *
   * An execption of type igg::DictionaryIncomplete is thrown if features have not been
   * extracted yet.
   *
   * @param kStrategy The clustering strategy to use.
   */
  void ComputeClusterCentroids(const ClusteringStrategy<float>& kStrategy) const;

  /*
   * Execute the histogram generation and re-weighting step to create a visual dictionary
   * of the given dataset.
   *
   * Note that this function may overwrite results associated with the dataset
   * on the harddisk.
   *
   * An execption of type igg::DictionaryIncomplete is thrown if features have not been
   * extracted and clustered yet.
   */
  void MakeHistograms() const;

  /*
   * Generated a web output for the given dataset showing the most similar and most
   * different images for some example items.
   *
   * The result is available under <data-dir>/web/.
   *
   * Note that this function may overwrite results associated with the dataset
   * on the harddisk.
   *
   * An execption of type igg::DictionaryIncomplete is thrown if the visual dictionary
   * was not completely created beforehand.
   *
   * @param kNumExamples Number of examples.
   * @param kNumExamplesSimilar Number of most similar images shown. A good value may be 10.
   * @param kNumExamplesDifferent Number of most different images shown. A good value may be 10.
   * @param kExampleImageSize Size of the example images in web output. A good value may be 256.
   * @param kExampleImageSizeSmall Size of a smaller version of the example images in web output.
   * A good value may be 128.
   */
  void MakeWebOutput
    (const size_t kNumExamples,
     const size_t kNumExamplesSimilar,
     const size_t kNumExamplesDifferent,
     const unsigned int kExampleImageSize,
     const unsigned int kExampleImageSizeSmall) const;

  /*
   * Get the verbose property.
   *
   * @return If true, terminal output is provided for most function calls.
   */
  bool Verbose() const {return this->verbose_;}

  /*
   * Set the verbose property.
   *
   * @param kVerbose If true, terminal output is provided for most function calls.
   */
  void SetVerbose(const bool kVerbose) {this->verbose_ = kVerbose;}

private:
  const std::shared_ptr<const Dataset> kDataset_;
  bool verbose_;
};

/*
 * Thrown by igg::BagOfVisual words if a visual dictionary is required, however not available.
 */
struct DictionaryIncomplete: public std::runtime_error {
  DictionaryIncomplete(const std::string kWhat): std::runtime_error(kWhat) {}
};

} // namespace igg

#endif // CPP_FINAL_PROJECT_BAG_OF_WORDS_HPP_
