/**
 * @file clustering_strategy_kmeans_with_index.ipp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <cmath>
#include <algorithm>

#include "tools/sampling.hpp"
#include "tools/linalg.hpp"

#include "kmeans_with_index/index.hpp"

namespace igg {

template <class T>
ClusteringStrategyKmeansWithIndex<T>::ClusteringStrategyKmeansWithIndex
  (const size_t kNumClusters,
   const int kNumIterations,
   const T kEpsilon,
   const size_t kNumTrees,
   const size_t kSearchLimit,
   const size_t kNumSplitDimensions,
   const int kSeed,
   const bool kVerbose):
  kNumClusters_{kNumClusters},
  kNumIterations_{kNumIterations},
  kEpsilon_{kEpsilon},
  kNumTrees_{kNumTrees},
  kSearchLimit_{kSearchLimit},
  kNumSplitDimensions_{kNumSplitDimensions},
  kSeed_{kSeed},
  kVerbose_{kVerbose}
{}


template <class T>
std::vector<FeaturePoint<T>> ClusteringStrategyKmeansWithIndex<T>::ClusterCentroids
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

    // Build index
    // Get pointers of centroids
    const auto kCentroidPointers = GetPointers(centroids);

    Index<FeaturePoint<float>> index
      (kCentroidPointers, this->kNumTrees_, this->kSearchLimit_, this->kNumSplitDimensions_, this->kSeed_);

    if (this->kVerbose_) {std::cout << "  * Assign data points to nearest cluster.\n";}
    for (const auto& kPoint: kPointSet) {
      const auto kApproximateNearestCentroid = index.ApproximateNearestNeighbor(kPoint);
      nearest_cluster_index = std::distance
        (kCentroidPointers.begin(), std::find(kCentroidPointers.begin(), kCentroidPointers.end(), kApproximateNearestCentroid));

      //nearest_cluster_index = NearestNeighbor(kPoint, centroids);
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
std::vector<FeaturePoint<T>> ClusteringStrategyKmeansWithIndex<T>::SubsamplePointSet
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
std::vector<FeaturePoint<T>> ClusteringStrategyKmeansWithIndex<T>::InitCentroids
  (const std::vector<FeaturePoint<T>> &kPointSet) const
{
  return this->SubsamplePointSet(kPointSet, this->kNumClusters_);
}

} // namespace igg

