/**
 * @file make_histograms.cpp
 *
 * Provides a command line interface for the final visualization of the results.
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <boost/program_options.hpp>

#include "bag_of_words.hpp"


int main (int argc, char** argv) {
  // Parse terminal input using boost functionality
  namespace po = boost::program_options;

  po::options_description options_description("Options");
  options_description.add_options()
    ("help,h", "Show help.")
    ("num-examples,", po::value<size_t>()->default_value(100), "Number of example images.")
    ("num-examples-similar,", po::value<size_t>()->default_value(10), "How many of the most similar images are shown for each example.")
    ("num-examples-different,", po::value<size_t>()->default_value(10), "How many of the most different images are shown for each example.")
    ("example-image-size,", po::value<int>()->default_value(256), "Size of example images.")
    ("example-image-size-small,", po::value<int>()->default_value(128), "Size of example images (small version).");
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
    std::cout << "Please make sure the CPP_FINAL_PROJECT_DATA_DIR environment variable is set, "
      "and all previous processing steps were executed.\n";
    std::cout << options_description;
    return 0;
  }

  // Read parameters
  const size_t kNumExamples = variables_map["num-examples"].as<size_t>();
  const size_t kNumExamplesSimilar = variables_map["num-examples-similar"].as<size_t>();
  const size_t kNumExamplesDifferent = variables_map["num-examples-different"].as<size_t>();
  const int kExampleImageSize = variables_map["example-image-size"].as<int>();
  const int kExampleImageSizeSmall = variables_map["example-image-size-small"].as<int>();
  if (!(kExampleImageSize>0) || !(kExampleImageSizeSmall>0)) {
    std::cerr << "Image size is expected to be positive.\n";
    return 1;
  }

  const auto kDataset = igg::Dataset::Default();
  if (!kDataset) {std::cerr << "Error while loading dataset.\n"; return 1;}

  const igg::BagOfWords kBagOfWords(kDataset, true); // True to allow terminal output

  try {
    kBagOfWords.MakeWebOutput(kNumExamples, kNumExamplesSimilar, kNumExamplesDifferent, kExampleImageSize, kExampleImageSizeSmall);
  } catch (const std::exception& kError) {
    std::cerr << "An error occured: " << kError.what() << "\n";
    return 1;
  }

  return 0;
}
