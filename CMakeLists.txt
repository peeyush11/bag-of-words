#
# CMakeLists.txt
#
# @author Ignacio Vizzo (original version)
# @author Jan Quakernack (modifications)
#
# The original version is Copyright (c) 2019 Ignacio Vizzo, all rights reserved.
#
project(cpp_final_project)
cmake_minimum_required(VERSION 3.1)

# Set build type if not set
if(NOT CMAKE_BUILD_TYPE)
  #set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Set additional flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# Alternative to compile with debug symbols
#set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Create a results dir in project root where we will put the results of the build
set(RESULTS_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/results)
file(MAKE_DIRECTORY ${RESULTS_OUTPUT_DIRECTORY})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RESULTS_OUTPUT_DIRECTORY}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${RESULTS_OUTPUT_DIRECTORY}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RESULTS_OUTPUT_DIRECTORY}/bin)

# Attempt to find OpenCV3 on your system, for more details please read
# /usr/share/OpenCV/OpenCVConfig.cmake
find_package(OpenCV 3 REQUIRED core highgui imgproc xfeatures2d)
if(OpenCV_FOUND)
  message(STATUS "Found OpenCV version: ${OpenCV_VERSION}")
  message(STATUS "OpenCV directories: ${OpenCV_INCLUDE_DIRS}")
else()
  message(FATAL_ERROR "OpenCV not found, please refer to README.md.")
endif(OpenCV_FOUND)

set(OPENCV_ENABLE_NONFREE YES)

# Attempt to find Boost
find_package(Boost COMPONENTS filesystem program_options REQUIRED)
if(Boost_FOUND)
  message(STATUS "Found Boost version: ${Boost_VERSION}")
   #message(STATUS "Boost directories: ${Boost_INCLUDE_DIRS}")
else()
  message(FATAL_ERROR "Boost not found, please refer to README.md.")
endif(Boost_FOUND)

# Attempt to find google benchmark
find_package(benchmark)
if(benchmark_FOUND)
  message(STATUS "Found benchmark version: ${benchmark_VERSION}")
else()
  message(STATUS "Benchmark not found (benchmark code will not be build).")
endif(benchmark_FOUND)

# Attempt to find Threads (required for benchmark)
find_package (Threads)
if(Threads_FOUND)
  message(STATUS "Found Threads.")
else()
  message(STATUS "Threads not found (required for benchmark, which will not be build).")
endif(Threads_FOUND)

# Attempt to find Eigen3 (linear algebra)
find_package(Eigen3 REQUIRED)
if(Eigen3_FOUND)
  message(STATUS "Found Eigen3 version: ${Eigen3_VERSION}")
  #message(STATUS "Eigen3 directories: ${EIGEN3_INCLUDE_DIRS}")
else()
  message(FATAL_ERROR "Eigen3 not found, please refer to README.md.")
endif(Eigen3_FOUND)

# Add all files in src/ to the compiler's search paths
include_directories(${PROJECT_SOURCE_DIR}/src)

# After all are setup is done, we can go to our src/ directory to build our files
add_subdirectory("src")

# Build test files as well
enable_testing()
add_subdirectory("tests")
