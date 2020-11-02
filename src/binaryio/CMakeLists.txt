add_library(binaryio_lib STATIC binaryio.cpp)
target_link_libraries(binaryio_lib ${OpenCV_LIBS})
