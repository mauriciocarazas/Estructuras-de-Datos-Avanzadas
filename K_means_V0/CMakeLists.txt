cmake_minimum_required(VERSION 3.23)
project(K_means_V0)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD 23)
set(ENV{OPENCV_DIR} "C:\\tools\\opencv\\build")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(K_means_V0 main.cpp)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
target_link_libraries(K_means_V0 ${OpenCV_LIBS})
