

namespace igg {

template <class T>
ClusteringStrategyKmeansOpenCV<T>::ClusteringStrategyKmeansOpenCV
  (const size_t kNumClusters,
   const int kNumIterations,
   const T kEpsilon,
   const int kAttempts,
   const bool kVerbose):
  kNumClusters_{kNumClusters},
  kNumIterations_{kNumIterations},
  kEpsilon_{kEpsilon},
  kAttempts_{kAttempts},
  kVerbose_{kVerbose}
{}


template <class T>
std::vector<FeaturePoint<T>> ClusteringStrategyKmeansOpenCV<T>::ClusterCentroids
  (const std::vector<FeaturePoint<T>>& kPointSet) const
{
  if (kPointSet.empty())
    {throw std::invalid_argument("Empty set of points.");}

  const auto kNumPoints = kPointSet.size();

  if (kNumPoints<this->kNumClusters_) {
    throw std::invalid_argument
      ("Number of clusters is larger than number of points.");
  }

  // Convert points to cv::Mat
  const auto kPointSetMat = ToMat<float>(kPointSet);

  // To store results
  cv::Mat centroids_mat;
  cv::Mat labels_mat; // OpenCV wants to return labels as well

  cv::kmeans
    (kPointSetMat, this->kNumClusters_, labels_mat,
     cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS,
       this->kNumIterations_, static_cast<double>(this->kEpsilon_)),
     this->kAttempts_, cv::KMEANS_PP_CENTERS, centroids_mat);

  return FromMat<T>(std::move(centroids_mat));
}

} // namespace igg
