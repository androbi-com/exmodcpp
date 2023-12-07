cmake_minimum_required(VERSION 3.5)
project(pimpl VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)

add_executable(pimpl main.cpp)
target_sources(pimpl PRIVATE widget.cpp)
