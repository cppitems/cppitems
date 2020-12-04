project(TestC_decrement LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall")

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)