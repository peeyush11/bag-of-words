

#include <cmath>
#include <algorithm>
#include <eigen3/Eigen/Dense>
#include <random>

#include "tools/terminalout.hpp"


namespace igg
{
namespace vers_2
{

template <class T>
KMeans<T>::KMeans(const size_t K, const int iterations, const size_t total_points) : K_{K}, iters_{iterations}, total_points_{total_points}
{
    clusters_.reserve(K_);
    responsibility_.setConstant(K_, total_points_, false); // Initially, no point is assigned to any cluster
}

template <class T>
int KMeans<T>::NearestCluster(const FeaturePoint<T> &point, float& min_dist)
{
    min_dist = 0.0;
    int NearestClusterId = 0;

    for (size_t i = 0; i < clusters_.size(); i++)
    {
        double dist = KNorm(clusters_[i].GetCentroid(), point);

        if (dist < min_dist || i == 0)
        {
            min_dist = dist;
            NearestClusterId = i;
        }
    }

    return NearestClusterId;
}

template <class T>
void KMeans<T>::RunKMeans(const std::vector<FeaturePoint<T>> &point_set)
{
    std::cout << "Point set size: " << point_set.size() << std::endl;
    std::cout << "K: " << K_ << std::endl;
    // Check for no. of clusters > no. of points
    if (point_set.size() < K_)
    {
        throw std::runtime_error("Error: Number of clusters greater than number of points.");
    }

    total_points_ = point_set.size();

    // Initializing clusters
    InitCluster(point_set);
    // PrintClusterCentroid();
    std::cout << "Clusters initialized: " << clusters_.size() << std::endl;

    std::cout << "Running K-Means clustering" << std::endl;

    int iter = 0;

    while (true)
    {
        std::cout << "Start iteration: " << iter << "\n";

        bool done = true; // Set to false as soon as we update some cluster assignments

        for (size_t i = 0; i < total_points_; i++)
        {
            // Find cluster with nearest centroid
            float min = 0.0;
            // Function will also requires to return minimum distance by reference, but it is not used here
            int neareast_cluster_id = NearestCluster(point_set[i], min);

            // Check integrity of resposibility matrix
            int count = responsibility_.col(i).count();
            if (count > 1)
            {
                throw std::runtime_error("Something's wrong, count should be 0 or 1. Each point should belong to only one cluster.");
            }

            if (count == 1)
            {
                // Get the current cluster id of this point
                // It is the ith column's maximum's index
                Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>::Index max_row; // Set in following line
                responsibility_.col(i).maxCoeff(&max_row);

                int current_cluster_id = max_row;
                if (neareast_cluster_id != current_cluster_id)
                {
                    responsibility_(current_cluster_id, i) = false; // Unset current cluster assigment
                    responsibility_(neareast_cluster_id, i) = true; // Set new cluster assigment
                    done = false; // Something was updated, we need to continue to iterate
                }
                else
                {
                    // No need to update cluster, current assignment equals new assigment
                    continue;
                }
            }
            else
            {
                // This point has no current assignment, it is assigned to a cluster for the first time
                responsibility_(neareast_cluster_id, i) = true;
                done = false; // Something was updated, we need to continue to iterate
            }
        }

        // Recompute centroids
        if (!done)
        {
            ReComputeCentroid(point_set);
        }

        if (done || iter >= iters_)
        {
            std::cout << "Clustering completed in iteration: " << iter << std::endl;
            break;
        }

        iter++;
    }
}

template <class T>
void KMeans<T>::InitCluster(const std::vector<FeaturePoint<T>> &point_set)
{
    // Randomly choose centroid for all clusters for first iteration using K-Means++ initialization
    // References:
    // https://stackoverflow.com/questions/5466323/how-exactly-does-k-means-work
    // https://www.fluentcpp.com/2019/05/24/how-to-fill-a-cpp-collection-with-random-values


    // For the first cluster, all points have equal probability to be drawn as the centroid
    // For this we need to sample integers from a uniform distribution
    std::random_device random_device;  // Will be used to obtain a seed for the random number engine
    std::mt19937 random_generator(random_device()); // Standard mersenne_twister_engine seeded with random_device()
    std::uniform_int_distribution<int> uniform_int_distribution(0, total_points_ - 1);

    int point_index = uniform_int_distribution(random_generator); // A random integer between 0 (inclusive) and total_points_ (exclusive)
    Cluster<float> cluster(0, point_set[point_index]);
    clusters_.emplace_back(cluster);

    // Sample remaining centroids, points have probability inverse to the minimum distance to the already existing clusters
    // For this we need to sample floats from a uniform distribution
    std::uniform_real_distribution<float> uniform_float_distribution(0.0, 1.0);

    size_t k = 1; // Cluster index, starting with 1 as first centroid has been sampled already
    while (k < K_)
    {
        // For each point, get the minimum distance to already initialized clusters
        std::vector<float> min_dist;
        float sum = 0.0f;
        float min = 0.0f;
        for (size_t i = 0; i < total_points_; i++)
        {
            NearestCluster(point_set[i], min);
            sum += min;
            min_dist.emplace_back(min);
        }

        // From this, compute a cumulative probability for each point
        std::vector<float> cumulative_probs;
        float cum_sum = 0.0f;
        for (size_t i = 0; i < min_dist.size(); i++)
        {
            min_dist[i] = min_dist[i] / sum;
            cum_sum += min_dist[i];
            cumulative_probs.push_back(cum_sum);
        }

        // Sample a float between 0 and 1
        float probability = uniform_float_distribution(random_generator);

        for (size_t i = 0; i < cumulative_probs.size(); i++)
        {
            if (probability < cumulative_probs[i])
            {
                // Accept this point as a cluster centroid
                Cluster<float> cluster(k, point_set[i]);
                clusters_.emplace_back(cluster);
                k++;
                break;
            }
        }

    }
}

template <class T>
void KMeans<T>::ReComputeCentroid(const std::vector<FeaturePoint<T>> &point_set)
{
    for (size_t i = 0; i < K_; i++)
    {

        int total_points_cluster = responsibility_.row(i).count();
        std::cout << "Total points in cluster " << i << ": " << total_points_cluster << std::endl;
        std::vector<float> vsum(point_set[0].GetDimensions(), 0.0f);
        for (int j = 0; j < responsibility_.cols(); j++)
        {

            if (responsibility_(i, j) == true)
            {
                std::vector<T> data = point_set[j].GetData();
                // typename std::vector<T>::iterator it = point_set[j].begin();
                //Reference: https://stackoverflow.com/questions/28607912/sum-values-of-2-vectors/28607985
                std::transform(vsum.begin(), vsum.end(), data.begin(), vsum.begin(), std::plus<float>());
            }
        }
        //Reference: https://stackoverflow.com/questions/41030911/how-to-multiply-a-vector-and-scalar-in-c
        std::transform(vsum.begin(), vsum.end(), vsum.begin(),
                       [total_points_cluster](T i) { return static_cast<float>(i) * 1 / total_points_cluster; });
        FeaturePoint<T> point(1);
        point.SetVector(vsum);

        clusters_[i].SetCentroid(point);
    }
}

template <class T>
float KMeans<T>::KNorm(const FeaturePoint<T> &centroid, const FeaturePoint<T> &point)
{
    float sum = 0.0f;
    if (centroid.GetDimensions() == point.GetDimensions())
    {
        for (int i = 0; i < centroid.GetDimensions(); i++)
        {
            sum += pow((centroid.At(i) - point.At(i)), 2);
        }
    }
    else
    {
        throw std::runtime_error("Point dimensions don't  match.");
    }

    return sum;
}

template <class T>
void KMeans<T>::PrintClusterCentroid()
{

    for (int i = 0; i < K_; i++)
    {
        std::cout << " Centroid of cluster: " << i << " : ";

        for (int j = 0; j < clusters_[0].CentroidDimension(); j++)
        {
            std::cout << clusters_[i].At(j) << " ";
        }
        std::cout << std::endl;
    }
}

template <class T>
void KMeans<T>::PrintClusterPointIds()
{
    for (int i = 0; i < K_; i++)
    {
        std::cout << "Points in cluster: " << i << " : ";
        for (int j = 0; j < responsibility_.cols(); j++)
        {
            if (responsibility_(i, j) == true)
            {
                std::cout << "\t " << j;
            }
        }
        std::cout << std::endl;
    }
}

template <class T>
std::vector<std::vector<T>> KMeans<T>::GetCentroids()
{
    std::vector<std::vector<T>> centroids;
    centroids.reserve(this->K_);
    for (auto &cluster : this->clusters_)
    {
        std::vector<T> centroid;
        const int FeatureDimension = cluster.CentroidDimension();
        centroid.reserve(FeatureDimension);
        for (int index = 0; index < FeatureDimension; index++)
        {
            centroid.emplace_back(static_cast<T>(cluster.At(index)));
        }
        centroids.emplace_back(centroid);
    }
    return centroids;
}

template <class T>
void KMeans<T>::PrintResponsibilityMatrix()
{
    std::cout<<"Responsibility Matrix: \n"<<responsibility_<<std::endl;
}

} // namespace vers_2
} // namespace igg
