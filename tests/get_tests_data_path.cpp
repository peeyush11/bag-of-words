#include <iostream>

#include "get_tests_data_path.hpp"


namespace igg {

namespace { // Put inside nameless namespace to prevent access from outside this file

// Locate any directory below the current directory
fs::path LocateDirectory(const std::string& kDirName, const size_t kMaxIterations) {
  // Try to locate directory by traversing subdirectories recursively
  fs::recursive_directory_iterator dir_iterator(fs::path("."));
  fs::recursive_directory_iterator end_iterator;
  // Reference for usage end_iterator: https://stackoverflow.com/questions/67273/
  // how-do-you-iterate-through-every-file-directory-recursively-in-standard-c

  size_t iteration = 0;
  while (dir_iterator!=end_iterator) {
    if (iteration>=kMaxIterations) {break;} // To prevent searching endlessly through the whole filesystem
    iteration++;
    if (fs::is_directory(*dir_iterator) && (*dir_iterator).path().filename()==kDirName) {
      return (*dir_iterator).path();
    }
    dir_iterator++;
  }

  throw std::runtime_error
    ("Unable to locate directory "+kDirName+".");

}

} // nameless namespace


fs::path GetTestsDataPath(const size_t kMaxIterations) {
  return LocateDirectory("tests_data", kMaxIterations);
}


fs::path GetTestsOutputPath(const size_t kMaxIterations) {
  return LocateDirectory("tests_output", kMaxIterations);
}

} // namespace igg
