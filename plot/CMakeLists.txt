cmake_minimum_required(VERSION 3.5)
project(hello VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)

# we directly include the sciplot header files in the project, but you can also use
# a system wide install of sciplot: https://sciplot.github.io/installation/
# find_package(sciplot)
# target_link_libraries(plot sciplot::sciplot)

add_executable(plot main.cpp)
