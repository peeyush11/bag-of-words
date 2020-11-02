add_subdirectory(/usr/src/gtest ${PROJECT_BINARY_DIR}/gtest)

# Include testing cmake package
include(CTest)

set(TEST_BINARY ${PROJECT_NAME}_test)

add_library(get_tests_data_path_lib STATIC get_tests_data_path.cpp)
target_link_libraries(get_tests_data_path_lib dataset_lib Boost::filesystem)

add_library(make_test_dataset_lib STATIC make_test_dataset.cpp)
target_link_libraries(make_test_dataset_lib dataset_lib get_tests_data_path_lib)

add_executable (${TEST_BINARY}
                test_dataset.cpp
                test_features.cpp
                test_binaryio.cpp
                test_clustering.cpp
                test_sampling.cpp
                test_linalg.cpp
                test_histogram.cpp
                test_web.cpp
                test_bag_of_words.cpp)

target_link_libraries (${TEST_BINARY}
                       dataset_lib
                       get_tests_data_path_lib
                       make_test_dataset_lib
                       features_lib
                       binaryio_lib
                       web_lib
                       bag_of_words_lib
                       ${OpenCV_LIBS}
                       Boost::filesystem
                       ${EIGEN3_LIBS}
                       gtest
                       gtest_main)

add_test(NAME ${TEST_BINARY} COMMAND ${TEST_BINARY})


# Build benchmark code only if google benchmark was found
if(benchmark_FOUND AND Threads_FOUND)
  message(STATUS "Will compile benchmark.")
  include_directories(${benchmark_INCLUDE_DIRS})
  set(BENCHMARK_BINARY ${PROJECT_NAME}_benchmark)
  add_executable (${BENCHMARK_BINARY}
                  benchmark_clustering.cpp)
  target_link_libraries (${BENCHMARK_BINARY}
                         benchmark
                         ${OpenCV_LIBS}
                         ${benchmark_LIBRARIES}
                         ${CMAKE_THREAD_LIBS_INIT}
                         ${EIGEN3_LIBS})
endif(benchmark_FOUND AND Threads_FOUND)
