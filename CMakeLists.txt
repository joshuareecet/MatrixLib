cmake_minimum_required(VERSION 4.2)
project(MatrixLib)

set(CMAKE_CXX_STANDARD 20)

add_executable(MatrixLib main.cpp
        matrix.hpp
        matrix.cpp)
target_compile_options(MatrixLib PRIVATE -Wall -Wextra -Wpedantic)
