cmake_minimum_required(VERSION 3.5)
project(coro_task VERSION 0.1.0)

add_executable(coro_task main.cpp)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "NVHPC")
    target_compile_options(coro_task PRIVATE "-fcoroutines")
endif()
