cmake_minimum_required(VERSION 3.0)

project(ex33 LANGUAGES CXX VERSION 1.0)

# set build options
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ferror-limit=1")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=1")
endif()

# BUILD LIB
add_subdirectory(librandompi)

# BUILD EXECUTABLE
add_executable(testPolymorphism polymorphism.cpp)
target_link_libraries(testPolymorphism librandompi)