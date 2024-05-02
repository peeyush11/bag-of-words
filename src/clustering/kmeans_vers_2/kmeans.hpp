#ifndef CPP_FINAL_PROJECT_CLUSTERING_KMEANS_VERS_2_KMEANS_HPP_
#define CPP_FINAL_PROJECT_CLUSTERING_KMEANS_VERS_2_KMEANS_HPP_


#include "cluster.hpp"
#include <eigen3/Eigen/Dense>

#include "feature_point.hpp"


namespace igg
{
namespace vers_2
{

template <class T>
class KMeans
{
private:
    size_t K_;
    int iters_;
    size_t total_points_;

    std::vector<Cluster<T>> clusters_;

    // Matrix of bools to store and lookup cluster assignments
    // One row for each cluster, one column for each data point
    // [i, j]==true <=> point j is assigned to cluster i
    // Note that it is in principle possible to assign a point to multiple clusters,
    // therefore we need to maintain this matrix properly
    Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> responsibility_; // Use Eigen::Dynamic as size is not known at compile time

public:
    KMeans() = default;
    explicit KMeans(const size_t K, const int iterations, const size_t total_points);
    ~KMeans() = default;

    void RunKMeans(const std::vector<FeaturePoint<T>> &point_set);
    int NearestCluster(const FeaturePoint<T> &point, float &min_dist);
    float KNorm(const FeaturePoint<T> &centroid, const FeaturePoint<T> &point);
    void InitCluster(const std::vector<FeaturePoint<T>> &point_set);
    void ReComputeCentroid(const std::vector<FeaturePoint<T>> &point_set);
    void PrintClusterPointIds();
    void PrintClusterCentroid();
    std::vector<std::vector<T>> GetCentroids();
    void PrintResponsibilityMatrix();
};

} // namespave vers_2
} // namespace igg

#include "kmeans.ipp"

#endif // CPP_FINAL_PROJECT_CLUSTERING_KMEANS_VERS_2_KMEANS_HPP_
