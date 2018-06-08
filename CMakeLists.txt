cmake_minimum_required(VERSION 3.10)
include(../utils.cmake)

project(Tensor3)
# Enable OpenMP
add_openmp()
# Add install for the interface file.
add_install_header(include/interfaces/Tensor3)
# Add test executable
add_executable(test0 test/test.cpp)
target_link_libraries(test0 -flto)
# Add test
enable_testing()
add_test(NAME test COMMAND test0)
