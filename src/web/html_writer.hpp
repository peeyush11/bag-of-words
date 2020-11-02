#ifndef CPP_FINAL_PROJECT_HTML_WRITER_HPP_
#define CPP_FINAL_PROJECT_HTML_WRITER_HPP_

/**
 * @file html_writer.hpp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <string>
#include <fstream>


namespace igg {

/**
 * The prupose of this class is to provide an easy way of writing html tags to structur our output.
 *
 * Usage:
 *
 *   HtmlWriter writer("index.html"); // Will create a new file in the current directory
 *
 *   html_writer << HtmlWriter::Header{};
 *
 *   html_writer << HtmlWriter::OpenBody{};
 *   html_writer << HtmlWriter::Title{"Title goes here"};
 *   html_writer << HtmlWriter::Image{"images/image_1.jpg"};
 *   html_writer << HtmlWriter::Image{"images/image_2.jpg"};
 *   html_writer << HtmlWriter::CloseBody{};
 *
 * See below for all supported elements.
 *
 */
class HtmlWriter {
public:
  HtmlWriter(const std::string kDocumentPath);

  template <class TagType>
  HtmlWriter& operator<<(const TagType& kTag);

  struct Header {};
  struct OpenBody {};
  struct CloseBody {};
  struct OpenItemDiv {const std::string kTitle;};
  struct CloseItemDiv {};
  struct Title {const std::string kTitle;};
  struct OpenImageGroupDiv {};
  struct CloseImageGroupDiv {};
  struct OpenImageDiv {};
  struct CloseImageDiv{const std::string kCaption;};
  struct Image {const std::string kPath;};

private:
  std::ofstream file_;

};

} // namespace igg

#endif // CPP_FINAL_PROJECT_HTML_WRITER_
