cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER "clang++" CACHE STRING "")
set(CMAKE_EXPORT_COMPILE_COMMANDS true CACHE BOOL "")
set(CMAKE_CXX_FLAGS "-std=c++17 -fopenmp -stdlib=libc++ -I/usr/lib/llvm-10/include/c++/v1 -pthread -Werror -Wall" CACHE STRING "")
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")

set(CMAKE_CXX_FLAGS_ASAN "-fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g" CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS_ASAN "-fsanitize=address" CACHE STRING "")
set(CMAKE_CXX_FLAGS_TSAN "-fsanitize=thread -fno-omit-frame-pointer -g" CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS_TSAN "-fsanitize=thread" CACHE STRING "")
set(CMAKE_CXX_FLAGS_MSAN "-fsanitize=memory -fsanitize-memory-track-origins -fno-omit-frame-pointer -g" CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS_MSAN "-fsanitize=memory" CACHE STRING "")

project(001 LANGUAGES CXX)

add_custom_target(copy ALL DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json  ${CMAKE_SOURCE_DIR}/compile_commands.json )

add_library(gridlib SHARED grid.cpp grid.h)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE gridlib)

add_executable(main_asan main_asan.cpp)
target_link_libraries(main_asan PRIVATE gridlib)

add_executable(main_msan main_msan.cpp)
target_link_libraries(main_msan PRIVATE gridlib)

add_executable(main_tsan main_tsan.cpp)
target_link_libraries(main_tsan PRIVATE gridlib)
target_link_directories(main_tsan BEFORE PRIVATE "/opt/llvm-project/install/lib/") # special TSAN version for OMP