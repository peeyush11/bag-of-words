

namespace igg
{
namespace vers_2
{

template <class T>
Cluster<T>::Cluster(const int &clusterId, const FeaturePoint<T> &centroid) : clusterId_{clusterId}
{
    centroid_ = centroid;
}

template <class T>
void Cluster<T>::AddPoint(const FeaturePoint<T> &point)
{
    pointSet_.emplace_back(point);
}

template <class T>
bool Cluster<T>::DeletePoint(const int &pointId)
{
    int size = pointSet_.size();

    for (int i = 0; i < size; i++)
    {
        if (pointSet_[i].GetPointId() == pointId)
        {
            pointSet_.erase(pointSet_.begin() + i);

            return true;
        }
    }
    return false;
}

template <class T>
FeaturePoint<T> Cluster<T>::GetCentroid()
{
    return centroid_;
}

template <class T>
int Cluster<T>::GetClusterId()
{
    return clusterId_;
}

template <class T>
FeaturePoint<T> Cluster<T>::GetPoint(const int &index)
{
    return pointSet_[index];
}

template <class T>
int Cluster<T>::ClusterPointsCount()
{
    return pointSet_.size();
}

template <class T>
T Cluster<T>::At(const int &index)
{
    return centroid_.At(index);
}

template <class T>
void Cluster<T>::SetCentroidValue(const int &index, const T &value)
{
    this->centroid_[index] = value;
}

template <class T>
int Cluster<T>::CentroidDimension()
{
    return centroid_.GetDimensions();
}

} // namespace vers_2
} // namespace igg
