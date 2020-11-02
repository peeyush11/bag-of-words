#include <gtest/gtest.h>

#include "web/html_writer.hpp"

#include "get_tests_data_path.hpp"


namespace igg {

TEST(DatasetWeb, HtmlWriter) {
  const auto kDocumentPath = GetTestsOutputPath()/"test.html";
  HtmlWriter html_writer(kDocumentPath.string());

  html_writer << HtmlWriter::Header{};
  html_writer << HtmlWriter::OpenBody{};
  html_writer << HtmlWriter::OpenItemDiv{"Item title"};

  html_writer << HtmlWriter::Title{"Group 1"};
  html_writer << HtmlWriter::OpenImageGroupDiv{};
  html_writer << HtmlWriter::OpenImageDiv{};
  html_writer << HtmlWriter::Image{"test_image_1.png"};
  html_writer << HtmlWriter::Image{"test_image_2.png"};
  html_writer << HtmlWriter::CloseImageDiv{"Caption 1"};
  html_writer << HtmlWriter::OpenImageDiv{};
  html_writer << HtmlWriter::Image{"test_image_3.png"};
  html_writer << HtmlWriter::Image{"test_image_4.png"};
  html_writer << HtmlWriter::CloseImageDiv{"Caption 2"};
  html_writer << HtmlWriter::CloseImageGroupDiv{};

  html_writer << HtmlWriter::Title{"Group 2"};
  html_writer << HtmlWriter::OpenImageGroupDiv{};
  html_writer << HtmlWriter::OpenImageDiv{};
  html_writer << HtmlWriter::Image{"test_image_5.png"};
  html_writer << HtmlWriter::Image{"test_image_6.png"};
  html_writer << HtmlWriter::CloseImageDiv{"Caption 3"};
  html_writer << HtmlWriter::CloseImageGroupDiv{};

  html_writer << HtmlWriter::CloseItemDiv{};
  html_writer << HtmlWriter::CloseBody{};
}


} // namespace igg
