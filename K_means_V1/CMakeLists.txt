cmake_minimum_required(VERSION 3.23)
project(K_means_V1)

set(CMAKE_CXX_STANDARD 23)

find_package(SFML 2.5 COMPONENTS system window graphics)

add_executable(K_means_V1 main.cpp KmeansCode.h)

target_link_libraries(K_means_V1 sfml-system sfml-window sfml-graphics)