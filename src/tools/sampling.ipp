

#include <stdexcept>

namespace igg {

template <class T>
std::vector<T> SampleVectorFromUniformDistribution
  (const size_t kSize, const T kMin, const T kMax, std::mt19937& engine)
{
  // Make sure we are not using this function with wrong type
  // (this is checked at compile time)
  static_assert
    (std::is_integral<T>::value || std::is_floating_point<T>::value,
     "Integer or float type required.");

  using UniformDistribution = typename std::conditional
    <std::is_integral<T>::value,
     std::uniform_int_distribution<T>,
     std::uniform_real_distribution<T>>::type;

  std::vector<T> sample(kSize);
  UniformDistribution uniform(kMin, kMax);
  for (auto& value : sample) {value = uniform(engine);}
  return sample;
}


template <class T>
std::vector<std::vector<T>> SampleVectorsFromUniformDistribution
  (const size_t kNumSamples,
   const size_t kSize,
   const T kMin,
   const T kMax,
   std::mt19937& engine)
{
  std::vector<std::vector<T>> samples;
  samples.reserve(kNumSamples);
  for(size_t do_not_use = 0; do_not_use<kNumSamples; do_not_use++) {
    samples.emplace_back
      (SampleVectorFromUniformDistribution
        (kSize, kMin, kMax, engine));
  }
  return samples;
}


template <class T>
std::vector<T> SampleVectorFromMultivariateNormalDistrubution
  (const std::vector<T> &kMean,
   const std::vector<T> &kStd,
   std::mt19937& engine)
{
  static_assert
    (std::is_floating_point<T>::value,
     "Float type required.");

  if (kMean.size()!=kStd.size()){
    throw std::invalid_argument("Dimension mismatch.");
  }

  const size_t kSize = kMean.size();
  std::vector<T> sample;
  sample.reserve(kSize);
  using NormalDistribution = typename std::normal_distribution<T>;
  for (size_t index = 0; index<kSize; index++) {
    NormalDistribution normal(kMean[index], kStd[index]);
    sample.emplace_back(normal(engine));
  }
  return sample;
}


template <class T>
std::vector<T> SampleIndicesWithoutReplacement
  (const T kNumElementsInSample,
   const T kNumElementsTotal,
   std::mt19937& engine)
{
  if (kNumElementsInSample>kNumElementsTotal) {
    throw std::invalid_argument("Sample size larger than total size.");
  }

  std::vector<size_t> all_indices(kNumElementsTotal);
  // Fill with values 0, 1, ..., kNumElements-1
  std::iota(all_indices.begin(), all_indices.end(), 0);

  // Swap the first kNumElementsInSample indices with a randomly selected one
  // Can be replaced with std::sample in C++17
  size_t random_index = 0;
  // -1 because intervall is closed
  std::uniform_int_distribution<size_t> uniform(0, kNumElementsTotal-1);
  for (size_t sample_index = 0;
       sample_index<kNumElementsInSample;
       sample_index++)
  {
    random_index = uniform(engine);
    std::swap(all_indices[sample_index], all_indices[random_index]);
  }

  return std::vector<T>
    (all_indices.begin(),
     all_indices.begin()+kNumElementsInSample);
}

} // namespace igg
