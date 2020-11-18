cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall" CACHE STRING "")
project(ex2 LANGUAGES CXX)
add_executable(test_timer test_timer.cpp)