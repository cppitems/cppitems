cmake_minimum_required(VERSION 3.0)

project(ListIterator)

# set default build type as an option
# This makes it easier to set the correct build type in GUIs
SET(DEFAULT_BUILD_TYPE "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE
      STRING "Possible values: Debug, Release, MinSizeRel, RelWithDebInfo" FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# buy default tests are built, but this can be turned off
# command line: cmake -DBUILD_ITERATOR_TESTS=OFF ..
# or use ccmake (simple GUI for cmake): press 'c' to configure cmake,
# then 'g' to generate makefile, options can be changed by highlighting them and pressing ENTER
option(BUILD_ITERATOR_TESTS "Build unit tests to check the iterator functionality." ON)
if(BUILD_ITERATOR_TESTS)
  add_subdirectory("IteratorTests")
endif()

# When this option is turned to ON a bigger example using the iterator is built
# This would be the final executable in a real project
option(BUILD_FIBONACCI_EXE "Build an example for the final use of the list iterator." OFF)
if(BUILD_FIBONACCI_EXE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Werror -Wall")
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
  set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
  add_executable(fibancci_iterator fibonacci_iterator.cpp)
endif()