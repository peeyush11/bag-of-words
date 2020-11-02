#ifndef CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_KMEANS_HPP_
#define CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_KMEANS_HPP_

/**
 * @file clustering_strategy_kmeans.hpp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <random>

#include "clustering_strategy.hpp"


namespace igg {

/**
 * A straighfowrd implementation of K-Means.
 */
template <class T>
class ClusteringStrategyKmeans: public ClusteringStrategy<T> {

public:
  /**
   * Constructor.
   *
   * Note that the actual clustering is only performed after calling ClusterCentroids.
   *
   * @param kNumClusters Number of clusters.
   * @param kNumIterations Maximum number of iterations.
   * @param kEpsilon Early stopping if all centroid updates are smaller than this value.
   * @param kSeed For initialization of centroids, which are randomly selected from the
   * provided point set. Note that also multiple sequential calls of ClusterCentroids
   * will always use the same seed.
   * @param kVerbose If true, print some output to the terminal.
   */
  ClusteringStrategyKmeans
    (const size_t kNumClusters,
     const int kNumIterations,
     const T kEpsilon,
     const int kSeed,
     const bool kVerbose);

  /**
   * Perform the actual clustering.
   *
   * Note this may take some time to execute, especially for large point sets.
   *
   * Throws an instance of std::runtime_error if the point set is empty or the
   * number of clusters is larger than the number of elements in the point set.
   *
   * @param Points to cluster. All are expected to have the same number of dimensions.
   *
   * returns The cluster centroids as determined by the algorithm.
   */
  std::vector<FeaturePoint<T>> ClusterCentroids
    (const std::vector<FeaturePoint<T>>& kPointSet) const override;

private:
  const size_t kNumClusters_;
  const int kNumIterations_;
  const T kEpsilon_;
  const int kSeed_;
  const bool kVerbose_;

  std::vector<FeaturePoint<T>> InitCentroids
    (const std::vector<FeaturePoint<T>>& kPointSet) const;

  std::vector<FeaturePoint<T>> SubsamplePointSet
    (const std::vector<FeaturePoint<T>>& kPointSet, const size_t kSubSampleSize) const;

  // For debugging only
  cv::Mat MakePlot
    (const std::vector<std::vector<FeaturePoint<T> const *>>& kClusters,
     const std::vector<FeaturePoint<T>>& kCentroids);

};

} // namespace igg

#include "clustering_strategy_kmeans.ipp"

#endif // CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_KMEANS_HPP_
