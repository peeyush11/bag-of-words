#ifndef CPP_FINAL_PROJECT_CLUSTERING_KMEANS_VERS_2_CLUSTER_HPP_
#define CPP_FINAL_PROJECT_CLUSTERING_KMEANS_VERS_2_CLUSTER_HPP_

/**
 * @file cluster.hpp
 *
 * @author Peeyush Kumar
 * @version 1.0
 */

#include <vector>

#include "feature_point.hpp"


namespace igg
{
namespace vers_2
{

template <class T>
class Cluster
{
private:
    int clusterId_;
    FeaturePoint<T> centroid_;
    std::vector<vers_2::FeaturePoint<T>> pointSet_;

public:
    Cluster() = default;
    explicit Cluster(const int &clusterId, const FeaturePoint<T> &centroid);

    ~Cluster() = default;

    void AddPoint(const FeaturePoint<T> &point);
    bool DeletePoint(const int &pointId);

    int GetClusterId();
    T At(const int &index);
    FeaturePoint<T> GetPoint(const int &index);
    int ClusterPointsCount();

    void SetCentroidValue(const int &index, const T &value);
    int CentroidDimension();
    FeaturePoint<T> GetCentroid();
    void SetCentroid(FeaturePoint<T> centroid)
    {
        centroid_ = centroid;
    }
};

} // namespace vers_2
} // namespace igg

#include "cluster.ipp"

#endif // CPP_FINAL_PROJECT_CLUSTERING_KMEANS_VERS_2_CLUSTER_HPP_
