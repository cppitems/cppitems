cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall -fno-omit-frame-pointer -fsanitize=address" CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_DEBUGS} -fno-omit-frame-pointer -fsanitize=address" CACHE STRING "")
project(ex12 LANGUAGES CXX)
add_executable(benchmark_constructors benchmark_constructors.cpp)