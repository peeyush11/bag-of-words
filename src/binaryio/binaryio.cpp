
#include "binaryio.hpp"


namespace igg {

bool WriteMatToBinary(const std::string& kPath, const cv::Mat& kMat)
{
  // Reference: https://github.com/takmin/BinaryCvMat/blob/master/BinaryCvMat.cpp
  auto file = std::ofstream
    (kPath, std::ofstream::binary|std::ofstream::out|std::ofstream::trunc);
  if (!file.is_open()) {
    std::cerr << "Cannot write to file " << kPath << ".\n";
    return false;
  }

  // Write header information
  int rows = kMat.rows;
  int cols = kMat.cols;
  int type = kMat.type();
  file.write(reinterpret_cast<char*>(&rows), sizeof(int));
  file.write(reinterpret_cast<char*>(&cols), sizeof(int));
  file.write(reinterpret_cast<char*>(&type), sizeof(int));

  // Write matrix data
  file.write(reinterpret_cast<char*>(kMat.data), kMat.elemSize()*kMat.total());

  return true;
}


cv::Mat ReadMatFromBinary(const std::string& kPath)
{
  // Reference: https://github.com/takmin/BinaryCvMat/blob/master/BinaryCvMat.cpp
  std::ifstream file = std::ifstream
    (kPath, std::ifstream::binary|std::ifstream::in);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file "+kPath+".");
  }

  // Read header information
  int rows = 0;
  int cols = 0;
  int type = 0;
  file.read(reinterpret_cast<char*>(&rows), sizeof(int));
  file.read(reinterpret_cast<char*>(&cols), sizeof(int));
  file.read(reinterpret_cast<char*>(&type), sizeof(int));

  // Read matrix data
  auto mat = cv::Mat(rows, cols, type);
  file.read(reinterpret_cast<char*>(mat.data), mat.elemSize()*mat.total());
  return mat;
}


std::streamsize FileSize(const std::string& kPath) {
  // Reference: https://stackoverflow.com/questions/2409504/
  // using-c-filestreams-fstream-how-can-you-determine-the-size-of-a-file
  std::ifstream file = std::ifstream
    (kPath, std::ifstream::binary|std::ifstream::in);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file "+kPath+".");
  }
  // Skip all bytes (until maximum stream size)
  file.ignore(std::numeric_limits<std::streamsize>::max());
  // See how many bytes were skipped (which is the file size)
  return file.gcount();
}


bool FileExists(const std::string& kPath) {
  std::ifstream file = std::ifstream
    (kPath, std::ifstream::binary|std::ifstream::in);
  if (!file.is_open()) {
    return false;
  }
  return true;
}

} // namespace igg

