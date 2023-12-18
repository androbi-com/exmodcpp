cmake_minimum_required(VERSION 3.5)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "NVHPC")
    project(cuda_mandel LANGUAGES CXX CUDA)
    #enable_language(CUDA)
    add_executable(cuda_mandel main.cu)
    # arquitectures:
    # - 75: T4 on G4DN instances
    set_property(TARGET cuda_mandel PROPERTY CUDA_ARCHITECTURES 75-real)
    target_include_directories(cuda_mandel PUBLIC $<BUILD_INTERFACE:${cmdline_SOURCE_DIR}>)
endif()
