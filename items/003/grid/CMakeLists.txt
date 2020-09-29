cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER "clang++" CACHE STRING "")
set(CMAKE_EXPORT_COMPILE_COMMANDS true CACHE BOOL "")
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall" CACHE STRING "")
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")

project(001 LANGUAGES CXX)

add_custom_target(copy ALL DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json  ${CMAKE_SOURCE_DIR}/compile_commands.json )
add_executable(main main.cpp)
