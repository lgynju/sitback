cmake_minimum_required(VERSION 2.8.12)
project(sitback)

# OpenCV is required here.
# the OpenCV_DIR could be read from system environment variable.
# either use set(OpenCV_DIR "C:/**/opencv//build//install").
find_package(OpenCV REQUIRED)

add_executable(sitback main.cpp)
target_link_libraries(sitback ${OpenCV_LIBS})