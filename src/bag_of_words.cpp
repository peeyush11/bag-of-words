/**
 * @file bag_of_words.cpp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include "bag_of_words.hpp"

#include "features/features.hpp"
#include "binaryio/binaryio.hpp"
#include "histogram/histogram.hpp"
#include "web/web.hpp"
#include "web/html_writer.hpp"


namespace igg {

BagOfWords::BagOfWords
  (const std::shared_ptr<const Dataset> kDataset, const bool kVerbose):
  kDataset_{kDataset}, verbose_{kVerbose}
{
  if (this->verbose_) {
    std::cout << "Load dataset containing " <<
      this->kDataset_->Items().size() << " images.\n";
  }
}


void BagOfWords::CreateDictionary
  (const ClusteringStrategy<float>& kStrategy, const bool kRecompute) const
{
  if (kRecompute || !this->kDataset_->AllItemsHaveFeatures())
    {this->ExtractFeatures();}
  if (kRecompute || !this->kDataset_->HasCentroids())
    {this->ComputeClusterCentroids(kStrategy);}
  if (kRecompute || !this->kDataset_->AllItemsHaveHistograms())
    {this->MakeHistograms();}
}


std::vector<float> BagOfWords::Similarities
  (const std::shared_ptr<const ImageItem> kQueryItem) const
{
  const auto kNumImages = this->kDataset_->Items().size();

  // Read histograms
  std::vector<Histogram<float>> histograms;
  histograms.reserve(kNumImages);
  for(const auto& kItem: this->kDataset_->Items()) {
    Histogram<float> histogram;
    try {
      histogram = kItem->LoadHistogram();
    } catch (const std::runtime_error&) {
      throw DictionaryIncomplete
        ("Expected to find histogram binary "+kItem->HistogramBinaryFilename()+
         ", but it seems like it cannot be loaded. Did you call CreateDictionary()?");
    }
    histograms.emplace_back(std::move(histogram));
  }
  Histogram<float> query_histogram;
  try {
    query_histogram = kQueryItem->LoadHistogram();
  } catch (const std::runtime_error&) {
    throw DictionaryIncomplete
      ("Expected to find histogram binary "+kQueryItem->HistogramBinaryFilename()+
       ", but it seems like it cannot be loaded. Did you call CreateDictionary()?");
  }

  return ComputeSimilarities<float>(query_histogram, histograms);
}


std::pair
  <std::vector<std::shared_ptr<const ImageItem>>,
   std::vector<float>>
  BagOfWords::OrderItemsBySimilarity
  (const std::vector<float>& kSimilarities) const
{
  const auto kNumImages = this->kDataset_->Items().size();

  if (kSimilarities.size()!= kNumImages) {
    throw std::invalid_argument
      ("Number of similarities does not match number of images in dataset.");
  }

  // Perform argsort (get the indices that would sort the similarities vector)
  // Reference: https://stackoverflow.com/questions/1577475/
  // c-sorting-and-keeping-track-of-indexes
  std::vector<size_t> indices(kNumImages);
  // Populate with indices in increasing order
  std::iota(indices.begin(), indices.end(), 0);
  // Use lambda expression for sorting
  std::sort
    (indices.begin(), indices.end(),
     [&kSimilarities](const size_t kIndex1, const size_t kIndex2)
       {return kSimilarities[kIndex1]>kSimilarities[kIndex2];});

  // Return items in order and corresponding similarities
  std::vector<std::shared_ptr<const ImageItem>> items_ordered_by_similarity;
  items_ordered_by_similarity.reserve(kNumImages);

  std::vector<float> ordered_similarities;
  ordered_similarities.reserve(kNumImages);

  for(const auto kIndex: indices) {
    items_ordered_by_similarity.emplace_back(this->kDataset_->Items()[kIndex]);
    ordered_similarities.emplace_back(kSimilarities[kIndex]);
  }

  return std::make_pair(items_ordered_by_similarity, ordered_similarities);
}


bool BagOfWords::DictionaryComplete() const
{
  if (!this->kDataset_->HasHistogramWeights()) {return false;}
  if (!this->kDataset_->HasCentroids()) {return false;}
  if (!this->kDataset_->AllItemsHaveHistograms()) {return false;}
  if (!this->kDataset_->AllItemsHaveFeatures()) {return false;}
  return true;
}


void BagOfWords::ExtractFeatures() const {
  if (this->verbose_) {std::cout << "Start extracting features.\n";}

  for (const auto& kItem : this->kDataset_->Items()) {
    if (this->verbose_) {std::cout << "* Load image " << kItem->ImageFilename() << ".\n";}
    const auto kImage = kItem->LoadImage();
    const auto kFeatures = ComputeFeatures(kImage);
    if (this->verbose_) {
      std::cout << "* Extracted " << kFeatures.rows <<
        " features with " << kFeatures.cols << " dimensions each.\n";
    }
    WriteMatToBinary(kItem->FeaturesBinaryPath(), kFeatures);
    if (this->verbose_) {
      std::cout << "* Write features to " << kItem->FeaturesBinaryFilename() <<
        " (size: " << FileSize(kItem->FeaturesBinaryPath()) << " bytes).\n";
    }
  }

  if (this->verbose_) {std::cout << "Done extracting features.\n";}
}


void BagOfWords::ComputeClusterCentroids(const ClusteringStrategy<float>& kStrategy) const {
  if (this->verbose_) {std::cout << "Start clustering.\n";}

  // Collect extracted features from all images
  const auto kNumImages = this->kDataset_->Items().size();
  std::vector<std::vector<FeaturePoint<float>>> features_per_image;
  features_per_image.reserve(kNumImages);

  for (const auto& kItem: this->kDataset_->Items()) {
    if (this->verbose_) {std::cout << "* Load features binary " << kItem->FeaturesBinaryFilename() << ".\n";}
    cv::Mat mat;
    try {
      mat = kItem->LoadFeatures();
    } catch (const std::runtime_error&) {
      throw DictionaryIncomplete
        ("Expected to find features binary "+kItem->FeaturesBinaryFilename()+
         ", but it seems like it cannot be loaded. Did you call CreateDictionary()?");
    }
    features_per_image.emplace_back(std::move(FromMat<float>(std::move(mat))));
  }

  const auto kFeatures = Flatten(std::move(features_per_image));

  // Perform actual clustering
  std::vector<FeaturePoint<float>> centroids = kStrategy.ClusterCentroids(kFeatures);

  WriteCentroidsToBinary(this->kDataset_->CentroidsPath(), centroids);
  if (this->verbose_) {std::cout << "* Write cluster centroids to " << this->kDataset_->CentroidsPath() << ".\n";}

  if (this->verbose_) {std::cout << "Done clustering.\n";}
}


void BagOfWords::MakeHistograms() const {
  if (this->verbose_) {std::cout << "Start computing histograms.\n";}

  if (this->verbose_) {std::cout << "* Read cluster centroids.\n";}
  std::vector<FeaturePoint<float>> centroids;
  try {
    centroids = this->kDataset_->LoadCentroids();
  } catch (const std::runtime_error&) {
    throw DictionaryIncomplete
      ("Expected to find centroids binary, but it seems like it cannot be loaded. "
       "Did you call CreateDictionary()?");
  }

  const auto kNumClusters = centroids.size();
  if (this->verbose_) {std::cout << "* Number of clusters (= words): " << kNumClusters << "\n";}

  // Track which images occur in each cluster (needed to reweight histogram bins)
  // Only need to rembember each images once, therefor we use std::set
  std::vector<std::set<std::shared_ptr<const ImageItem>>> cluster_occurences
    (kNumClusters, std::set<std::shared_ptr<const ImageItem>>());

  for (const auto kItem: this->kDataset_->Items()) {
    if (this->verbose_) {std::cout << "* Load features binary " << kItem->FeaturesBinaryFilename() << ".\n";}
    cv::Mat mat;
    try {
      mat = kItem->LoadFeatures();
    } catch (const std::runtime_error&) {
      throw DictionaryIncomplete
        ("Expected to find features binary "+kItem->FeaturesBinaryFilename()+
         ", but it seems like it cannot be loaded. Did you call CreateDictionary()?");
    }
    const auto kFeatures = FromMat<float>(std::move(mat));

    if (this->verbose_) {std::cout << "* Assign clusters and make histogram.\n";}
    // Make a histogram with one bin for each cluster
    Histogram<float> histogram(kNumClusters, 0.0f);

    for (const auto& kFeature: kFeatures) {
      // Find the cluster this feature belongs to
      const size_t kCluster = NearestNeighbor(kFeature, centroids);

      // Update histrogram
      histogram[kCluster] += 1.0f;

      // Update cluster occurences
      cluster_occurences[kCluster].emplace(kItem);
    }

    if (this->verbose_) {std::cout << "* Write histogram to binary.\n";}
    WriteToBinary(kItem->HistogramBinaryPath(), histogram);
  }

  // Number of images in each cluster for re-weighting
  std::vector<float> cluster_counts;
  cluster_counts.reserve(kNumClusters);
  for (const auto& kClusterOccurence: cluster_occurences) {
    cluster_counts.emplace_back(static_cast<float>(kClusterOccurence.size()));
  }

  // Compute histogram re-weighting factors
  // (Logarithm of total number of images over number images in which the cluster occured)
  const float kNumImages = static_cast<float>(this->kDataset_->Items().size());
  std::vector<float> histogram_weights;
  histogram_weights.reserve(kNumClusters);
  for (const auto& kClusterOccurence: cluster_occurences) {
    histogram_weights.emplace_back(std::log(kNumImages/static_cast<float>(kClusterOccurence.size())));
  }

  // Write weights to file
  if (this->verbose_) {
    std::cout << "* Write histogram weights to binary " <<
    this->kDataset_->HistogramWeightsPath() << ".\n";
  }
  WriteToBinary<float>(this->kDataset_->HistogramWeightsPath(), histogram_weights);

  if (this->verbose_) {std::cout << "* Re-weight histograms.\n";}
  for (const auto kItem: this->kDataset_->Items()) {
    if (this->verbose_) {std::cout << "* Load histogram binary " << kItem->HistogramBinaryFilename() << ".\n";}
    Histogram<float> histogram;
    try {
      histogram = igg::ReadFromBinary<float>(kItem->HistogramBinaryPath());
    } catch (const std::runtime_error&) {
      throw DictionaryIncomplete
        ("Expected to find histogram binary "+kItem->HistogramBinaryFilename()+
         ", but it seems like it cannot be loaded. Did you call CreateDictionary()?");
    }

    if (this->verbose_) {std::cout << "* Perfrom re-weighting.\n";}
    // Number of features in this image for re-weighting
    const float kNumFeatures = std::accumulate
      (histogram.begin(), histogram.end(), 0.0f); // 0.0f is initial value

    for (size_t cluster = 0; cluster<kNumClusters; cluster++) {
      histogram[cluster] = histogram[cluster]/kNumFeatures*histogram_weights[cluster];
    }

    if (this->verbose_) {std::cout << "* Write re-weighted histogram to binary file.\n";}
    igg::WriteToBinary<float>(kItem->HistogramBinaryPath(), histogram);
  }

  if (this->verbose_) {std::cout << "Done computing histograms.\n";}
}


void BagOfWords::MakeWebOutput
  (const size_t kNumExamples,
   const size_t kNumExamplesSimilar,
   const size_t kNumExamplesDifferent,
   const unsigned int kExampleImageSize,
   const unsigned int kExampleImageSizeSmall) const
{
  if (!this->DictionaryComplete()) {
    throw DictionaryIncomplete
      ("Visual dictionary is not in place. Did you call CreateDictionary()?");
  }

  if (this->verbose_) {std::cout << "Start generating web output.\n";}

  namespace fs = boost::filesystem;

  const auto kNumImages = this->kDataset_->Items().size();

  const auto kWebDir = fs::path(this->kDataset_->WebDir());
  const auto kIndexFilePath = kWebDir/"index.html";
  const auto kStyleFilePath = kWebDir/"style.css";

  // Clean up previous output
  if (fs::exists(kWebDir)) {
    fs::remove_all(kWebDir);
    fs::create_directory(kWebDir);
  }

  // Create a directory to store jpgs of all images in dataset
  const auto kWebImagesDir = kWebDir/"images/";
  fs::create_directory(kWebImagesDir);

  // Save jpgs of all images in dataset
  for (const auto& kItem : this->kDataset_->Items()) {
    if (this->verbose_) {std::cout << "* Rescale and save as jpg: " << kItem->ImageFilestem() << ".\n";}
    const auto kImage = kItem->LoadImage();

    // Two versions at different scales for displaying
    const auto kImagePath = kWebImagesDir/(kItem->ImageFilestem()+".jpg");
    igg::RescaleAndSave(kImage, kExampleImageSize, kImagePath);

    // Smaller version
    const auto kImagePathSmall = kWebImagesDir/(kItem->ImageFilestem()+"_small.jpg");
    igg::RescaleAndSave(kImage, kExampleImageSizeSmall, kImagePathSmall);

    // Save histogram plt
    cv::Mat histogram_plot
      = igg::MakeHistogramPlot(kItem, kExampleImageSize, kExampleImageSize);
    const auto kHistogramPlotPath
      = kWebImagesDir/(kItem->ImageFilestem()+"_hist.jpg");
    cv::imwrite(kHistogramPlotPath.string(), histogram_plot);

    // Smaller version
    cv::Mat histogram_plot_small
      = igg::MakeHistogramPlot(kItem, kExampleImageSizeSmall, kExampleImageSizeSmall);
    const auto kHistogramPlotPathSmall
      = kWebImagesDir/(kItem->ImageFilestem()+"_hist_small.jpg");
    cv::imwrite(kHistogramPlotPathSmall.string(), histogram_plot_small);
  }

  // Write html
  HtmlWriter html_writer(kIndexFilePath.string());

  html_writer << HtmlWriter::Header{};
  html_writer << HtmlWriter::OpenBody{};

  for (size_t image_index = 0; image_index<kNumExamples && image_index<kNumImages; image_index++)
  {
    const auto kItem = this->kDataset_->Items()[image_index];

    if (this->verbose_) {std::cout << "* Get similarities: " << kItem->ImageFilestem() << ".\n";}
    const auto kSimilarities = this->Similarities(kItem);
    const auto kOrderedResults = this->OrderItemsBySimilarity(kSimilarities);
    // Returns pair of ordered items and corresponding similarities

    // Unpack pair
    std::vector<std::shared_ptr<const igg::ImageItem>> items_ordered_by_similarity;
    std::vector<float> ordered_similarities;
    std::tie(items_ordered_by_similarity, ordered_similarities) = kOrderedResults;

    if (this->verbose_) {std::cout << "* Add to html output: " << kItem->ImageFilestem() << ".\n";}

    html_writer << HtmlWriter::OpenItemDiv{"Image: "+kItem->ImageFilename()};

    html_writer << HtmlWriter::OpenImageGroupDiv{} << HtmlWriter::Title{"Query:"} << HtmlWriter::OpenImageDiv{};
    html_writer << HtmlWriter::Image{"images/"+kItem->ImageFilestem()+".jpg"};
    html_writer << HtmlWriter::Image{"images/"+kItem->ImageFilestem()+"_hist.jpg"};
    html_writer << HtmlWriter::CloseImageDiv{""} << HtmlWriter::CloseImageGroupDiv{};

    html_writer << HtmlWriter::OpenImageGroupDiv{} << HtmlWriter::Title{"Most similar:"};
    for (size_t image_index = 1; image_index<=kNumExamplesSimilar && image_index<kNumImages; image_index++) {
      html_writer << HtmlWriter::OpenImageDiv{};
      html_writer << HtmlWriter::Image{"images/"+items_ordered_by_similarity[image_index]->ImageFilestem()+"_small.jpg"};
      html_writer << HtmlWriter::Image{"images/"+items_ordered_by_similarity[image_index]->ImageFilestem()+"_hist_small.jpg"};
      html_writer << HtmlWriter::CloseImageDiv{std::to_string(ordered_similarities[image_index])};
    }
    html_writer << HtmlWriter::CloseImageGroupDiv{};

    html_writer << HtmlWriter::OpenImageGroupDiv{} << HtmlWriter::Title{"Most different:"};
    for (size_t image_index = kNumImages-1; image_index>=kNumImages-kNumExamplesDifferent; image_index--) {
      html_writer << HtmlWriter::OpenImageDiv{};
      html_writer << HtmlWriter::Image{"images/"+items_ordered_by_similarity[image_index]->ImageFilestem()+"_small.jpg"};
      html_writer << HtmlWriter::Image{"images/"+items_ordered_by_similarity[image_index]->ImageFilestem()+"_hist_small.jpg"};
      html_writer << HtmlWriter::CloseImageDiv{std::to_string(ordered_similarities[image_index])};
    }
    html_writer << HtmlWriter::CloseImageGroupDiv{} << HtmlWriter::CloseItemDiv{};
  }

  html_writer << HtmlWriter::CloseBody{};

  // Create style file
  igg::WriteStyleFile(kStyleFilePath);

  if (this->verbose_) {std::cout << "Done generating web output.\n";}
}

} // namespace igg
