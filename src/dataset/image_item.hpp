#ifndef CPP_FINAL_PROJECT_DATASET_IMAGE_ITEM_HPP_
#define CPP_FINAL_PROJECT_DATASET_IMAGE_ITEM_HPP_

/**
 * @file image_item.hpp
 *
 * @author Jan Quakernack
 *
 * @version 1.0
 */

#include <string>
#include <memory>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

#include "binaryio/binaryio.hpp"


/**
 * This class abstracts and image file as an element of an image dataset.
 *
 * It provides functionality to access the images filepath as well as loading
 * the image as a cv::Mat or various intermediate results.
 */
namespace igg {

class ImageItem {
public:
   /**
    * Constructor.
    *
    * @param kImagePath Path where the image file is located.
    * @param kFeaturesBinaryPath Path where the extracted features can be stored.
    * @param kHistogramBinaryPath_Path where the generated histogram
    * representation of this image can be stored.
    */
   ImageItem
     (const std::string kImagePath,
      const std::string kFeaturesBinaryPath,
      const std::string kHistogramBinaryPath);

   /**
    * Path where the image file is located.
    */
   std::string ImagePath() const {return this->kImagePath_;}

   /**
    * Image filename.
    */
   std::string ImageFilename() const;

   /**
    * Image filename without file extension (used for web output).
    */
   std::string ImageFilestem() const;

   /**
    * Load the image as an instance of cv::Mat.
    */
   cv::Mat LoadImage() const;

   /**
    * Path to binary file containing the extracted features.
    *
    * Note it may not exists yet.
    */
   std::string FeaturesBinaryPath() const {return this->kFeaturesBinaryPath_;}

   /**
    * Filename of the binary file containing the extracted features.
    */
   std::string FeaturesBinaryFilename() const;

   /**
    * Check if a binary file with extracted features exists.
    */
   bool HasFeatures() const {return FileExists(this->kFeaturesBinaryPath_);}

   /**
    * Load the extracted features from file as an instance of cv::Mat.
    */
   cv::Mat LoadFeatures() const;

   /**
    * Path to binary file containing the histogram representation of this image.
    *
    * Note it may not exists yet.
    */
   std::string HistogramBinaryPath() const {return this->kHistogramBinaryPath_;}

   /**
    * Filename of the file containing the histogram representation of this image.
    */
   std::string HistogramBinaryFilename() const;

   /**
    * Check if a binary file containing the histogram representation of this image exists.
    */
   bool HasHistogram() const {return FileExists(this->kHistogramBinaryPath_);}

   /**
    * Loads the histogram representation of this image from file.
    */
   Histogram<float> LoadHistogram() const;

 private:
   const std::string kImagePath_;
   const std::string kFeaturesBinaryPath_;
   const std::string kHistogramBinaryPath_;
};

} // namespace igg

#endif // CPP_FINAL_PROJECT_DATASET_IMAGE_ITEM_HPP_

