#ifndef CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_KMEANS_WITH_INDEX_HPP_
#define CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_KMEANS_WITH_INDEX_HPP_

/**
 * @file clustering_strategy_kmeans_with_index.hpp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <random>

#include "clustering_strategy.hpp"


namespace igg {

/**
 * K-Means using an index based on random trees.
 */
template <class T>
class ClusteringStrategyKmeansWithIndex: public ClusteringStrategy<T> {

public:
  ClusteringStrategyKmeansWithIndex
    (const size_t kNumClusters,
     const int kNumIterations,
     const T kEpsilon,
     const size_t kNumTrees,
     const size_t kSearchLimit,
     const size_t kNumSplitDimensions,
     const int kSeed,
     const bool kVerbose);

  std::vector<FeaturePoint<T>> ClusterCentroids
    (const std::vector<FeaturePoint<T>>& kPointSet) const override;

private:
  const size_t kNumClusters_;
  const int kNumIterations_;
  const T kEpsilon_;
  const size_t kNumTrees_;
  const size_t kSearchLimit_;
  const size_t kNumSplitDimensions_;
  const int kSeed_;
  const bool kVerbose_;

  std::vector<FeaturePoint<T>> InitCentroids
    (const std::vector<FeaturePoint<T>>& kPointSet) const;

  std::vector<FeaturePoint<T>> SubsamplePointSet
    (const std::vector<FeaturePoint<T>>& kPointSet, const size_t kSubSampleSize) const;
};

} // namespace igg

#include "clustering_strategy_kmeans_with_index.ipp"

#endif // CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_KMEANS_WITH_INDEX_HPP_
