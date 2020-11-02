add_library(dataset_lib STATIC dataset.cpp image_item.cpp)
target_link_libraries(dataset_lib binaryio_lib ${OpenCV_LIBS} Boost::filesystem)
