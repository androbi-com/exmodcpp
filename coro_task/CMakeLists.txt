cmake_minimum_required(VERSION 3.5)
project(coro_task VERSION 0.1.0)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # coroutine header without "experimental" since clang 14
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14)
        message(FATAL_ERROR "Clang version must be at least 14!")
    endif()
    add_compile_options("-std=c++20")
else() # coroutines require at least gcc 10
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10)
        message(FATAL_ERROR "gcc version must be at least 10!")
    endif()
    add_compile_options("-std=c++20")
endif()

add_executable(coro_task main.cpp)