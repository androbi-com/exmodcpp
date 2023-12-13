cmake_minimum_required(VERSION 3.5)
project(cuda_hello LANGUAGES CXX CUDA)
enable_language(CUDA)
#find_package(CUDA REQUIRED)
add_executable(cuda_hello main.cu)
# arquitectures:
# - 75: T4 on G4DN instances
set_property(TARGET cuda_hello PROPERTY CUDA_ARCHITECTURES 75-real)

get_target_property(target_options cuda_hello COMPILE_OPTIONS)
list(REMOVE_ITEM target_options "-Wnon-virtual-dtor")
set_property(TARGET cuda_hello PROPERTY COMPILE_OPTIONS ${target_options})
