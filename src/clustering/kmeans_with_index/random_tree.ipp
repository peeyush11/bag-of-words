/**
 * @file random_tree.ipp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include "tools/linalg.hpp"

namespace igg {

template <class PointType>
RandomTree<PointType>::RandomTree
  (const std::vector<PointType const *>& kPointSet,
   const size_t kNumSplitDimensions,
   const int kSeed):
  kNumSplitDimensions_{kNumSplitDimensions},
  engine_(std::mt19937(kSeed))
{
  this->root_ = this->BuildTree(kPointSet);
}


template <class PointType>
std::unique_ptr<const Node<PointType>> RandomTree<PointType>::BuildTree
  (const std::vector<PointType const *>& kPointSet)
{
  if (kPointSet.empty()) {
    throw std::runtime_error("Tried to build tree from empty set.");
  }

  if ((*kPointSet[0]).size()<this->kNumSplitDimensions_) {
    throw std::runtime_error("Points have less dimensions than number of split dimensions considered.");
  }

  if (kPointSet.size()==1) {
    // Return a leaf
    return std::make_unique<Node<PointType>>(kPointSet[0]);
  }

  // Split into two subsets and build a subtree for each half

  // Pick split dimension
  const auto kSplitDimension = this->SampleSplitDimension(kPointSet);

  // Sort points along selected split dimension
  // TODO: This is still suboptimal as it is not necessary to sort the complete array

  const auto kNumPoints = kPointSet.size();

  // Perform argsort (get the indices that would sort the variance vector)
  // Reference: https://stackoverflow.com/questions/1577475/
  // c-sorting-and-keeping-track-of-indexes
  std::vector<size_t> indices(kNumPoints);
  // Populate with indices in increasing order
  std::iota(indices.begin(), indices.end(), 0);
  // Use lambda expression for sorting
  std::sort
    (indices.begin(), indices.end(),
     [&kPointSet, &kSplitDimension](const size_t kIndex1, const size_t kIndex2)
       {return (*kPointSet[kIndex1])[kSplitDimension]<(*kPointSet[kIndex2])[kSplitDimension];});

  const size_t kSizeLeft = kNumPoints/2;
  const size_t kSizeRight = kNumPoints-kSizeLeft;

  std::vector<PointType const *> left_subset;
  left_subset.reserve(kSizeLeft);

  for (size_t index = 0; index<kSizeLeft; index++) {
    left_subset.emplace_back(kPointSet[indices[index]]);
  }

  std::vector<PointType const *> right_subset;
  right_subset.reserve(kSizeRight);

  for (size_t index = 0; index<kSizeRight; index++) {
    right_subset.emplace_back(kPointSet[indices[kSizeLeft+index]]);
  }

  // Return smallest value of the right split as split value
  const auto kSplitValue = (*kPointSet[indices[kSizeLeft]])[kSplitDimension];
  const auto kMinValue = (*kPointSet[*indices.begin()])[kSplitDimension];
  const auto kMaxValue = (*kPointSet[*(indices.end()-1)])[kSplitDimension];

  //std::cout << "size left " << kSizeLeft << "\n";
  //std::cout << "size left " << left_subset.size() << "\n";
  //std::cout << "size right " << kSizeRight << "\n";
  //std::cout << "size right " << right_subset.size() << "\n";
  //std::cout << "split value " << kSplitValue << "\n";
  //std::cout << "min value " << kMinValue << "\n";
  //std::cout << "max value " << kMaxValue << "\n";
  //std::cout << "split dimension " << kSplitDimension << "\n";

  auto left_node = this->BuildTree(left_subset);
  auto right_node = this->BuildTree(right_subset);

  return std::make_unique<const Node<PointType>>
    (std::move(left_node), std::move(right_node), kSplitValue, kMinValue, kMaxValue, kSplitDimension);
}


template <class PointType>
size_t RandomTree<PointType>::SampleSplitDimension
  (const std::vector<PointType const *>& kPointSet)
{
  // Compute the variance of the current subset
  const auto kVariance = Variance<PointType>(kPointSet);
  const auto kNumDimensions = kVariance.size();

  // Decide along which dimension to split
  // (one randomly selected dimension of the kNumSplitDimensions_ with the highest variance)
  std::uniform_int_distribution<> distribution(0, this->kNumSplitDimensions_-1);
  const auto kDimensionIndex = distribution(this->engine_);

  // Perform argsort (get the indices that would sort the variance vector)
  // Reference: https://stackoverflow.com/questions/1577475/
  // c-sorting-and-keeping-track-of-indexes
  std::vector<size_t> indices(kNumDimensions);
  // Populate with indices in increasing order
  std::iota(indices.begin(), indices.end(), 0);
  // Use lambda expression for sorting
  std::sort
    (indices.begin(), indices.end(),
     [&kVariance](const size_t kIndex1, const size_t kIndex2)
       {return kVariance[kIndex1]>kVariance[kIndex2];});

  // TODO: This is still suboptimal as it is not necessary to sort the whole vector

  return indices[kDimensionIndex];
}

} // namespace igg
