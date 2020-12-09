cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_COMPILER "clang++" CACHE STRING "")
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall -fsanitize=address -fno-omit-frame-pointer" CACHE STRING "")
set(CMAKE_EXPORT_COMPILE_COMMANDS true CACHE BOOL "")
project(ex32 LANGUAGES CXX)

option(BUILD_SMARTPTR_TESTS "Build unit tests to check the smart pointer functionality." ON)
if(BUILD_SMARTPTR_TESTS)
  add_subdirectory("SmartPtrTests")
endif()

#add_custom_target(copy ALL DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json  ${CMAKE_SOURCE_DIR}/compile_commands.json )
#add_executable(test_unique_ptr test_unique_ptr.cpp )