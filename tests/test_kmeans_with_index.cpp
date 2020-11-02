/**
 * @file test_kmeans_with_index.cpp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <random>
#include <gtest/gtest.h>

#include "clustering/feature_point.hpp"
#include "clustering/kmeans_with_index/random_tree.hpp"

#include "make_clustering_test_data.hpp"


namespace igg {

TEST(ClusteringTest, BuildRandomTree) {
  // Use a certain seed to get reproduceable results
  const int kSeed = 0;
  std::mt19937 engine(kSeed);
  const auto kPointSet = MakeClusteringTestData(engine);

  //const int kNumIterations = 10;
  //const size_t kNumClusters = 5;
  //const bool kVerbose = false;

  //ClusteringStrategyKmeansVers2<float> kmeans
    //(kNumClusters, kNumIterations, kVerbose);
  //kmeans.ClusterCentroids(kPointSet);
}


} // namespace igg
