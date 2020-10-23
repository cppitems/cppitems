cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall" CACHE STRING "")
project(ex11 LANGUAGES CXX)
add_executable(benchmark_initializers benchmark_initializers.cpp)