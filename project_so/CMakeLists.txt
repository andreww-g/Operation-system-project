cmake_minimum_required(VERSION 3.22)
project(Downloads/project_so)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
add_executable(project_so main.cpp)
target_link_libraries(project_so ${CURSES_LIBRARIES})
set(CMAKE_CXX_STANDARD 14)

