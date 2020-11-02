#ifndef CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_KMEANS_VERS_2_HPP_
#define CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_KMEANS_VERS_2_HPP_

/**
 * @file clustering_strategy_kmeans_vers_2.hpp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include "clustering_strategy.hpp"


/**
 * Provides a wrapper around an alternative implementation of K-means.
 */
namespace igg {

template <class T>
class ClusteringStrategyKmeansVers2: public ClusteringStrategy<T> {

public:
  ClusteringStrategyKmeansVers2
    (const size_t kNumClusters,
     const int kNumIterations,
     const bool kVerbose);

  std::vector<FeaturePoint<T>> ClusterCentroids
    (const std::vector<FeaturePoint<T>>& kPointSet) const override;

private:
  const size_t kNumClusters_;
  const int kNumIterations_;
  const bool kVerbose_;

};

} // namespace igg

#include "clustering_strategy_kmeans_vers_2.ipp"

#endif // CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_KMEANS_VERS_2_HPP_
