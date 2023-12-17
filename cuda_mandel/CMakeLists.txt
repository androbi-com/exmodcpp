cmake_minimum_required(VERSION 3.5)
if (DEFINED CMAKE_CUDA_COMPILER)
    project(cuda_mandel LANGUAGES CXX CUDA)
    #enable_language(CUDA)
    add_executable(cuda_mandel main.cpp)
    # arquitectures:
    # - 75: T4 on G4DN instances
    set_property(TARGET cuda_mandel PROPERTY CUDA_ARCHITECTURES 75-real)
endif()