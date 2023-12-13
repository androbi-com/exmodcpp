cmake_minimum_required(VERSION 3.5)
project(cuda_hello LANGUAGES CXX CUDA)
enable_language(CUDA)
add_executable(cuda_hello main.cu)
# arquitectures:
# - 75: T4 on G4DN instances
set_property(TARGET cuda_hello PROPERTY CUDA_ARCHITECTURES 75-real)