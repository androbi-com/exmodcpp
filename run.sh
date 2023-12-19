mkdir build
cd build
cmake [-D CMAKE_C_COMPILER=nvc -D CMAKE_CXX_COMPILER=nvc++] ..
cmake --build .