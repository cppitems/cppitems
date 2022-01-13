cmake_minimum_required(VERSION 3.18)

project(Widget LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckCXXCompilerFlag)
set(ASAN_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
set(ASAN_FLAGS_LIST "-fsanitize=address;-fno-omit-frame-pointer")
set(CMAKE_REQUIRED_FLAGS ${ASAN_FLAGS})
check_cxx_compiler_flag(${ASAN_FLAGS} ASAN_WORKS)
unset(CMAKE_REQUIRED_FLAGS)

add_library(widget SHARED)
target_sources(widget PRIVATE include/widget.cpp include/template.hpp)
target_sources(widget INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/widget.hpp>
  $<INSTALL_INTERFACE:include/widget.hpp>)
set_target_properties(widget PROPERTIES PUBLIC_HEADER include/widget.hpp)    
target_include_directories(widget PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(widget PRIVATE cxx_std_17)
target_compile_features(widget PUBLIC cxx_std_14)
target_compile_definitions(widget PRIVATE "$<$<CONFIG:Debug>:DEBUGGING>")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(widget PUBLIC "$<$<BOOL:${ASAN_WORKS}>:${ASAN_FLAGS_LIST}>")
  target_link_libraries(widget PUBLIC "$<$<BOOL:${ASAN_WORKS}>:${ASAN_FLAGS_LIST}>")
endif()
add_library(widget::widget ALIAS widget)

add_executable(test)
target_sources(test PRIVATE test/main.cpp)
target_link_libraries(test PRIVATE widget::widget)

install(TARGETS widget
        EXPORT WidgetTargets
        PUBLIC_HEADER DESTINATION include)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/WidgetConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/WidgetConfig.cmake"
  INSTALL_DESTINATION lib/cmake)

install(EXPORT WidgetTargets
        FILE WidgetTargets.cmake
        NAMESPACE Widget::
        DESTINATION lib/cmake)   
install(FILES "${PROJECT_BINARY_DIR}/WidgetConfig.cmake"
        DESTINATION lib/cmake)