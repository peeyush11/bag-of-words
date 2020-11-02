#ifndef CPP_FINAL_PROJECT_TESTS_MAKE_TEST_DATASET_HPP_
#define CPP_FINAL_PROJECT_TESTS_MAKE_TEST_DATASET_HPP_

#include <boost/filesystem.hpp>

#include "dataset/dataset.hpp"

#include "get_tests_data_path.hpp"


namespace igg {

namespace fs = boost::filesystem;

Dataset MakeTestDataset();


} // namespace igg

#endif // CPP_FINAL_PROJECT_TESTS_MAKE_TEST_DATASET_HPP_
