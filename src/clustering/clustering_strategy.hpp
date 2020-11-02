#ifndef CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_HPP_
#define CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_HPP_

/**
 * @file clustering_strategy.hpp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include "feature_point.hpp"


namespace igg {

/**
 * The purpose of this class is to provide an abstract interface for various
 * implementation of clustering methods. These methods are expected to provide
 * cluster centroids.
 */
template <class T>
class ClusteringStrategy {

public:
  virtual std::vector<FeaturePoint<T>> ClusterCentroids
    (const std::vector<FeaturePoint<T>>& kPointSet) const = 0;

};

} // namespace igg

#endif // CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_HPP_
