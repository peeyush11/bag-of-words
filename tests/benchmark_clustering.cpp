#include <benchmark/benchmark.h>
#include <vector>
#include <iostream>
#include <random>

#include "clustering/clustering_strategy_kmeans.hpp"
#include "clustering/clustering_strategy_kmeans_vers_2.hpp"
#include "clustering/clustering_strategy_kmeans_opencv.hpp"
#include "tools/sampling.hpp"
#include "make_clustering_test_data.hpp"


namespace igg {

std::vector<FeaturePoint<float>> MakeBenchmarkClusteringTestData() {
  const int kSeed = 0;
  std::mt19937 engine(kSeed);
  const size_t kNumFeatures = 128;
  const size_t kNumClusters = 100;
  const float kMinValue = 0.0f;
  const float kMaxValue = 1.0f;
  const float kMinStd = 0.3f;
  const float kMaxStd = 0.3f;
  const size_t kMinNumSamplesPerCluster = 100;
  const size_t kMaxNumSamplesPerCluster = 100;

  return MakeClusteringTestData
    (engine, kNumFeatures, kNumClusters, kMinValue,
     kMaxValue, kMinStd, kMaxStd,
     kMinNumSamplesPerCluster, kMaxNumSamplesPerCluster);
}

static void BM_Kmeans(benchmark::State& state) {
  // Make test data
  auto kTestPointSet = MakeBenchmarkClusteringTestData();

  // Initialize clustering
  const int kSeed = 0;
  const int kNumIterations = 100;
  const float kEpsilon = 1e-3f;
  const size_t kNumClusters = 100;
  const bool kVerbose = false;

  ClusteringStrategyKmeans<float> kmeans
    (kNumClusters, kNumIterations, kEpsilon, kSeed, kVerbose);

  // Perform benchmark
  for(auto _: state) {
    kmeans.ClusterCentroids(kTestPointSet);
  }
}


static void BM_KmeansVers2(benchmark::State& state) {
  // Make test data
  auto kTestPointSet = MakeBenchmarkClusteringTestData();

  // Initialize clustering
  const int kNumIterations = 100;
  const size_t kNumClusters = 100;
  const bool kVerbose = false;

  igg::ClusteringStrategyKmeansVers2<float> kmeans
    (kNumClusters, kNumIterations, kVerbose);

  // Perform benchmark
  for(auto _: state) {
    kmeans.ClusterCentroids(kTestPointSet);
  }
}


static void BM_KmeansOpenCV(benchmark::State& state) {
  // Make test data
  auto kTestPointSet = MakeBenchmarkClusteringTestData();

  // Initialize clustering
  const int kNumIterations = 100;
  const double kEpsilon = 1e-3f;
  const size_t kNumClusters = 100;
  const int kAttempts = 1;
  const bool kVerbose = false;

  ClusteringStrategyKmeansOpenCV<float> kmeans
    (kNumClusters, kNumIterations, kEpsilon, kAttempts, kVerbose);

  // Perform benchmark
  for(auto _: state) {
    kmeans.ClusterCentroids(kTestPointSet);
  }
}

BENCHMARK(BM_Kmeans);
BENCHMARK(BM_KmeansVers2);
BENCHMARK(BM_KmeansOpenCV);

} // namespace igg

BENCHMARK_MAIN();
