

#include "tools/linalg.hpp"


namespace igg {

template <class T>
std::vector<T> ComputeSimilarities
  (const Histogram<T> kHistogram,
   const std::vector<Histogram<T>> kHistogramsToCompare)
{
  const auto kHistogramNorm = L2Norm<Histogram<T>>(kHistogram);

  const auto kNumSimilarities = kHistogramsToCompare.size();
  std::vector<T> similarities;
  similarities.reserve(kNumSimilarities);

  for (const auto& kHistogramToCompare: kHistogramsToCompare) {
    const auto kHistogramToCompareNorm = L2Norm<Histogram<T>>(kHistogramToCompare);
    const auto kDotProduct = DotProduct<Histogram<T>>(kHistogram, kHistogramToCompare);
    // Will throw an exception in case of dimension mismatch

    const T kSimilarity = kDotProduct/kHistogramNorm/kHistogramToCompareNorm;
    similarities.emplace_back(kSimilarity);
  }

  return similarities;
}

} // namespace igg
