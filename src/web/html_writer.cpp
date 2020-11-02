/**
 * @file html_writer.cpp
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include "html_writer.hpp"

namespace igg {
  HtmlWriter::HtmlWriter(const std::string kDocumentPath):
    file_{std::ofstream(kDocumentPath, std::ofstream::out)}
  {
    if (!this->file_.is_open()){throw std::runtime_error("Failed to open "+kDocumentPath+".");}
    // Stream is closed automatically by destructor
  }


  template <>
  HtmlWriter& HtmlWriter::operator<<(const HtmlWriter::Header& kTag) {
    this->file_ << "<!DOCTYPE html>\n";
    this->file_ << "<html>\n";
    this->file_ << "<head>\n";
    this->file_ << "  <link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n";
    this->file_ << "</head>\n";
    return *this;
  }


  template <>
  HtmlWriter& HtmlWriter::operator<<(const HtmlWriter::OpenBody& kTag) {
    this->file_ << "<body>\n";
    return *this;
  }


  template <>
  HtmlWriter& HtmlWriter::operator<<(const HtmlWriter::CloseBody& kTag) {
    this->file_ << "</body>\n";
    this->file_ << "</html>\n";
    return *this;
  }

  template <>
  HtmlWriter& HtmlWriter::operator<<(const HtmlWriter::OpenImageGroupDiv& kTag) {
    this->file_ << "<div class=\"image_group\">\n";
    return *this;
  }


  template <>
  HtmlWriter& HtmlWriter::operator<<(const HtmlWriter::CloseImageGroupDiv& kTag) {
    this->file_ << "</div>\n";
    return *this;
  }


  template <>
  HtmlWriter& HtmlWriter::operator<<(const HtmlWriter::OpenItemDiv& kTag) {
    this->file_ << "<h2 class=\"title\">" << kTag.kTitle << "</h2>\n";
    this->file_ << "<div class=\"dataset_item\">\n";
    return *this;
  }


  template <>
  HtmlWriter& HtmlWriter::operator<<(const HtmlWriter::CloseItemDiv& kTag) {
    this->file_ << "</div>\n";
    return *this;
  }


  template <>
  HtmlWriter& HtmlWriter::operator<<(const HtmlWriter::Title& kTag) {
    this->file_ << "<h3 class=\"subtitle\">" << kTag.kTitle << "</h3>\n";
    return *this;
  }


  template <>
  HtmlWriter& HtmlWriter::operator<<(const HtmlWriter::OpenImageDiv& kTag) {
    this->file_ << "<div class=\"image\">\n";
    return *this;
  }


  template <>
  HtmlWriter& HtmlWriter::operator<<(const HtmlWriter::CloseImageDiv& kTag) {
    this->file_ << "<h4 class=\"caption\">" << kTag.kCaption << "</h4>\n";
    this->file_ << "</div>\n";
    return *this;
  }


  template <>
  HtmlWriter& HtmlWriter::operator<<(const HtmlWriter::Image& kTag) {
    this->file_ << "<img class= \"image\" src=\"" << kTag.kPath << "\">\n";
    return *this;
  }

} // namespace igg

