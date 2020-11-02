#include "make_test_dataset.hpp"

namespace igg {

Dataset MakeTestDataset() {
  const auto kTestsDataPath = GetTestsDataPath();
  const auto kTestsOutputPath = GetTestsOutputPath();

  // Copy the test images, so we do not accidentially break our test set
  // in case something goes wrong
  const auto kTestDatasetDir = kTestsOutputPath/"test_dataset";
  // Clean up previous output
  if (fs::exists(kTestDatasetDir)) {fs::remove_all(kTestDatasetDir);}
  fs::create_directory(kTestDatasetDir);
  const auto kTestDatasetImagesDir = kTestDatasetDir/"images";
  fs::create_directory(kTestDatasetImagesDir);

  // Where the test images are located
  const auto kTestImagesDir = kTestsDataPath/"coil-25";

  for (const auto& kDirEntry : fs::recursive_directory_iterator(kTestImagesDir)) {
    if (!fs::is_directory(kDirEntry) && kDirEntry.path().extension()==".png") {
      const auto kDestination = kTestDatasetImagesDir/kDirEntry.path().filename();
      fs::copy_file(kDirEntry, kDestination);
    }
  }

  return Dataset(kTestDatasetDir.string());
}

}
