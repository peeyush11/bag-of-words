#ifndef CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_KMEANS_OPENCV_HPP_
#define CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_KMEANS_OPENCV_HPP_

/**
 * @file clustering_strategy_kmeans_opencv.hpp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include "clustering_strategy.hpp"


namespace igg {

/**
 * Provides a wrapper around the OpenCV implementation of K-means
 * to make it compatible with the rest of the framework.
 */
template <class T>
class ClusteringStrategyKmeansOpenCV: public ClusteringStrategy<T> {

public:

  /**
   * Constructor.
   *
   * For more details of the parameters please refer to the OpenCV documentation.
   *
   * @param kNumClusters Number of clusters.
   * @param kNumIterations Maximum number of iterations.
   * @param kEpsilon Early stopping if all centroid updates are smaller than this value.
   * @param kAttempt Repeat multiple times with different initializations.
   * @param kVerbose If true, print some output to the terminal.
  */
  ClusteringStrategyKmeansOpenCV
    (const size_t kNumClusters,
     const int kNumIterations,
     const T kEpsilon,
     const int kAttempts,
     const bool kVerbose);

  /**
   * Perform the actual clustering.
   *
   * Note that this involves converting out data type to a cv::Mat,
   * so there is some additional overhead.
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
  const int kAttempts_;
  const bool kVerbose_;

};

} // namespace igg

#include "clustering_strategy_kmeans_opencv.ipp"

#endif // CPP_FINAL_PROJECT_CLUSTERING_CLUSTERING_STRATEGY_KMEANS_OPENCV_HPP_
