/**
 * @file index.ipp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

namespace igg {

template <class PointType>
Index<PointType>::Index
  (const std::vector<PointType const *>& kPointSet,
   const size_t kNumTrees,
   const size_t kSearchLimit,
   const size_t kNumSplitDimensions,
   const int kSeed):
  kSearchLimit_{kSearchLimit}
{
  // Seed random number generator
  // Will be used to draw seeds for random trees
  std::mt19937 engine(kSeed);
  std::uniform_int_distribution<> distribution(0); // Upper limit is numerical max of int

  // Initialize trees
  this->trees_.reserve(kNumTrees);
  for (size_t tree_index = 0; tree_index<kNumTrees; tree_index++) {
    //std::cout << "Constructing tree " << tree_index << "\n";
    const auto kTreeSeed = distribution(engine);
    this->trees_.emplace_back(RandomTree<PointType>
      (kPointSet, kNumSplitDimensions, kTreeSeed));
  }
}


template <class PointType>
PointType const * Index<PointType>::ApproximateNearestNeighbor
  (const PointType& kQueryPoint)
{
  // We need the priority queue to store pairs of Nodes to traverse with their asscociated
  // priority, which equals the inverse minimal distance of the query point to the current split
  // (we directly use squared distance and sort queue in acending order)
  std::priority_queue
    <QueueElement<PointType>,
     std::vector<QueueElement<PointType>>,
     CompareInQueue<QueueElement<PointType>>>
    queue;

  // For each tree, add the root node to the queue
  for (const auto& kTree: this->trees_) {
    queue.emplace(std::make_pair(kTree.Root(), 0.0));
  }

  PointType const * best_search_result = nullptr;
  typename PointType::value_type best_squared_distance = std::numeric_limits<typename PointType::value_type>::max();

  // Perform kSearchLimit_ nearest neighbour searches, each by taking the top
  // element from the queue and adding more elements while traversing the tree
  for (size_t search_count = 0; search_count<this->kSearchLimit_; search_count++) {
    if (queue.empty())
      {break;}

    const auto kSearchResult = this->TraverseTree
      (kQueryPoint, queue.top().first, queue.top().second, &queue);
    queue.pop();

    const auto kSquaredDistance = SquaredL2Norm(Difference(kQueryPoint, *kSearchResult));

    //std::cout << "search count " << search_count << "\n";
    //std::cout << "distance " << kSquaredDistance << "\n";

    if (kSquaredDistance<best_squared_distance) {
      best_squared_distance = kSquaredDistance;
      best_search_result = kSearchResult;
    }
  }

  return best_search_result;
}


template <class PointType>
PointType const * Index<PointType>::TraverseTree
  (const PointType kQueryPoint,
   Node<PointType> const * node,
   typename PointType::value_type squared_distance,
   std::priority_queue
     <QueueElement<PointType>,
      std::vector<QueueElement<PointType>>,
      CompareInQueue<QueueElement<PointType>>> * queue)
{
  if (node->IsLeaf()) {
    return node->Element();
  }

  const auto kValue = kQueryPoint[node->SplitDimension()];

  Node<PointType> const * node_to_continue;
  Node<PointType> const * node_to_add_to_queue;
  typename PointType::value_type min_value;
  typename PointType::value_type max_value;
  // All will be defined inside if-else-block

  if (kValue>=node->SplitValue()) {
    // Continue right
    node_to_continue = node->Right();
    node_to_add_to_queue = node->Left();
    min_value = node->MinValue();
    max_value = node->SplitValue();
  } else {
    // Continue left
    node_to_continue = node->Left();
    node_to_add_to_queue = node->Right();
    min_value = node->SplitValue();
    max_value = node->MaxValue();
  }

  const auto kUpdatedSquaredDistance =
    squared_distance
    -this->SquaredDistanceFromSplit(kValue, node->MinValue(), node->MaxValue())
    +this->SquaredDistanceFromSplit(kValue, min_value, max_value);

  queue->emplace(std::make_pair(node_to_add_to_queue, kUpdatedSquaredDistance));

  return TraverseTree(kQueryPoint, node_to_continue, squared_distance, queue);
}


template <class PointType>
typename PointType::value_type Index<PointType>::SquaredDistanceFromSplit
  (const typename PointType::value_type kValue,
   const typename PointType::value_type kMinValue,
   const typename PointType::value_type kMaxValue)
{
  if (kValue>=kMinValue && kValue<=kMaxValue) {
    return 0.0;
  }

  const auto kDifferenceMin = kValue-kMinValue;
  const auto kDifferenceMax = kValue-kMinValue;

  return std::min(kDifferenceMin*kDifferenceMin, kDifferenceMax*kDifferenceMax);
}

};
