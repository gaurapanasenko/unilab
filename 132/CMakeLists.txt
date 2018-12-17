cmake_minimum_required(VERSION 2.8)

project(superint)

#link_directories(${GL_LIBRARY_)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libsuperint ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(superint SHARED libsuperint/superint.cpp)

add_executable(main src/main.cpp src/tests.cpp)
target_link_libraries(main superint)

add_executable(run_tests src/run_tests.cpp src/tests.cpp)
target_link_libraries(run_tests superint)
