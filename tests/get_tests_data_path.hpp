#ifndef CPP_FINAL_PROJECT_TESTS_GET_TESTS_DATA_PATH_HPP_
#define CPP_FINAL_PROJECT_TESTS_GET_TESTS_DATA_PATH_HPP_

/**
 * get_test_data_path.hpp
 *
 * @author Jan Quakernack
 *
 * @version 1.0
 *
 */

#include <boost/filesystem.hpp>


namespace igg {

namespace fs = boost::filesystem;

/**
 * The purpose of this function is to locate the directory named tests_data/
 * containing the data required for various test related to the project even
 * if it is not directly below the current directory but in some subdirectory.
 *
 * A std::runtime_error thrown if it cannot be located.
 *
 * @param kMaxIterations The maximum number of directory entry checked before
 * throwing an exception.
 *
 * @return The path to tests_data/.
 */
fs::path GetTestsDataPath(const size_t kMaxIterations=10000);

/**
 * The purpose of this function is to locate the directory named tests_output/
 * containing the output data of various test related to the project even
 * if it is not directly below the current directory but in some subdirectory.
 *
 * A std::runtime_error thrown if it cannot be located.
 *
 * @param kMaxIterations The maximum number of directory entry checked before
 * throwing an exception.
 *
 * @return The path to tests_output/.
 */
fs::path GetTestsOutputPath(const size_t kMaxIterations=10000);

} // namespace igg

#endif // CPP_FINAL_PROJECT_TESTS_GET_TESTS_DATA_PATH_HPP_
