

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "bag_of_words_vers_2.hpp"
#include "web/web.hpp"
#include "web/html_writer.hpp"


int main(int argc, char** argv)
{
    if (argc<2)
    {
        std::cerr << "Error while parsing arguments: Please provide the path to the query image.\n";
        return 1;
    }

    const std::string kQueryImagePath = argv[1];

    const cv::Mat kQueryImage = cv::imread(kQueryImagePath, CV_LOAD_IMAGE_COLOR);

    if (!kQueryImage.data)
    {
        std::cerr << "Error while reading image.\n";
        return 1;
    }

    int kNumIterations = 100;
    double kEpsilon = 1e-3;
    int kNumClusters = 10;
    bool kVerbose = true;
    igg::vers_2::bagofwords bag_of_words(kNumIterations, kEpsilon, kNumClusters, kVerbose);

    std::vector<std::shared_ptr<const igg::ImageItem>> items_odered_by_similarity;

    try
    {
      items_odered_by_similarity = bag_of_words.SearchImage(kQueryImage);
    }
    catch (const std::exception& kError)
    {
        std::cerr << "Error while similarity query: " << kError.what() << "\n";
    }

    igg::HtmlWriter html_writer("bag_of_words_output.html");
    html_writer << igg::HtmlWriter::Header{};
    html_writer << igg::HtmlWriter::OpenBody{};

    html_writer << igg::HtmlWriter::Title{"Query:"};
    html_writer << igg::HtmlWriter::Image{kQueryImagePath};

    html_writer << igg::HtmlWriter::Title{"Most similar:"};
    size_t kMaxExamplesSimilar = 5; // Do not show more than the five most similar images
    size_t current_index = 0;
    for (const auto& kItem: items_odered_by_similarity) {
        if (current_index>=kMaxExamplesSimilar)
        {
            break;
        }
        current_index++;

        html_writer << igg::HtmlWriter::Image{kItem->ImagePath()};
    }

    std::cout << "Output was written to bag_of_words_output.html.\n";
}
