add_subdirectory(dataset)
add_subdirectory(features)
add_subdirectory(binaryio)
add_subdirectory(clustering)
add_subdirectory(web)
add_subdirectory(tools)

add_library(bag_of_words_lib STATIC bag_of_words.cpp)
target_link_libraries(bag_of_words_lib dataset_lib features_lib binaryio_lib web_lib)

add_executable(extract_features extract_features.cpp)
target_link_libraries(extract_features bag_of_words_lib Boost::program_options)

add_executable(compute_cluster_centroids compute_cluster_centroids.cpp)
target_link_libraries(compute_cluster_centroids bag_of_words_lib Boost::program_options ${EIGEN3_LIBS})

add_executable(make_histograms make_histograms.cpp)
target_link_libraries(make_histograms bag_of_words_lib Boost::program_options)

add_executable(make_web_output make_web_output.cpp)
target_link_libraries(make_web_output bag_of_words_lib Boost::program_options)

add_library(bag_of_words_vers_2_lib STATIC bag_of_words_vers_2.cpp)
target_link_libraries(bag_of_words_vers_2_lib dataset_lib features_lib binaryio_lib web_lib)

add_executable(create_dictionary_vers_2 create_dictionary_vers_2.cpp)
target_link_libraries(create_dictionary_vers_2 bag_of_words_vers_2_lib Boost::program_options ${EIGEN3_LIBS})

add_executable(search_image_vers_2 search_image_vers_2.cpp)
target_link_libraries(search_image_vers_2 bag_of_words_vers_2_lib)
