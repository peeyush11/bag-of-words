#include <gtest/gtest.h>
#include <iostream>

#include "bag_of_words.hpp"
#include "clustering/clustering_strategy_kmeans.hpp"

#include "make_test_dataset.hpp"


namespace igg {

TEST(BagOfWordsTest, BagOfWordsWithTestDataset) {
  const auto kDataset = std::make_shared<const Dataset>(MakeTestDataset());
  const BagOfWords kBagOfWords(kDataset, false); // False for no terminal output

  // We need a clustering strategy we can later pass to the instance of igg::BagOfWords
  const size_t kNumClusters = 10;
  const int kIterations = 25;
  const float kEpsilon = 1e-3f;
  const int kSeed = 0;
  const ClusteringStrategyKmeans<float> kStrategy
    (kNumClusters, kIterations, kEpsilon, kSeed, false); // False for no terminal output

  // We also need to define some parameters of the web output
  const size_t kNumExamples = 25;
  const size_t kNumExamplesSimilar = 5;
  const size_t kNumExamplesDifferent = 5;
  const unsigned int kExampleImageSize = 256;
  const unsigned int kExampleImageSizeSmall = 128;

  // The dictionary was not yet created, this should evaluate to false
  EXPECT_FALSE(kBagOfWords.DictionaryComplete());

  // If we try to perform clustering, compute historgams or generate web output
  // without having extracted the features, this should throw an exception
  EXPECT_THROW(kBagOfWords.ComputeClusterCentroids(kStrategy), DictionaryIncomplete);
  EXPECT_THROW(kBagOfWords.MakeHistograms(), DictionaryIncomplete);
  EXPECT_THROW(kBagOfWords.MakeWebOutput
    (kNumExamples, kNumExamplesSimilar, kNumExamplesDifferent,
     kExampleImageSize, kExampleImageSizeSmall),
    DictionaryIncomplete);

  // Now create the dictionary
  EXPECT_NO_THROW(kBagOfWords.CreateDictionary(kStrategy, false));
  EXPECT_TRUE(kBagOfWords.DictionaryComplete());

  // Generate web output
  EXPECT_NO_THROW(kBagOfWords.MakeWebOutput
    (kNumExamples, kNumExamplesSimilar, kNumExamplesDifferent,
     kExampleImageSize, kExampleImageSizeSmall));
}

} // namespace igg
