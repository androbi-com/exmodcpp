cmake_minimum_required(VERSION 3.5)
project(hello VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)
add_executable(plot main.cpp)
# get sciplot include dirs
target_include_directories(plot PUBLIC $<BUILD_INTERFACE:${sciplot_SOURCE_DIR}>)