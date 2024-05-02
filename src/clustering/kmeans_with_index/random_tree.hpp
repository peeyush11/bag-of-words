#ifndef CPP_FINAL_PROJECT_CLUSTERING_KMEANS_WITH_INDEX_RANDOM_TREE_HPP_
#define CPP_FINAL_PROJECT_CLUSTERING_KMEANS_WITH_INDEX_RANDOM_TREE_HPP_


#include <vector>
#include <memory>
#include <random>

#include "node.hpp"


namespace igg {

template <class PointType>
class RandomTree {
public:
  RandomTree
    (const std::vector<PointType const *>& kPointSet,
     const size_t kNumSplitDimensions,
     const int kSeed);

  Node<PointType> const * Root() const {return this->root_.get();}

private:
  const size_t kNumSplitDimensions_;
  std::mt19937 engine_;
  std::unique_ptr<const Node<PointType>> root_;

  std::unique_ptr<const Node<PointType>> BuildTree
    (const std::vector<PointType const *>& kPointSet);

  size_t SampleSplitDimension
    (const std::vector<PointType const *>& kPointSet);


};

} // namespace igg

#include "random_tree.ipp"

#endif // CPP_FINAL_PROJECT_CLUSTERING_KMEANS_WITH_INDEX_RANDOM_TREE_HPP_
