add_library(web_lib STATIC web.cpp html_writer.cpp)
target_link_libraries(web_lib ${OpenCV_LIBS} Boost::filesystem)
