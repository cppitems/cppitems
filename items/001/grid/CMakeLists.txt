cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++ -I/usr/lib/llvm-10/include/c++/v1 -pthread -Werror -Wall")

set(CMAKE_CXX_FLAGS_ASAN "-fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g" CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS_ASAN "-fsanitize=address" CACHE STRING "")
set(CMAKE_CXX_FLAGS_MSAN "-fsanitize=memory -fsanitize-memory-track-origins -fno-omit-frame-pointer -g" CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS_MSAN "-fsanitize=memory" CACHE STRING "")
set(CMAKE_CXX_FLAGS_TSAN "-fsanitize=thread -fno-omit-frame-pointer -g" )
set(CMAKE_EXE_LINKER_FLAGS_TSAN "-fsanitize=thread" )

project(001 LANGUAGES CXX)

add_library(gridlib SHARED grid.cpp grid.h)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE gridlib)

add_executable(main_asan main_asan.cpp)
target_link_libraries(main_asan PRIVATE gridlib)

add_executable(main_msan main_msan.cpp)
target_link_libraries(main_msan PRIVATE gridlib)

add_executable(main_tsan main_tsan.cpp)
target_link_libraries(main_tsan PRIVATE gridlib)
