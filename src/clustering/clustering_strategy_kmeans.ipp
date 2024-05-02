

#include <cmath>
#include <algorithm>

#include "tools/sampling.hpp"
#include "tools/linalg.hpp"

namespace igg {

template <class T>
ClusteringStrategyKmeans<T>::ClusteringStrategyKmeans
  (const size_t kNumClusters,
   const int kNumIterations,
   const T kEpsilon,
   const int kSeed,
   const bool kVerbose):
  kNumClusters_{kNumClusters},
  kNumIterations_{kNumIterations},
  kEpsilon_{kEpsilon},
  kSeed_{kSeed},
  kVerbose_{kVerbose}
{}


template <class T>
std::vector<FeaturePoint<T>> ClusteringStrategyKmeans<T>::ClusterCentroids
  (const std::vector<FeaturePoint<T>>& kPointSet) const
{
  if (kPointSet.empty())
    {throw std::invalid_argument("Empty set of points.");}

  const auto kNumPoints = kPointSet.size();
  if (this->kVerbose_) {std::cout << "Number of points to cluster: " << kNumPoints << ".\n";}

  if (kNumPoints<this->kNumClusters_) {
    throw std::invalid_argument
      ("Number of clusters is larger than number of points.");
  }

  const auto kNumFeatures = kPointSet[0].size();
  auto centroids = this->InitCentroids(kPointSet);

  // Store updated centroids
  std::vector<FeaturePoint<T>> updated_centroids
    (this->kNumClusters_, FeaturePoint<T>(kNumFeatures));

  // Distance between centroids and updated centroids for early termination
  std::vector<T> deltas(this->kNumClusters_);

  // Initialize kNumClusters_ empty clusters (vector of pointers to const objects)
  std::vector<std::vector<FeaturePoint<T> const *>> clusters (this->kNumClusters_);

  // Reserve some memory for each cluster
  const auto kApproximateNumPointsPerCluster = kNumPoints/this->kNumClusters_;
  for (auto& cluster: clusters)
    {cluster.reserve(kApproximateNumPointsPerCluster);}

  size_t nearest_cluster_index; // Initialized inside loop
  T max_delta = std::numeric_limits<T>::max();
  int iteration = 0;

  while (true) {
    if (this->kVerbose_) {std::cout << "* Start iteration " << iteration << ".\n";}

    // Reset clusters
    for (auto& cluster: clusters)
      {cluster.clear();}

    if (this->kVerbose_) {std::cout << "  * Assign data points to nearest cluster.\n";}
    for (const auto& kPoint: kPointSet) {
      nearest_cluster_index = NearestNeighbor(kPoint, centroids);
      clusters[nearest_cluster_index].emplace_back(&kPoint);
    }

    // Shrink memory of each cluster
    //for (auto& cluster: clusters)
      //{cluster.shrink_to_fit();}

    // For debugging only
    //this->MakePlot(clusters, centroids);

    if (max_delta < this->kEpsilon_) {
      if (this->kVerbose_)
        {std::cout << "  * Max update smaller than epsilon (" << this->kEpsilon_ << "). Terminate.\n";}
      break;
    }

    if (iteration >= this->kNumIterations_) {
      if (this->kVerbose_)
        {std::cout << "  * Reached maximum number of iterations (" << this->kNumIterations_ << "). Terminate.\n";}
      break;
    }

    if (this->kVerbose_) {std::cout << "  * Update centroids.\n";}
    for (size_t cluster_index = 0; cluster_index<this->kNumClusters_; cluster_index++) {
      if (clusters[cluster_index].size()>0) {
        updated_centroids[cluster_index] = Centroid<FeaturePoint<T>>(clusters[cluster_index]);
      } else {
        // No update is cluster is empty
        updated_centroids[cluster_index] = centroids[cluster_index];
      }
      deltas[cluster_index] = std::sqrt(SquaredL2Norm<FeaturePoint<T>>
        (Difference<FeaturePoint<T>>(updated_centroids[cluster_index], centroids[cluster_index])));
    }

    // Use updated centroids for next iteration
    centroids = updated_centroids;

    max_delta = *std::max_element(deltas.begin(), deltas.end());
    if (this->kVerbose_) {std::cout << "  * Max update: " << max_delta << ".\n";}

    iteration++;
  }

  return centroids;
}


template <class T>
std::vector<FeaturePoint<T>> ClusteringStrategyKmeans<T>::SubsamplePointSet
  (const std::vector<FeaturePoint<T>>& kPointSet, const size_t kSubsampleSize) const
{
  const size_t kNumPoints = kPointSet.size();

  // Seed random number generator
  std::mt19937 engine(this->kSeed_);

  // Sample kNumClusters_ different indices
  const auto  kIndices = SampleIndicesWithoutReplacement<size_t>
    (this->kNumClusters_, kNumPoints, engine);

  // Collect points
  std::vector<FeaturePoint<T>> subsample;
  subsample.reserve(kSubsampleSize);

  for (const auto kIndex: kIndices) {
    subsample.emplace_back(kPointSet[kIndex]);
  }

  return subsample;
}


template <class T>
std::vector<FeaturePoint<T>> ClusteringStrategyKmeans<T>::InitCentroids
  (const std::vector<FeaturePoint<T>> &kPointSet) const
{
  return this->SubsamplePointSet(kPointSet, this->kNumClusters_);
}


template <class T>
cv::Mat ClusteringStrategyKmeans<T>::MakePlot
  (const std::vector<std::vector<FeaturePoint<T> const *>>& kClusters,
   const std::vector<FeaturePoint<T>>& kCentroids)
{
  const T kMaxValue = static_cast<T>(10);
  const T kMinValue = static_cast<T>(-10);
  const int kPlotSize = 500;

  cv::Mat plot = cv::Mat::zeros(kPlotSize, kPlotSize, CV_8UC3);

  int x = 0;
  int y = 0;
  size_t color_index = 0;
  const T kRange = kMaxValue-kMinValue;
  const std::vector<cv::Scalar> kClusterColors
    {cv::Scalar(255, 0, 0),
     cv::Scalar(0, 255, 0),
     cv::Scalar(0, 0, 255),
     cv::Scalar(255, 255, 0),
     cv::Scalar(0, 255, 255),
     cv::Scalar(255, 0, 255)};
  const size_t kNumColors = kClusterColors.size();

  // Draw points
  for(const auto& kCluster: kClusters) {
    for(const auto& kPoint: kCluster) {
      x = static_cast<int>(((*kPoint)[0]-kMinValue)/kRange*kPlotSize);
      y = static_cast<int>(((*kPoint)[1]-kMinValue)/kRange*kPlotSize);
      cv::drawMarker(plot, cv::Point(x, y), kClusterColors[color_index], cv::MARKER_CROSS, 5, 1);
    }
    color_index++; if (color_index>=kNumColors) {color_index = 0;}
  }

  // Draw centroids
  color_index = 0;
  for(const auto& kCentroid: kCentroids) {
    x = static_cast<int>((kCentroid[0]-kMinValue)/kRange*kPlotSize);
    y = static_cast<int>((kCentroid[1]-kMinValue)/kRange*kPlotSize);
    cv::drawMarker(plot, cv::Point(x, y), kClusterColors[color_index], cv::MARKER_STAR, 5, 2);
    color_index++; if (color_index>=kNumColors) {color_index = 0;}
  }

  cv::imshow("Clustering", plot);
  cv::waitKey();

  return plot;
}

} // namespace igg

