#ifndef CPP_FINAL_PROJECT_TOOLS_TERMINALOUT_HPP_
#define CPP_FINAL_PROJECT_TOOLS_TERMINALOUT_HPP_

/**
 * @file terminalout.hpp
 *
 * The purpose of this file is to make is possible to output
 * an std::vector to the terminal via:
 *
 * std::cout << vector << "\n";
 *
 * The was maily required for debugging purposes.
 *
 * To make use os this option include this file via:
 *
 * #include "tools/terminalout.hpp"
 *
 * @author Jan Quakernack
 * @version 1.0
 */

#include <iostream>
#include <vector>


namespace igg {

// Declare first, as this may work recursively
template <typename T>
std::ostream& operator<<
  (std::ostream &ostream, const std::vector<T> &kVector);


template <typename T>
std::ostream& operator<<
  (std::ostream &ostream, const std::vector<T> &kVector)
{
  auto delimiter = "";
  ostream << "[";
  for (const auto kItem : kVector) {
    ostream << delimiter << kItem;
    delimiter = ", ";
  }
  ostream << "]";
  return ostream;
}

} // namespace igg

#endif // CPP_FINAL_PROJECT_TOOLS_TERMINALOUT_HPP_
