cmake_minimum_required(VERSION 3.5)
project(executors VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 20)
add_executable(stdexec_mandel main.cpp)
get_target_property(target_options stdexec_mandel COMPILE_OPTIONS)
list(REMOVE_ITEM target_options "-g")
list(REMOVE_ITEM target_options "-Wall")
list(REMOVE_ITEM target_options "-Wextra")
list(REMOVE_ITEM target_options "-pedantic")
list(REMOVE_ITEM target_options "-Wshadow")
list(REMOVE_ITEM target_options "-Wnon-virtual-dtor")
#add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
# -Wextra -Wshadow -Wnon-virtual-dtor 
set_property(TARGET stdexec_mandel PROPERTY COMPILE_OPTIONS ${target_options})
# stdexec library 
target_link_libraries(stdexec_mandel STDEXEC::stdexec)
# cmdline, only add include
target_include_directories(stdexec_mandel PUBLIC $<BUILD_INTERFACE:${cmdline_SOURCE_DIR}>)