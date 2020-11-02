#ifndef CPP_FINAL_PROJECT_TOOLS_SAMPLING_HPP_
#define CPP_FINAL_PROJECT_TOOLS_SAMPLING_HPP_

/**
 * @file sampling.hpp
 *
 * The purpose of this file is to provide function templates for
 * random number sampling, especially random vectors or sets of
 * random vectors.
 *
 * Mainly used for generation of test data.
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <random>
#include <vector>

namespace igg {

/**
 * A Vector of uniformly distributed integral or floating point types within a certain range.
 */
template <class T>
std::vector<T> SampleVectorFromUniformDistribution
  (const size_t kSize, const T kMin, const T kMax, std::mt19937& engine);

/**
 * A number of vectors of uniformly distributed integral or floating point types within a certain range.
 *
 * @kNumSamples Number of samples vectors.
 * @kSize Number of dimensions.
 */
template <class T>
std::vector<std::vector<T>> SampleVectorsFromUniformDistribution
  (const size_t kNumSamples,
   const size_t kSize,
   const T kMin,
   const T kMax,
   std::mt19937& engine);

/**
 * A normally distributed random vector of floating point types.
 *
 * @param kMean Mean of the normal distribution.
 * @param kStd Std deviation of the normal distribution.
 */
template <class T>
std::vector<T> SampleVectorFromMultivariateNormalDistrubution
  (const std::vector<T> &kMean,
   const std::vector<T> &kStd,
   std::mt19937& engine);

/**
 * Sample indices in range 0 (inclusive) to kNumTotal (exclusive).
 *
 * Sampling is done without replacement.
 *
 * @param kNumSample Number of indices to sample.
 */
template <class T>
std::vector<T> SampleIndicesWithoutReplacement
  (const T kNumSample,
   const T kNumTotal,
   std::mt19937& engine);

} // namespace igg

#include "tools/sampling.ipp"

#endif // CPP_FINAL_PROJECT_TOOLS_SAMPLING_HPP_

