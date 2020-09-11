cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER
    "clang++"
    CACHE STRING "")
set(CMAKE_EXPORT_COMPILE_COMMANDS
    true
    CACHE BOOL "")

project(001 LANGUAGES CXX)

add_compile_options("-g")
add_compile_options("-std=c++17")
add_compile_options("-Werror")
add_compile_options("-Wall")

add_compile_options("-fno-omit-frame-pointer")
add_compile_options("-fsanitize=address")
add_link_options("-fsanitize=address")

add_library(gridlib OBJECT grid.cpp grid.h)
add_executable(main main.cpp)
target_link_libraries(main PRIVATE gridlib )