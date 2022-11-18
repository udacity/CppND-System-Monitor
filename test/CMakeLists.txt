project(test)

cmake_minimum_required(VERSION 2.6)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

include_directories("${CMAKE_SOURCE_DIR}/include")
file(GLOB TEST_SOURCES *.cpp)

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
set(PROJECT_TEST_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/test/)

add_executable(test ${TEST_SOURCES})
target_link_libraries(test linux_parser)