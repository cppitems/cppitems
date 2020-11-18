cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall -fmax-errors=1" CACHE STRING "")
project(ex2 LANGUAGES CXX)
add_executable(test_distribution test_distribution.cpp)