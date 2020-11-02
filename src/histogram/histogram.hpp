#ifndef CPP_FINAL_PROJECT_HISTOGRAM_HISTOGRAM_HPP_
#define CPP_FINAL_PROJECT_HISTOGRAM_HISTOGRAM_HPP_

/**
 * @file histogram.hpp
 *
 * The purpose of this file is to provide function templates to deal with
 * histogram representations of images.
 *
 * We introduce Histogram<T> as an alias for an std::vector<T>, where T
 * is usually a floating point type. See feature_point.hpp.
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <vector>


namespace igg {

template <class T>
using Histogram = std::vector<T>;

/*
 * Compute the similarity of a query histogram to an ordered set of histograms.
 *
 * All histograms are expected to have the same number of bins.
 *
 * @return Similarity measures as a std::vector. The order corresponds to the
 * order in kHistogramsToComapre.
 */
template <class T>
std::vector<T> ComputeSimilarities
  (const Histogram<T> kHistogram,
   const std::vector<Histogram<T>> kHistogramsToCompare);

} // namespace igg

#include "histogram.ipp"

#endif // CPP_FINAL_PROJECT_HISTOGRAM_HISTOGRAM_HPP_
