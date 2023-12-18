cmake_minimum_required(VERSION 3.5)
project(coro_tasksched VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 20)
add_executable(coro_tasksched main.cpp)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "NVHPC")
    target_compile_options(coro_tasksched PRIVATE "-fcoroutines --diag_suppress code_is_unreachable")
endif()

target_link_libraries(coro_tasksched PRIVATE fmt::fmt-header-only)
