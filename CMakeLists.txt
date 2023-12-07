cmake_minimum_required(VERSION 3.5)
project(examples VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 20)
#add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
add_compile_options(-Wno-non-template-friend)
include(cmake/CPM.cmake)

CPMAddPackage(
  NAME stdexec
  GITHUB_REPOSITORY NVIDIA/stdexec
  GIT_TAG main # This will always pull the latest code from the `main` branch. You may also use a specific release version or tag
  OPTIONS "STDEXEC_BUILD_EXAMPLES OFF" "STDEXEC_BUILD_TESTS OFF"
)
 
#add_executable(main example.cpp)

#target_link_libraries(main STDEXEC::stdexec)

# -Werror
add_subdirectory(coro_task)
add_subdirectory(hello)
add_subdirectory(pimpl)
add_subdirectory(plot)
add_subdirectory(type_erasure)
add_subdirectory(stdexec_hello)
add_subdirectory(stdexec_partasks)
add_subdirectory(stdexec_mandel)