cmake_minimum_required(VERSION 3.5)
project(examples VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 20)
#add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
# -Werror
add_compile_options(-Wno-non-template-friend)
# activate cpm
include(cmake/CPM.cmake)
# include package for stdexec (see build/_deps/stdexec-src)
CPMAddPackage(
  NAME stdexec
  GITHUB_REPOSITORY NVIDIA/stdexec
  GIT_TAG main # This will always pull the latest code from the `main` branch. You may also use a specific release version or tag
  OPTIONS "STDEXEC_BUILD_EXAMPLES OFF" "STDEXEC_BUILD_TESTS OFF"
  #DOWNLOAD_ONLY YES
)

CPMAddPackage(
  NAME cmdline
  GIT_REPOSITORY https://github.com/tanakh/cmdline.git
  GIT_TAG master
  #VERSION 1.0.0
  DOWNLOAD_ONLY YES
)

if (cmdline_ADDED)
  # cmdline has no CMake support, so we create our own target
  FILE(GLOB cmdline_sources ${cmdline_SOURCE_DIR}/*.h)
  #list(REMOVE_ITEM lua_sources "${lua_SOURCE_DIR}/lua.c" "${lua_SOURCE_DIR}/luac.c")
  add_library(cmdline STATIC ${cmdline_sources})

  target_include_directories(cmdline
    PUBLIC
      $<BUILD_INTERFACE:${lua_SOURCE_DIR}>
  )
endif()

add_subdirectory(coro_task)
add_subdirectory(hello)
add_subdirectory(pimpl)
add_subdirectory(plot)
add_subdirectory(type_erasure)
add_subdirectory(stdexec_hello)
add_subdirectory(stdexec_partasks)
add_subdirectory(stdexec_mandel)