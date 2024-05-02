
#include "bag_of_words_vers_2.hpp"

#include "features/features.hpp"
#include "histogram/histogram.hpp"
#include "clustering/clustering_strategy_kmeans.hpp"
#include "clustering/clustering_strategy_kmeans_vers_2.hpp"
#include "clustering/clustering_strategy_kmeans_opencv.hpp"


namespace igg
{
namespace vers_2
{

bagofwords::bagofwords():
    kNumIterations_{100},
    kEpsilon_{1e-3},
    kNumClusters_{100},
    kVerbose_{true}
{
    dataset_ = Dataset::Default();
    features_per_image_.reserve(dataset_->Items().size());
    centroids_.reserve(kNumClusters_);
    histogram_per_image_.reserve(kNumClusters_);
}

bagofwords::bagofwords(const int NumIterations, const double Epsilon, const int NumClusters, const bool Verbose):
    kNumIterations_{NumIterations},
    kEpsilon_{Epsilon},
    kNumClusters_{NumClusters},
    kVerbose_{Verbose}
{
    dataset_ = Dataset::Default();
    features_per_image_.reserve(dataset_->Items().size());
    centroids_.reserve(kNumClusters_);
    histogram_per_image_.reserve(kNumClusters_);
}

void bagofwords::CreateDictionary(const int LoadFeatures, const std::string& kSelectedAlgorithm)
{
    if (LoadFeatures == 0)
    {
        ExtractFeaturesImageDataset();
    }
    else
    {
        LoadFeaturesFromFile();
    }

    ComputeClusterCentroids(kSelectedAlgorithm);
    SaveCentroidsToFile();
    SaveHistogramImageDataset();
}

std::vector<std::shared_ptr<const ImageItem>> bagofwords::SearchImage(const cv::Mat& QuerriedImage)
{
    if (centroids_.empty())
    {
        if (!dataset_->HasCentroids())
        {
            throw std::runtime_error("Cluster centroids not found. Run CreateDictiionary() first.");
        }
        else
        {
            LoadCentroidsFromFile();
        }
    }

    if (histogram_per_image_.empty())
    {
        for (const std::shared_ptr<const igg::ImageItem> &kItem : dataset_->Items())
        {
            const auto kHistogramPath = kItem->HistogramBinaryPath();
            if (kItem->HasHistogram())
            {
                std::cout << "  * Load histogram binary file " << kItem->HistogramBinaryFilename() << ".\n";
                std::vector<float> histogram = igg::ReadFromBinary<float>(kHistogramPath);
                histogram_per_image_.emplace_back(histogram);
            }
            else
            {
                throw std::runtime_error("Histogram not found. Run CreateDictiionary() first.");
            }
        }
    }

    //Computer Features in Querried Image
    cv::Mat features = igg::ComputeFeatures(QuerriedImage);
    std::vector<std::vector<float>> qFeatures = igg::FromMat<float>(std::move(features));
    //Compute histogram of the querried Image from the dictionary
    std::vector<float> qhistogram(kNumClusters_);
    qhistogram = ComputeHistogram(qFeatures, kNumClusters_);
    const std::vector<float> kSimilarities = CompareHistogram(qhistogram);

    // Perform argsort (get the indices that would sort the similarities vector)
    // Reference: https://stackoverflow.com/questions/1577475/
    // c-sorting-and-keeping-track-of-indexes
    const size_t kNumImages = dataset_->Items().size();
    std::vector<size_t> indices(kNumImages);
    // Populate with indices in increasing order
    std::iota(indices.begin(), indices.end(), 0);
    // Use lambda expression for sorting
    std::sort(indices.begin(), indices.end(),
              [&kSimilarities](const size_t kIndex1, const size_t kIndex2)
              {return kSimilarities[kIndex1]>kSimilarities[kIndex2];});

    // Return items in order and corresponding similarities
    std::vector<std::shared_ptr<const ImageItem>> items_ordered_by_similarity;
    items_ordered_by_similarity.reserve(kNumImages);

    for(const auto kIndex: indices) {
      items_ordered_by_similarity.emplace_back(this->dataset_->Items()[kIndex]);
    }

    return items_ordered_by_similarity;
}

std::vector<float> bagofwords::CompareHistogram(const std::vector<float>& qhistogram)
{
    int size = histogram_per_image_.size();
    std::vector<float> similarity(size, 0.0f);
    for (size_t i = 0; i < histogram_per_image_.size(); i++)
    {
        float cos_distance = pow(L2Norm(histogram_per_image_[i], qhistogram),2) /
                             (L2Norm(histogram_per_image_[i], histogram_per_image_[i]) *
                              L2Norm(qhistogram, qhistogram));
        similarity[i] = cos_distance;
    }

    return similarity;
}

float bagofwords::L2Norm(const std::vector<float>& h1, const std::vector<float>& h2)
{
        float sum = std::inner_product(h1.begin(), h1.end(), h2.begin(), 0.0f);
        return sqrt(sum);
}

void bagofwords::LoadCentroidsFromFile()
{
    std::cout << "* Read clustering centroids.\n";
    centroids_ = dataset_->LoadCentroids();
    int kNumClusters_ = centroids_.size();
    std::cout << "* Number of clusters (= words): " << kNumClusters_ << "\n";
}

void bagofwords::SaveHistogramImageDataset()
{
    std::vector<float> histogram(kNumClusters_);
    std::vector<std::vector<std::shared_ptr<const igg::ImageItem>>> images_per_cluster(kNumClusters_);
    //std::vector<int>::iterator ip;

    for (size_t i = 0; i < features_per_image_.size(); i++)
    {
        // i is the index of an image in dataset

        histogram = ComputeHistogram(features_per_image_[i], kNumClusters_);

        for (int k = 0; k < kNumClusters_; k++)
        {
            // k is the index of a cluster (word)

            if (histogram[k]>0.0f)
            {
                // This cluster (word) occurs in the current image
                images_per_cluster[k].emplace_back(dataset_->Items()[i]);
            }
        }
        histogram_per_image_.emplace_back(histogram);
    }

    std::vector<float> image_count_per_cluster;

    for (size_t j = 0; j < images_per_cluster.size(); j++)
    {
        // This is not needed as we only insert each image once per cluster to images_per_cluster
        //ip = std::unique(images_per_cluster[j].begin(), images_per_cluster[j].end());
        //images_per_cluster[j].resize(std::distance(images_per_cluster[j].begin(), ip));
        image_count_per_cluster.emplace_back(static_cast<float>(images_per_cluster[j].size()));
    }

    int index = 0;

    //Re-Weight histograms and write them to file for each image
    for (const std::shared_ptr<const igg::ImageItem> &kItem : dataset_->Items())
    {
        ReWeightHistogram(image_count_per_cluster, index);

        std::cout << " Write histogram to binary file.\n";
        igg::WriteToBinary<float>(kItem->HistogramBinaryPath(), histogram_per_image_[index]);

        index++;
    }
}

void bagofwords::ReWeightHistogram(const std::vector<float>& image_count_per_cluster, int index)
{
    // Total number of images for re-weighting
    const float kNumImages = static_cast<float>(dataset_->Items().size());

    std::cout << "  * Perfrom re-weighting.\n";
    // Number of features in this image for re-weighting
    const float kNumFeatures = std::accumulate(histogram_per_image_[index].begin(), histogram_per_image_[index].end(), 0.0f);

    for (size_t cluster = 0; cluster < histogram_per_image_[index].size(); cluster++)
    {
        histogram_per_image_[index][cluster] =
            histogram_per_image_[index][cluster] / kNumFeatures * std::log(kNumImages / image_count_per_cluster[cluster]);
    }
}

std::vector<float> bagofwords::ComputeHistogram(const std::vector<std::vector<float>>& feature_set, const int bins)
{
    std::vector<float> hist(bins, 0.0f);
    for (size_t i = 0; i < feature_set.size(); i++)
    {
        int cluster = NearestCluster(feature_set[i]);
        hist[cluster] += 1.0;
    }

    return hist;
}

int bagofwords::NearestCluster(const std::vector<float>& image_feature)
{
    float min_dist = 0.0;
    int NearestClusterId = 0;
    for (size_t i = 0; i < centroids_.size(); i++)
    {
        float distance = KSqnorm(centroids_[i], image_feature);

        if (distance < min_dist || i == 0)
        {
            min_dist = distance;
            NearestClusterId = i;
        }
    }

    return NearestClusterId;
}

float bagofwords::KSqnorm(const std::vector<float>& centroid, const std::vector<float>& image_feature)
{
    float sum = 0.0;

    if (centroid.size() == image_feature.size())
    {
        for (size_t i = 0; i < centroid.size(); i++)
        {
            sum += pow(centroid[i] - image_feature[i], 2);
        }
    }
    else
    {
      throw std::runtime_error("Dimension missmatch.");
    }

    return sum;
}

void bagofwords::LoadFeaturesFromFile()
{
    for (const std::shared_ptr<const igg::ImageItem> &kItem : dataset_->Items())
    {
        if (kItem->HasFeatures())
        {
            std::cout << "  * Load features binary file " << kItem->FeaturesBinaryFilename() << ".\n";
            cv::Mat mat = kItem->LoadFeatures();
            features_per_image_.emplace_back(igg::FromMat<float>(std::move(mat)));
        }
        else
        {
            std::cerr << "Extracted features not found: " << kItem->ImageFilename() << std::endl;
            std::cout << "Extracting Features for this Image" << std::endl;
            cv::Mat features = igg::ComputeFeatures(kItem->LoadImage());
            std::cout << "  * Extracted " << features.rows
                      << " features with " << features.cols << " dimensions.\n";
            features_per_image_.emplace_back(igg::FromMat<float>(std::move(features)));
            SaveFeaturesToFile(kItem, features);
        }
    }
    kFeatures_flatten_ = igg::Flatten(std::move(features_per_image_));
}

void bagofwords::ExtractFeaturesImageDataset()
{
    for (const std::shared_ptr<const igg::ImageItem> &kItem : dataset_->Items())
    {
        std::cout << "  * Load image " << kItem->ImageFilename() << ".\n";
        const cv::Mat kImage = kItem->LoadImage();
        cv::Mat features = igg::ComputeFeatures(kImage);
        std::cout << "  * Extracted " << features.rows
                  << " features with " << features.cols << " dimensions.\n";
        features_per_image_.emplace_back(igg::FromMat<float>(std::move(features)));
        SaveFeaturesToFile(kItem, features);
    }

    kFeatures_flatten_ = igg::Flatten(std::move(features_per_image_));
}

void bagofwords::SaveFeaturesToFile(const std::shared_ptr<const igg::ImageItem>& kItem, const cv::Mat& kFeatures)
{
    igg::WriteMatToBinary(kItem->FeaturesBinaryPath(), kFeatures);
    std::cout << "  * Write to file " << kItem->FeaturesBinaryFilename()
              << " (size: " << igg::FileSize(kItem->FeaturesBinaryPath()) << " bytes).\n";
}

void bagofwords::ComputeClusterCentroids(const std::string& kSelectedAlgorithm)
{
    if (kSelectedAlgorithm == "kmeans_vers_2")
    {
        std::cout << "Use clustering algorithm: kmeans_vers_2 (alternative implementation of K-means)\n";

        igg::ClusteringStrategyKmeansVers2<float> kmeans(kNumClusters_, kNumIterations_, kVerbose_);
        centroids_ = kmeans.ClusterCentroids(kFeatures_flatten_);
    }
    else if (kSelectedAlgorithm == "kmeans_opencv")
    {
        std::cout << "Use clustering algorithm: kmeans_opencv (OpenCV implementation of K-means)\n";
        const int kAttempts = 1;
        igg::ClusteringStrategyKmeansOpenCV<float> kmeans(kNumClusters_, kNumIterations_, kEpsilon_, kAttempts, kVerbose_);
        centroids_ = kmeans.ClusterCentroids(kFeatures_flatten_);
    }
    else
    {
        std::cout << "Use clustering algorithm: kmeans (own implementation of K-means)\n";
        const int kSeed = 0;
        igg::ClusteringStrategyKmeans<float> kmeans(kNumClusters_, kNumIterations_, kEpsilon_, kSeed, kVerbose_);
        centroids_ = kmeans.ClusterCentroids(kFeatures_flatten_);
    }
}

void bagofwords::SaveCentroidsToFile()
{
    igg::WriteCentroidsToBinary(dataset_->CentroidsPath(), centroids_);
    std::cout << "* Result written to " << dataset_->CentroidsPath() << ".\n";
}

} // namespace vers_2
} // namespace igg
