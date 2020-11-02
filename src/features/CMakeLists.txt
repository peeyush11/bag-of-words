add_library(features_lib STATIC features.cpp)
target_link_libraries(features_lib ${OpenCV_LIBS})
