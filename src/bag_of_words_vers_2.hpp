#ifndef CPP_FINAL_PROJECT_BAG_OF_WORDS_VERS_2_HPP_
#define CPP_FINAL_PROJECT_BAG_OF_WORDS_VERS_2_HPP_

/**
 * @file bag_of_words_vers_2.hpp
 *
 * Note this is one of two alternative implementations.
 *
 * @author Peeyush Kumar
 * @version 1.0
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>

#include "dataset/dataset.hpp"


namespace igg
{
namespace vers_2
{

class bagofwords
{
private:
    std::shared_ptr<const igg::Dataset> dataset_;
    std::vector<std::vector<std::vector<float>>> features_per_image_;
    std::vector<std::vector<float>> kFeatures_flatten_;
    std::vector<std::vector<float>> centroids_;
    std::vector<std::vector<float>> histogram_per_image_;

    const int kNumIterations_;
    const double kEpsilon_;
    const int kNumClusters_;
    const bool kVerbose_;

public:
    bagofwords();
    bagofwords(const int NumIterations, const double Epsilon, const int NumClusters, const bool Verbose);

    // Get items ordered by similarity
    std::vector<std::shared_ptr<const ImageItem>> SearchImage(const cv::Mat& QuerriedImage);
    void CreateDictionary(const int LoadFeatures, const std::string& kSelectedAlgorithm);
    void ExtractFeaturesImageDataset();
    void ComputeClusterCentroids(const std::string& KSelectedAlgorithm);
    void SaveCentroidsToFile();
    void SaveFeaturesToFile(const std::shared_ptr<const igg::ImageItem>& kItem, const cv::Mat& kFeatures);
    void LoadFeaturesFromFile();
    void SaveHistogramImageDataset();
    std::vector<float> ComputeHistogram(const std::vector<std::vector<float>>& feature_set, const int bins);
    void ReWeightHistogram(const std::vector<float>& image_count_per_cluster, int index);
    int NearestCluster(const std::vector<float>& image_feature);
    float KSqnorm(const std::vector<float>& centroid, const std::vector<float>& image_feature);
    void LoadCentroidsFromFile();
    float L2Norm(const std::vector<float>& h1, const std::vector<float>& h2);
    std::vector<float> CompareHistogram(const std::vector<float>& qhistogram);
};

} // namespace vers_2
} // namespace igg

#endif // CPP_FINAL_PROJECT_BAG_OF_WORDS_VERS_2_HPP_
