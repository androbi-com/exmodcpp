cmake_minimum_required(VERSION 3.5)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "NVHPC")
    project(cuda_hello LANGUAGES CXX CUDA)
    #enable_language(CUDA)
    add_executable(cuda_hello main.cu)
    # arquitectures:
    # - 75: T4 on G4DN instances
    set_property(TARGET cuda_hello PROPERTY CUDA_ARCHITECTURES 75-real)
endif()