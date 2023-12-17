cmake_minimum_required(VERSION 3.5)
project(examples VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 20)
message("> Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
#add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
# -Werror
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-Wno-non-template-friend)
endif()
# activate cpm
include(cmake/CPM.cmake)

#add package for format
CPMAddPackage(
  NAME format
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG master
  OPTIONS "FMT_HEADER_ONLY ON"
)

# add package for stdexec (see build/_deps/stdexec-src)
CPMAddPackage(
  NAME stdexec
  GITHUB_REPOSITORY NVIDIA/stdexec
  GIT_TAG main # This will always pull the latest code from the `main` branch. You may also use a specific release version or tag
  OPTIONS "STDEXEC_BUILD_EXAMPLES OFF" "STDEXEC_BUILD_TESTS OFF"
)

# add sciplot package
CPMAddPackage(
  NAME sciplot
  GIT_REPOSITORY https://github.com/sciplot/sciplot
  GIT_TAG master
  OPTIONS "SCIPLOT_BUILD_EXAMPLES OFF" "SCIPLOT_BUILD_TESTS OFF" "SCIPLOT_BUILD_DOCS OFF"
)

#add package for cmdline, only download
CPMAddPackage(
  NAME cmdline
  GIT_REPOSITORY https://github.com/tanakh/cmdline.git
  GIT_TAG master
  DOWNLOAD_ONLY YES
)

add_subdirectory(hello)
add_subdirectory(pimpl)
add_subdirectory(plot)
add_subdirectory(type_erasure)
add_subdirectory(coro_task)
add_subdirectory(coro_tasksched)
add_subdirectory(cuda_hello)
add_subdirectory(cuda_mandel)
add_subdirectory(stdexec_hello)
add_subdirectory(stdexec_partasks)
add_subdirectory(stdexec_mandel)
add_subdirectory(threads_condvar)