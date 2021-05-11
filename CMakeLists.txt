cmake_minimum_required(VERSION 3.10)
project(Matrix_multiplication)

set(CMAKE_CXX_STANDARD 14)

add_executable(Matrix_multiplication main.cpp)
target_link_libraries(Matrix_multiplication -pthread)