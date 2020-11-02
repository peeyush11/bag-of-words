/**
 * @file compute_cluster_centroids.cpp
 *
 * Provides a command line interface for the clustering step.
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <boost/program_options.hpp>

#include "bag_of_words.hpp"
#include "clustering/clustering_strategy_kmeans.hpp"
#include "clustering/clustering_strategy_kmeans_vers_2.hpp"
#include "clustering/clustering_strategy_kmeans_opencv.hpp"
#include "clustering/clustering_strategy_kmeans_with_index.hpp"


int main (int argc, char** argv) {
  // Parse terminal input using boost functionality
  namespace po = boost::program_options;

  po::options_description options_description("Options");
  options_description.add_options()
    ("help,h", "Show help.")
    ("variant,v", po::value<std::string>()->default_value("kmeans"), "Variant of K-means to use. Options: kmeans, kmeans_vers_2, kmeans_opencv, kmeans_with_index.")
    ("num-clusters,k", po::value<size_t>()->default_value(100), "Number of clusters.")
    ("iterations,i", po::value<int>()->default_value(25), "Maximum number of iterations.")
    ("epsilon,e", po::value<float>()->default_value(1e-3f), "Stop if centroid updates are smaller than this value. Not supported by all variants.")
    ("seed,s", po::value<int>()->default_value(0), "Seed for initialization of centroids. Not supported by all variants.");
  // Note on the syntax: (...) is an operator on the object returned by add_options(), which returns a reference to the very same object
  // Reference: https://stackoverflow.com/questions/10486588/boost-program-options-add-options-syntax

  po::variables_map variables_map;
  try {
    po::store(po::parse_command_line(argc, argv, options_description), variables_map);
  } catch(po::error& error) {
    std::cerr << "Command not recognized.\n";
    std::cerr << error.what() << "\n.";
    return 1;
  }

  // Show help
  if (variables_map.count("help")) {
    std::cout << "Performs clustering of extracted features.\n";
    std::cout << "Please make sure the CPP_FINAL_PROJECT_DATA_DIR environment variable is set "
      "and features have be extracted.\n";
    std::cout << options_description;
    return 0;
  }

  // Read parameters
  const auto kVariant = variables_map["variant"].as<std::string>();
  const auto kNumClusters = variables_map["num-clusters"].as<size_t>();
  const auto kIterations = variables_map["iterations"].as<int>();
  if (!(kIterations>0)) {
    std::cerr << "Number of iterations is expected to be positive.\n";
    return 1;
  }
  const auto kEpsilon = variables_map["epsilon"].as<float>();
  if (kEpsilon<0.0f) {
    std::cerr << "Epsilon is expected to be non-negative.\n";
    return 1;
  }
  const auto kSeed = variables_map["seed"].as<int>();

  std::cout << "Clustering parameters:\n";
  std::cout << "* K-means variant: " << kVariant << "\n";
  std::cout << "* Number of clusters: " << kNumClusters << "\n";
  std::cout << "* Iterations: " << kIterations << "\n";
  std::cout << "* Epsilon: " << kEpsilon << "\n";
  std::cout << "* Seed: " << kSeed << "\n";

  const auto kDataset = igg::Dataset::Default();
  if (!kDataset) {std::cerr << "Error while loading dataset.\n"; return 1;}

  const igg::BagOfWords kBagOfWords(kDataset, true); // True to allow terminal output

  try {
    if (kVariant=="kmeans") {
      std::cout << "Using own implementation of K-Means.\n";
      const igg::ClusteringStrategyKmeans<float> kStrategy
        (kNumClusters, kIterations, kEpsilon, kSeed, true); // True to allow terminal output
      kBagOfWords.ComputeClusterCentroids(kStrategy);
    } else if (kVariant=="kmeans_vers_2") {
      std::cout << "Using own implementation of K-Means (second alternative).\n";
      const igg::ClusteringStrategyKmeansVers2<float> kStrategy
        (kNumClusters, kIterations, true); // True to allow terminal output
      kBagOfWords.ComputeClusterCentroids(kStrategy);
    } else if (kVariant=="kmeans_opencv") {
      std::cout << "Using OpenCV implementation of K-means.\n";
      const int kAttempts = 1;
      const igg::ClusteringStrategyKmeansOpenCV<float> kStrategy
        (kNumClusters, kIterations, kEpsilon, kAttempts, true); // True to allow terminal output
      kBagOfWords.ComputeClusterCentroids(kStrategy);
    } else if (kVariant=="kmeans_with_index") {
      const size_t kNumTrees = 10;
      const size_t kSearchLimit = 100;
      const size_t kNumSplitDimensions = 5;
      const igg::ClusteringStrategyKmeansWithIndex<float> kStrategy
        (kNumClusters, kIterations, kEpsilon, kNumTrees, kSearchLimit, kNumSplitDimensions, kSeed, true);
      kBagOfWords.ComputeClusterCentroids(kStrategy);
    } else {
      std::cerr << "Variant " << kVariant << " not recognized.\n";
      return -1;
    }
  } catch (const std::exception& kError) {
    std::cerr << "An error occured: " << kError.what() << "\n";
    return 1;
  }

  return 0;
}

