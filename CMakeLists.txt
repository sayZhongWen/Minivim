cmake_minimum_required(VERSION 3.16)
project(minivim)

set(CMAKE_CXX_STANDARD 14)


include_directories(.)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(minivim start.cpp normal_mode.cpp insert_mode.cpp command_mode.cpp)
target_link_libraries(minivim ${CURSES_LIBRARIES})