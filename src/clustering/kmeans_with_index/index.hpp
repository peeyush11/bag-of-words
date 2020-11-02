#ifndef CPP_FINAL_PROJECT_CLUSTERING_KMEANS_WITH_INDEX_INDEX_HPP_
#define CPP_FINAL_PROJECT_CLUSTERING_KMEANS_WITH_INDEX_INDEX_HPP_

/**
 * @file index.hpp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <queue>

#include "random_tree.hpp"


namespace igg {

template <class QueueElementType>
struct CompareInQueue {
  bool operator()
    (QueueElementType& element1,
     QueueElementType& element2)
  {
    return element1.second>element2.second;
  }
};

template <class PointType>
using QueueElement = std::pair<Node<PointType> const *, typename PointType::value_type>;

/**
 * Spatial index for approximate nearest neighbor search based on multiple random trees
 */
template <class PointType>
class Index {
public:
  Index
    (const std::vector<PointType const *>& kPointSet,
     const size_t kNumTrees,
     const size_t kSearchLimit,
     const size_t kNumSplitDimensions,
     const int kSeed);

  PointType const * ApproximateNearestNeighbor
    (const PointType& kQueryPoint);

private:
  const size_t kSearchLimit_;
  std::vector<RandomTree<PointType>> trees_;

  PointType const * TraverseTree
  (const PointType kQueryPoint,
   Node<PointType> const * node,
   typename PointType::value_type squared_distance,
   std::priority_queue
     <QueueElement<PointType>,
      std::vector<QueueElement<PointType>>,
      CompareInQueue<QueueElement<PointType>>> * queue);

  static typename PointType::value_type SquaredDistanceFromSplit
    (const typename PointType::value_type kValue,
     const typename PointType::value_type kMinValue,
     const typename PointType::value_type kMaxValue);

};

} // namespace igg

#include "index.ipp"

#endif // CPP_FINAL_PROJECT_CLUSTERING_KMEANS_WITH_INDEX_INDEX_HPP_
