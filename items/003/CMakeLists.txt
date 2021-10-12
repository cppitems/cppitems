cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER g++) # sets default c++ compiler to g++

project(minimal LANGUAGES CXX)

add_executable(minimal minimal.cpp lib/square.cpp)
# this command above already takes care of dependencies