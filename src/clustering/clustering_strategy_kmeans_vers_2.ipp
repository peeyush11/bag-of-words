/**
 * @file clustering_strategy_kmeans_vers_2.ipp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <cmath>
#include <algorithm>

//#include "tools/sampling.hpp"
//#include "tools/terminalout.hpp"

#include "kmeans_vers_2/kmeans.hpp"


namespace igg {

template <class T>
ClusteringStrategyKmeansVers2<T>::ClusteringStrategyKmeansVers2
  (const size_t kNumClusters,
   const int kNumIterations,
   const bool kVerbose):
  kNumClusters_{kNumClusters},
  kNumIterations_{kNumIterations},
  kVerbose_{kVerbose}
{}


template <class T>
std::vector<FeaturePoint<T>> ClusteringStrategyKmeansVers2<T>::ClusterCentroids
  (const std::vector<FeaturePoint<T>>& kPointSet) const
{
  if (kPointSet.empty())
    {throw std::invalid_argument("Empty set of points.");}

  const auto kNumPoints = kPointSet.size();

  if (kNumPoints<this->kNumClusters_) {
    throw std::invalid_argument
      ("Number of clusters is larger than number of points.");
  }

  vers_2::KMeans<T> kmeans(this->kNumClusters_, this->kNumIterations_, kNumPoints);

  // Convert points to required point type
  std::vector<vers_2::FeaturePoint<T>> converted_point_set;
  converted_point_set.reserve(kNumPoints);

  for (const auto& kPoint: kPointSet) {
    vers_2::FeaturePoint<T> converted_point;
    converted_point.SetVector(kPoint);
    converted_point_set.emplace_back(converted_point);
  }

  // Run clustering
  if (!this->kVerbose_) {
    // Disable terminal output
    // Reference: https://stackoverflow.com/questions/30184998/
    // how-to-disable-cout-output-in-the-runtime
    std::cout.setstate(std::ios_base::failbit);
  }

  kmeans.RunKMeans(converted_point_set);

  if (!this->kVerbose_) {
    // Enable output again
    std::cout.clear();
  }

  return kmeans.GetCentroids();
}


} // namespace igg

