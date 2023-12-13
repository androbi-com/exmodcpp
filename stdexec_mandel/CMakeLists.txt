cmake_minimum_required(VERSION 3.5)
project(stdexec_mandel VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 20)
add_executable(stdexec_mandel main.cpp)

# warnings from cmdline
target_compile_options(stdexec_mandel PRIVATE "--diag_suppress empty_throw_specification_not_cpp20")
# stdexec library 
target_link_libraries(stdexec_mandel STDEXEC::stdexec)
# cmdline, only add include
target_include_directories(stdexec_mandel PUBLIC $<BUILD_INTERFACE:${cmdline_SOURCE_DIR}>)