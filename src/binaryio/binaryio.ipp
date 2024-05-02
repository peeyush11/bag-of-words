

namespace igg {

template <class T>
bool WriteCentroidsToBinary
  (const std::string& kPath, const std::vector<FeaturePoint<T>>& kCentroids)
{
  if (kCentroids.empty()) {
    std::cerr << "Vector of centroids is empty.\n.";
    return false;
  }

  auto file = std::ofstream
    (kPath, std::ofstream::binary|std::ofstream::out|std::ofstream::trunc);
  if (!file.is_open()) {
    std::cerr << "Cannot write to file " << kPath << ".\n";
    return false;
  }

  auto num_centroids = kCentroids.size();
  auto num_features = kCentroids[0].size();

  // Write header information (number of clusters, number of features)
  file.write(reinterpret_cast<char*>(&num_centroids), sizeof(size_t));
  file.write(reinterpret_cast<char*>(&num_features), sizeof(size_t));

  // Write centroids
  for(const auto& kCentroid: kCentroids) {
    for(auto value: kCentroid) {
      file.write(reinterpret_cast<char*>(&value), sizeof(T));
    }
  }

  return true;
}


template <class T>
std::vector<FeaturePoint<T>> ReadCentroidsFromBinary(const std::string& kPath) {
  std::ifstream file = std::ifstream
    (kPath, std::ifstream::binary|std::ifstream::in);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file "+kPath+".");
  }

  // Read header information
  size_t num_centroids = 0;
  size_t num_features = 0;
  file.read(reinterpret_cast<char*>(&num_centroids), sizeof(size_t));
  file.read(reinterpret_cast<char*>(&num_features), sizeof(size_t));

  // Read centroids
  std::vector<FeaturePoint<T>> centroids;
  centroids.reserve(num_centroids);

  for (size_t centroid_index = 0; centroid_index<num_centroids; centroid_index++) {
    FeaturePoint<T> centroid;
    centroids.reserve(num_features);

    for (size_t index = 0; index<num_features; index++) {
      T value = 0;
      file.read(reinterpret_cast<char*>(&value), sizeof(T));
      centroid.emplace_back(value);
    }
    centroids.emplace_back(centroid);
  }

  return centroids;
}


template <class T>
bool WriteToBinary(const std::string& kPath, const std::vector<T>& kValues) {
  auto file = std::ofstream
    (kPath, std::ofstream::binary|std::ofstream::out|std::ofstream::trunc);
  if (!file.is_open()) {
    std::cerr << "Cannot write to file " << kPath << ".\n";
    return false;
  }

  // Write header information (number of values)
  size_t number_of_values = kValues.size();
  // Assumes number of features is the same for all centroids, no extra check
  file.write(reinterpret_cast<char*>(&number_of_values), sizeof(size_t));

  // Write clusters
  for(auto value: kValues) {
    file.write(reinterpret_cast<char*>(&value), sizeof(T));
  }

  return true;
}


template <class T>
std::vector<T> ReadFromBinary(const std::string& kPath) {
  std::ifstream file = std::ifstream
    (kPath, std::ifstream::binary|std::ifstream::in);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file "+kPath+".");
  }

  // Read header information
  size_t number_of_values = 0;
  file.read(reinterpret_cast<char*>(&number_of_values), sizeof(size_t));

  std::vector<T> values;
  values.reserve(number_of_values);

  for (size_t index = 0; index<number_of_values; index++) {
    T value = 0;
    file.read(reinterpret_cast<char*>(&value), sizeof(T));
    values.emplace_back(value);
  }

  return values;
}

} // namespace igg

