/**
 * @file linalg.ipp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <cmath>
#include <numeric>
#include <algorithm>


namespace igg {

template <class VectorType>
auto DotProduct
  (const VectorType& kVector1,
   const VectorType& kVector2)
{
  if (kVector1.size()!=kVector2.size())
    {throw std::invalid_argument("Dimension mismatch.");}

  using ScalarType = typename VectorType::value_type;

  return std::inner_product
    (kVector1.begin(), kVector1.end(), kVector2.begin(),
     static_cast<ScalarType>(0));
}


template <class VectorType>
VectorType Difference
  (const VectorType& kMinuend,
   const VectorType& kSubtrahend)
{
  const auto kSize = kMinuend.size();
  if (kSize!=kSubtrahend.size())
    {throw std::invalid_argument("Dimension mismatch.");}

  using ScalarType = typename VectorType::value_type;

  VectorType difference;
  difference.reserve(kSize);

  // Reference for usage of std::transform:
  // https://stackoverflow.com/questions/18478385/
  // convert-two-vectors-of-int-with-the-same-length-into-one-vector-of-pairs-of-int
  std::transform
    (kMinuend.begin(), kMinuend.end(), kSubtrahend.begin(),
     std::back_inserter(difference),
     [](ScalarType minuend_value, ScalarType  subtrahend_value) -> ScalarType {return minuend_value-subtrahend_value;});

  return difference;
}


template <class VectorType>
auto L2Norm(const VectorType& kVector) {
  return std::sqrt(SquaredL2Norm<VectorType>(kVector));
}


template <class VectorType>
auto SquaredL2Norm (const VectorType& kVector) {
  using ScalarType = typename VectorType::value_type;

  // Use a lamba expression to sum up squared components
  auto add_squared = [](ScalarType sum, ScalarType value) -> ScalarType
    {return std::move(sum)+value*value;};

  return std::accumulate
    (kVector.begin(), kVector.end(), static_cast<ScalarType>(0), add_squared);
}


template <class VectorType>
VectorType Centroid (const std::vector<VectorType const *>& kVectorSet) {
  if (kVectorSet.empty())
    {throw std::invalid_argument("Try to computy centroid of empty set.");}

  using ScalarType = typename VectorType::value_type;

  const auto kNumDims = kVectorSet[0]->size();
  const auto kSize = kVectorSet.size();

  VectorType centroid;
  centroid.reserve(kNumDims);

  for
    (size_t dimension_index = 0;
     dimension_index<kNumDims;
     dimension_index++)
  {
    // Use a lamba expression to compute element-wise sum
    auto add = [&](long double sum, VectorType const * vector) -> ScalarType
      {return std::move(sum)+static_cast<long double>((*vector)[dimension_index]);};

    auto sum = std::accumulate
      (kVectorSet.begin(), kVectorSet.end(), static_cast<ScalarType>(0), add);

    centroid.emplace_back(static_cast<ScalarType>(sum/kSize));
  }

  return centroid;
}


template <class VectorType>
VectorType Variance (const std::vector<VectorType const *>& kVectorSet) {
  if (kVectorSet.empty())
    {throw std::invalid_argument("Try to computy centroid of empty set.");}

  using ScalarType = typename VectorType::value_type;

  const auto kNumDims = kVectorSet[0]->size();
  const auto kSize = kVectorSet.size();

  const auto kCentroid = Centroid(kVectorSet);
  VectorType variance;
  variance.reserve(kNumDims);

  for
    (size_t dimension_index = 0;
     dimension_index<kNumDims;
     dimension_index++)
  {
    const auto kCentroidValue = kCentroid[dimension_index];

    // Use a lamba expression to add up squared differences
    auto add = [&](long double sum, VectorType const * vector) -> ScalarType
      {const auto kDifference = (*vector)[dimension_index]-kCentroidValue;
       return std::move(sum)+static_cast<long double>(kDifference*kDifference);};

    auto sum = std::accumulate
      (kVectorSet.begin(), kVectorSet.end(), static_cast<ScalarType>(0), add);

    variance.emplace_back(static_cast<ScalarType>(sum/kSize));
  }

  return variance;
}


} // namespace igg
