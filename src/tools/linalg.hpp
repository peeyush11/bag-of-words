#ifndef CPP_FINAL_PROJECT_TOOLS_LINALG_HPP_
#define CPP_FINAL_PROJECT_TOOLS_LINALG_HPP_

/**
 * @file linalg.hpp
 *
 * The perpose of this file is to provide linear algebra function templates
 * used at different places in the project.
 *
 * The usual choice for VectorType would be std::vector<float> or std::vector<double>.
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <vector>


namespace igg {

/**
 * Get the dot product (inner product) of two vectors.
 *
 * An instance of std::invalid_argument is thrown if they do not
 * have the same number of dimensions.
 */
template <class VectorType>
auto DotProduct
  (const VectorType& kVector1,
   const VectorType& kVector2);

/**
 * Get the element-wise difference of two vectors.
 *
 * An instance of std::invalid_argument is thrown if they do not
 * have the same number of dimensions.
 */
template <class VectorType>
VectorType Difference
  (const VectorType& kMinuend,
   const VectorType& kSubtrahend);

/**
 * Get the L2 norm of a vector.
 */
template <class VectorType>
auto L2Norm(const VectorType& kVector);

/**
 * Get the squared L2 norm of a vector.
 */
template <class VectorType>
auto SquaredL2Norm (const VectorType& kVector);

/**
 * Compute the centroid (mean) of a set of vectors.
 *
 * An instance of std::invalid_argument is thrown if the set is empty.
 *
 * All points are expected to have the same number of dimensions. If this
 * is not the case, the behaviour is undefined.
 */
template <class VectorType>
VectorType Centroid (const std::vector<VectorType const *>& kVectorSet);

/**
 * Compute the variance of a set of vectors.
 *
 * An instance of std::invalid_argument is thrown if the set is empty.
 *
 * All points are expected to have the same number of dimensions. If this
 * is not the case, the behaviour is undefined.
 */
template <class VectorType>
VectorType Variance (const std::vector<VectorType const *>& kVectorSet);

} // namespace igg

#include "linalg.ipp"

#endif // CPP_FINAL_PROJECT_TOOLS_LINALG_HPP_
