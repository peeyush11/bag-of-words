/**
 * @file make_histograms.cpp
 *
 * Provides a command line interface for the histogram generation and re-weighting step.
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
    ("help,h", "Show help.");

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
    std::cout << "Generates and re-weights histograms from clustering.\n";
    std::cout << "Please make sure the CPP_FINAL_PROJECT_DATA_DIR environment variable is set, "
      "features have be extracted and clustered.\n";
    return 0;
  }

  const auto kDataset = igg::Dataset::Default();
  if (!kDataset) {std::cerr << "Error while loading dataset.\n"; return 1;}

  const igg::BagOfWords kBagOfWords(kDataset, true); // True to allow terminal output

  try {
    kBagOfWords.MakeHistograms();
  } catch (const std::exception& kError) {
    std::cerr << "An error occured: " << kError.what() << "\n";
    return 1;
  }

  return 0;
}
