cmake_minimum_required(VERSION 3.5)
project(coro_tasksched VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 20)
add_compile_options("-std=c++20")
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "NVHPC")
    add_compile_options(-fcoroutines)
endif()
add_executable(coro_tasksched main.cpp)
# warnings from fmt
target_compile_options(coro_tasksched PRIVATE "--diag_suppress code_is_unreachable")

target_link_libraries(coro_tasksched PRIVATE fmt::fmt-header-only)
