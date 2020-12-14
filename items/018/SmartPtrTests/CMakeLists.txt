# Add all tests to cmake build
cmake_minimum_required(VERSION 3.0)

project(smartptr_tests)


MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      IF(NOT ${child} STREQUAL "build")
        LIST(APPEND dirlist ${child})
      ENDIF()
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

# Here you can also specify flags that should apply to all tests
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Werror -Wall")

# add all tests
SUBDIRLIST(TEST_DIRS ${CMAKE_CURRENT_LIST_DIR})

foreach(subdir ${TEST_DIRS})
  message("Parsing test folder:" ${subdir})
  add_subdirectory(${subdir})
endforeach()
