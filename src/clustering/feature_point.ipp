/**
 * @file feature_point.ipp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <numeric>

#include "tools/linalg.hpp"


namespace igg {

template <class T>
size_t NearestNeighbor
  (const FeaturePoint<T>& kQueryPoint,
   const std::vector<FeaturePoint<T>>& kPointSet)
{
  if (kPointSet.empty())
    {throw std::invalid_argument("Empty set of points.");}

  const auto kNumPoints = kPointSet.size();

  T min_distance = std::numeric_limits<T>::max();
  size_t nearest_cluster_index = 0;

  FeaturePoint<T> difference; // Initialized inside loop
  T squared_distance; // Initialized inside loop

  for (size_t point_index = 0; point_index<kNumPoints; point_index++) {
    difference = Difference<FeaturePoint<T>>(kQueryPoint, kPointSet[point_index]);
    squared_distance = SquaredL2Norm<FeaturePoint<T>>(difference);

    if (squared_distance<=min_distance) {
      min_distance = squared_distance;
      nearest_cluster_index = point_index;
    }
  }

  return nearest_cluster_index;
}


template <class T>
std::vector<T> Flatten
  (std::vector<std::vector<T>>&& vector_of_vectors)
{
  // Use lambda expression to get total number of elements
  auto add_size = [](size_t num_elements, std::vector<T> vector_in_vector) -> size_t
    {return std::move(num_elements)+vector_in_vector.size();};

  const auto kNumElements = std::accumulate
      (vector_of_vectors.begin(), vector_of_vectors.end(), 0, add_size);

  // Reference: https://stackoverflow.com/questions/201718/concatenating-two-stdvectors
  std::vector<T> flattened_vector;
  flattened_vector.reserve(kNumElements);

  for(auto vector_in_vector: vector_of_vectors) {
    flattened_vector.insert
      (flattened_vector.end(),
       std::make_move_iterator(vector_in_vector.begin()),
       std::make_move_iterator(vector_in_vector.end()));
  }

  return flattened_vector;
}


template <class T>
std::vector<FeaturePoint<T>> FromMat(cv::Mat&& mat) {
  std::vector<FeaturePoint<T>> point_set;
  point_set.reserve(mat.rows);

  for(int index = 0; index<mat.rows; index++) {
    cv::Mat row_mat = mat.row(index);
    FeaturePoint<T> point(row_mat.begin<T>(), row_mat.end<T>());
    point_set.emplace_back(point);
  }

  return point_set;
}


template <class T>
cv::Mat ToMat(std::vector<FeaturePoint<T>>&& point_set) {
  if (point_set.empty())
    {throw std::invalid_argument("Empty set of points.");}

  const auto kNumRows = point_set.size();
  const auto kFlattened = Flatten(std::move(point_set));
  // Arguments of cv::Mat::reshape: channels, rows
  return cv::Mat(kFlattened, true).reshape(0, static_cast<int>(kNumRows));
}


template <class T>
cv::Mat ToMat(const std::vector<FeaturePoint<T>>& kPointSet) {
  return ToMat(std::vector<FeaturePoint<T>>(kPointSet));
}

template <class T>
std::vector<FeaturePoint<T> const *> GetPointers
  (const std::vector<FeaturePoint<T>>& kPointSet)
{
  std::vector<FeaturePoint<T> const *> pointers(kPointSet.size());

  // Use lambda expression to get address of each element
  std::transform
    (kPointSet.begin(), kPointSet.end(), pointers.begin(),
     [](const auto& kPoint){return &kPoint;});

  return pointers;
}

} // namespace igg

