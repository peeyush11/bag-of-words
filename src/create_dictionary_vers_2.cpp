/**
 * @file create_dictionary_vers_2.cpp
 *
 * @author Peeyush Kumar
 * @version 1.0
 */

#include "iostream"
#include <boost/program_options.hpp>

#include "bag_of_words_vers_2.hpp"


int main(int argc, char** argv)
{
    int kNumIterations = 100;
    double kEpsilon = 1e-3;
    int kNumClusters = 10;
    bool kVerbose = true;
    igg::vers_2::bagofwords bag_of_words(kNumIterations, kEpsilon, kNumClusters, kVerbose);

    namespace po = boost::program_options;

    po::options_description options_description("options");
    options_description.add_options()
        ("algorithm,a", po::value<std::string>()->default_value("kmeans"), "clustering algorithm (options: kmeans, kmeans_vers_2, kmeans_opencv)");

    po::variables_map variables_map;
    try
    {
        po::store(po::parse_command_line(argc, argv, options_description), variables_map);
    }
    catch (po::error &error)
    {
        std::cerr << "Command not recognized.\n";
        std::cerr << error.what() << "\n.";
        return 1;
    }

    // Show help
    if (variables_map.count("help")) {
        std::cout << options_description;
        return 0;
    }

    const auto kSelectedAlgorithm = variables_map["algorithm"].as<std::string>();

    std::cout << "K-Means variant: " << kSelectedAlgorithm << "\n";

    try
    {
        bag_of_words.CreateDictionary(0, kSelectedAlgorithm); // 0 for extracting features (and not loading from file)
    }
    catch (const std::exception& kError)
    {
        std::cerr << "Error while creating dictionary: " << kError.what() << "\n";
    }
}
