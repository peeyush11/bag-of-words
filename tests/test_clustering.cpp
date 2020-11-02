#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <numeric>
#include <random>
#include <type_traits>
#include <typeinfo>
#include <stdexcept>
#include <boost/filesystem.hpp>

#include "clustering/feature_point.hpp"
#include "clustering/clustering_strategy_kmeans.hpp"
#include "clustering/clustering_strategy_kmeans_vers_2.hpp"
#include "clustering/clustering_strategy_kmeans_opencv.hpp"
#include "clustering/clustering_strategy_kmeans_with_index.hpp"
#include "tools/sampling.hpp"
#include "tools/terminalout.hpp"
#include "clustering/kmeans_with_index/index.hpp"

#include "make_clustering_test_data.hpp"


namespace igg {

namespace fs = boost::filesystem;

TEST(ClusteringTest, FeaturePointNearestNeighbor) {
  const FeaturePoint<float> kQueryPoint{2.1f, 4.7f, 4.8f};

  const FeaturePoint<float> kPoint1{1.3f, 2.0f, 3.0f};
  const FeaturePoint<float> kPoint2{2.0f, 4.6f, 5.0f};
  const FeaturePoint<float> kPoint3{3.0f, 3.0f, 1.9f};

  const std::vector<FeaturePoint<float>> kPointSet
    {kPoint1, kPoint2, kPoint3};

  const auto kNearestPointIndex = NearestNeighbor(kQueryPoint, kPointSet);

  EXPECT_EQ(kNearestPointIndex, static_cast<size_t>(1)); // Using cast so the compiler does not warn about signed/unsigned comparision
}


TEST(ClusteringTest, Kmeans) {
  // Use a certain seed to get reproduceable results
  const int kSeed = 0;
  std::mt19937 engine(kSeed);
  const auto kPointSet = MakeClusteringTestData(engine);

  const int kNumIterations = 10;
  const float kEpsilon = 1e-3f;
  const size_t kNumClusters = 5;
  const bool kVerbose = false;

  ClusteringStrategyKmeans<float> kmeans
    (kNumClusters, kNumIterations, kEpsilon, kSeed, kVerbose);
  kmeans.ClusterCentroids(kPointSet);
}


TEST(ClusteringTest, KmeansVers2) {
  // Use a certain seed to get reproduceable results
  const int kSeed = 0;
  std::mt19937 engine(kSeed);
  const auto kPointSet = MakeClusteringTestData(engine);

  const int kNumIterations = 10;
  const size_t kNumClusters = 5;
  const bool kVerbose = false;

  ClusteringStrategyKmeansVers2<float> kmeans
    (kNumClusters, kNumIterations, kVerbose);
  kmeans.ClusterCentroids(kPointSet);
}


TEST(ClusteringTest, KmeansOpenCV) {
  // Use a certain seed to get reproduceable results
  const int kSeed = 0;
  std::mt19937 engine(kSeed);
  const auto kPointSet = MakeClusteringTestData(engine);

  const int kNumIterations = 10;
  const double kEpsilon = 1e-3;
  const size_t kNumClusters = 5;
  const int kAttempts = 1;
  const bool kVerbose = false;

  ClusteringStrategyKmeansOpenCV<float> kmeans
    (kNumClusters, kNumIterations, kEpsilon, kAttempts, kVerbose);
  kmeans.ClusterCentroids(kPointSet);
}


TEST(ClusteringTest, FeaturePointFlatten) {
  const FeaturePoint<float> kPoint1{1.3f, 2.0f, 3.0f};
  const FeaturePoint<float> kPoint2{2.0f, 4.6f, 5.0f};
  const FeaturePoint<float> kPoint3{3.0f, 3.0f, 1.9f};

  const std::vector<FeaturePoint<float>> kPointSet1
    {kPoint1, kPoint2, kPoint3};

  const FeaturePoint<float> kPoint4{1.3f, 2.0f, 3.0f};
  const FeaturePoint<float> kPoint5{2.0f, 4.6f, 5.0f};
  const FeaturePoint<float> kPoint6{3.0f, 3.0f, 1.9f};

  const std::vector<FeaturePoint<float>> kPointSet2
    {kPoint4, kPoint5, kPoint6};

  std::vector<std::vector<FeaturePoint<float>>> set_of_point_sets
    {kPointSet1, kPointSet2};

  const auto kFlattenedSet = Flatten(std::move(set_of_point_sets));

  EXPECT_EQ(kFlattenedSet.size(), static_cast<size_t>(6));

  EXPECT_FLOAT_EQ(kFlattenedSet[0][1], 2.0f);
  EXPECT_FLOAT_EQ(kFlattenedSet[1][1], 4.6f);
  EXPECT_FLOAT_EQ(kFlattenedSet[5][2], 1.9f);
}


TEST(ClusteringTest, FeaturePointFromMat) {
  // Arguments of cv::Mat::reshape: channels, rows
  auto mat = cv::Mat_<float>({0.0f, 1.1f, 2.2f, 3.3f, 4.4f, 5.5f}).reshape(0, 2);

  const auto kPointSet = FromMat<float>(std::move(mat));

  EXPECT_FLOAT_EQ(kPointSet[0][2], 2.2f);
  EXPECT_FLOAT_EQ(kPointSet[1][1], 4.4f);
}


TEST(ClusteringTest, FeaturePointToMat) {
  const FeaturePoint<float> kPoint1{1.3f, 2.0f};
  const FeaturePoint<float> kPoint2{2.0f, 4.6f};
  const FeaturePoint<float> kPoint3{3.1f, 3.0f};

  std::vector<FeaturePoint<float>> point_set
    {kPoint1, kPoint2, kPoint3};

  const auto kMat = ToMat(std::move(point_set));

  EXPECT_EQ(kMat.rows, 3);
  EXPECT_EQ(kMat.cols, 2);

  EXPECT_FLOAT_EQ(kMat.at<float>(0, 1), 2.0f);
  EXPECT_FLOAT_EQ(kMat.at<float>(2, 0), 3.1f);
}


TEST(ClusteringTest, GetPointers) {
  const FeaturePoint<float> kPoint1{1.3f, 2.0f};
  const FeaturePoint<float> kPoint2{2.0f, 4.6f};
  const FeaturePoint<float> kPoint3{3.1f, 3.0f};

  std::vector<FeaturePoint<float>> point_set
    {kPoint1, kPoint2, kPoint3};

  const auto kPointers = GetPointers(point_set);

  EXPECT_EQ(&point_set[0], kPointers[0]);
  EXPECT_EQ(&point_set[1], kPointers[1]);
  EXPECT_EQ(&point_set[2], kPointers[2]);
}


TEST(ClusteringTest, BuildIndexAndSearch) {
  // Use a certain seed to get reproduceable results
  const int kSeed = 0;

  // Test data, this time with some more dimensions
  const size_t kNumFeatures = 32;
  const size_t kNumClusters = 16;
  const float kMinValue = -10.0f;
  const float kMaxValue = 10.0f;
  const float kMinStd = 0.5f;
  const float kMaxStd = 0.5f;
  const size_t kMinNumSamplesPerCluster = 64;
  const size_t kMaxNumSamplesPerCluster = 64;

  std::mt19937 engine(kSeed);
  const auto kPointSet = MakeClusteringTestData
    (engine,
     kNumFeatures,
     kNumClusters,
     kMinValue,
     kMaxValue,
     kMinStd,
     kMaxStd,
     kMinNumSamplesPerCluster,
     kMaxNumSamplesPerCluster);

  const auto kPointersToPointSet = GetPointers(kPointSet);

  const size_t kNumTrees = 10;
  const size_t kSearchLimit = 10;
  const size_t kNumSplitDimensions = 5;

  Index<FeaturePoint<float>> index(kPointersToPointSet, kNumTrees, kSearchLimit, kNumSplitDimensions, kSeed);
  const auto kQueryPoint = kPointSet[512];

  EXPECT_NO_THROW(index.ApproximateNearestNeighbor(kQueryPoint));
}


TEST(ClusteringTest, KmeansWithIndex) {
  // Use a certain seed to get reproduceable results
  const int kSeed = 0;

  // Test data, this time with some more dimensions
  const size_t kNumFeatures = 32;
  const size_t kNumClusters = 16;
  const float kMinValue = -10.0f;
  const float kMaxValue = 10.0f;
  const float kMinStd = 0.5f;
  const float kMaxStd = 0.5f;
  const size_t kMinNumSamplesPerCluster = 64;
  const size_t kMaxNumSamplesPerCluster = 64;

  std::mt19937 engine(kSeed);
  const auto kPointSet = MakeClusteringTestData
    (engine,
     kNumFeatures,
     kNumClusters,
     kMinValue,
     kMaxValue,
     kMinStd,
     kMaxStd,
     kMinNumSamplesPerCluster,
     kMaxNumSamplesPerCluster);

  const auto kPointersToPointSet = GetPointers(kPointSet);

  const int kNumIterations = 25;
  const float kEpsilon = 0.001;
  const size_t kNumTrees = 10;
  const size_t kSearchLimit = 10;
  const size_t kNumSplitDimensions = 5;

  const bool kVerbose = false;

  ClusteringStrategyKmeansWithIndex<float> kmeans
    (kNumClusters, kNumIterations, kEpsilon, kNumTrees, kSearchLimit, kNumSplitDimensions, kSeed, kVerbose);


  EXPECT_NO_THROW(kmeans.ClusterCentroids(kPointSet));
}


} // namespace igg

