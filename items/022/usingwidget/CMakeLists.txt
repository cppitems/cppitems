cmake_minimum_required(VERSION 3.18)

project(UsingWidget CXX)

find_package(Widget CONFIG REQUIRED)
message(STATUS ${Widget_LIBRARIES})
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} Widget::widget)