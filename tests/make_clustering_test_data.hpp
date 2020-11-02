#ifndef CPP_FINAL_PROJECT_TESTS_MAKE_CLUSTERING_TEST_DATA_HPP_
#define CPP_FINAL_PROJECT_TESTS_MAKE_CLUSTERING_TEST_DATA_HPP_

#include "tools/sampling.hpp"

namespace igg {

// Helper function template to generate test data
template <class T>
std::vector<FeaturePoint<T>> MakeClusteringTestData
  (const std::vector<std::vector<T>> &kMeans,
   const std::vector<std::vector<T>> &kStds,
   const std::vector<size_t> &kNumSamplesPerCluster,
   std::mt19937& engine)
{
  static_assert
    (std::is_floating_point<T>::value,
     "Float type required.");

  const size_t kNumClusters = kMeans.size();

  // Overall number of feature points
  const size_t kNumSamples = std::accumulate
    (kNumSamplesPerCluster.begin(), kNumSamplesPerCluster.end(), 0);

  // Sample feature points
  std::vector<FeaturePoint<T>> point_set;
  point_set.reserve(kNumSamples);
  size_t num_samples_in_current_cluster = 0;
  for (size_t cluster_index = 0; cluster_index<kNumClusters; cluster_index++) {
    num_samples_in_current_cluster = kNumSamplesPerCluster[cluster_index];
    for(size_t do_not_use = 0;
        do_not_use<num_samples_in_current_cluster;
        do_not_use++)
    {
      auto sample = SampleVectorFromMultivariateNormalDistrubution<T>
        (kMeans[cluster_index],
         kStds[cluster_index],
         engine);
      point_set.emplace_back(sample);
    }
  }

  // Shuffle feature points
  std::shuffle(std::begin(point_set), std::end(point_set), engine);

  return point_set;
}


std::vector<FeaturePoint<float>> MakeClusteringTestData
  (std::mt19937& engine,
   const size_t kNumFeatures = 2,
   const size_t kNumClusters = 5,
   const float kMinValue = -10.0f,
   const float kMaxValue = 10.0f,
   const float kMinStd = 0.5f,
   const float kMaxStd = 0.5f,
   const size_t kMinNumSamplesPerCluster = 10,
   const size_t kMaxNumSamplesPerCluster = 10)
{

  // Sample mean, standard deviation for each cluster
  const auto kClusterMeans =
    SampleVectorsFromUniformDistribution<float>
      (kNumClusters, kNumFeatures, kMinValue, kMaxValue, engine);

  const auto kClusterStds =
    SampleVectorsFromUniformDistribution<float>
      (kNumClusters, kNumFeatures, kMinStd, kMaxStd, engine);

  // Sample number of data points for each cluster
  const auto kNumSamplesPerCluster =
    SampleVectorFromUniformDistribution<size_t>
      (kNumClusters, kMinNumSamplesPerCluster, kMaxNumSamplesPerCluster, engine);

  auto point_set = MakeClusteringTestData<float>
    (kClusterMeans, kClusterStds, kNumSamplesPerCluster, engine);

  return point_set;
}

} // namespace igg

#endif // CPP_FINAL_PROJECT_TESTS_MAKE_CLUSTERING_TEST_DATA_HPP_
