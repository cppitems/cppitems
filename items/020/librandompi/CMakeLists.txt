cmake_minimum_required(VERSION 3.0)
project(librandompi LANGUAGES CXX VERSION 1.0)

set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall")

# BUILD LIBRARY
add_library(librandompi SHARED RandomPi_library.cpp)
target_include_directories(librandompi PRIVATE .)

set_target_properties(librandompi PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(librandompi PROPERTIES PUBLIC_HEADER "RandomPi_library.hpp")