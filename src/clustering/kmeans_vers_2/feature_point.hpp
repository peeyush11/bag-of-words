#ifndef CPP_FINAL_PROJECT_CLUSTERING_KMEANS_VERS_2_FEATURE_POINT_HPP_
#define CPP_FINAL_PROJECT_CLUSTERING_KMEANS_VERS_2_FEATURE_POINT_HPP_

/**
 * @file feature_point.hpp
 *
 * @author Peeyush Kumar
 * @version 1.0
 */

#include <iostream>
#include <vector>


namespace igg
{
namespace vers_2
{

// Point class to store N-dimension point.
template <class T>
class FeaturePoint
{
private:
    int pointId_;
    std::vector<T> data_;

public:
    FeaturePoint() = default;
    // Keyword explicit forbids implicit conversion (e.g. FeaturePoint point = 1 is not allowed)
    explicit FeaturePoint(const int &id) : pointId_{id} {}
    ~FeaturePoint() = default;

    int GetDimensions()
    {
        return data_.size();
    }

    int GetDimensions() const
    {
        return data_.size();
    }

    int GetPointId()
    {
        return pointId_;
    }

    T At(const int &index)
    {
        return data_[index];
    }

    T At(const int &index) const
    {
        return data_[index];
    }

    void AppendValue(const T &value)
    {
        data_.emplace_back(value);
    }

    void SetVector(const std::vector<T>& data)
    {
        data_ = data;
    }

    std::vector<T> GetData() const
    {
        return data_;
    }

    // auto begin()
    // {
    //     return data_.begin();
    // }

    // auto end()
    // {
    //     return data_.end();
    // }
};

} // namespace vers_2
} // namespace igg

#endif // CPP_FINAL_PROJECT_CLUSTERING_KMEANS_VERS_2_FEATURE_POINT_HPP_
