#ifndef CPP_FINAL_PROJECT_CLUSTERING_KMEANS_WITH_INDEX_NODE_HPP_
#define CPP_FINAL_PROJECT_CLUSTERING_KMEANS_WITH_INDEX_NODE_HPP_

#include <vector>
#include <memory>


namespace igg {

template <class PointType>
class Node {
public:
  /*
   * Constructor for a leaf node.
   */
  Node(PointType const * const kElement):
    kLeft_{nullptr},
    kRight_{nullptr},
    kElement_{kElement},
    kSplitValue_{0},
    kMinValue_{0},
    kMaxValue_{0},
    kSplitDimension_{0}
  {}

  /*
   * Constructor for an inner node.
   */
  Node
    (std::unique_ptr<const Node<PointType>>&& left,
     std::unique_ptr<const Node<PointType>>&& right,
     const typename PointType::value_type kSplitValue,
     const typename PointType::value_type kMinValue,
     const typename PointType::value_type kMaxValue,
     const size_t kSplitDimension):
    kLeft_{std::move(left)},
    kRight_{std::move(right)},
    kElement_{nullptr},
    kSplitValue_{kSplitValue},
    kMinValue_{kMinValue},
    kMaxValue_{kMaxValue},
    kSplitDimension_{kSplitDimension}
  {}

  bool IsLeaf() const {return this->kElement_;}

  PointType const * Element() const {return this->kElement_;}

  /**
   * The value at which the data was split.
   */
  typename PointType::value_type SplitValue() const {return this->kSplitValue_;}

  /**
   * Minimum of all points represented by this node along the split dimension.
   */
  typename PointType::value_type MinValue() const {return this->kMinValue_;}

  /**
   * Maximum of all points represented by this node along the split dimension.
   */
  typename PointType::value_type MaxValue() const {return this->kMaxValue_;}

  /**
   * The dimension along which the data was split.
   */
  typename PointType::value_type SplitDimension() const {return this->kSplitDimension_;}

  /**
   * Left child.
   */
  Node<PointType> const * Left() const {return this->kLeft_.get();}

  /**
   * Right child.
   */
  Node<PointType> const * Right() const {return this->kRight_.get();}

private:
  const std::unique_ptr<const Node<PointType>> kLeft_;
  const std::unique_ptr<const Node<PointType>> kRight_;

  PointType const * const kElement_;

  const typename PointType::value_type kSplitValue_;
  const typename PointType::value_type kMinValue_;
  const typename PointType::value_type kMaxValue_;
  const size_t kSplitDimension_;
};

} // namespace igg

#endif // CPP_FINAL_PROJECT_CLUSTERING_KMEANS_WITH_INDEX_NODE_HPP_
