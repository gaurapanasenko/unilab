cmake_minimum_required(VERSION 3.5)

project(PaDC_Lab2 LANGUAGES C)

find_package(MPI REQUIRED)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(hello_world hello_world.c)

target_link_libraries(hello_world ${MPI_C_LIBRARIES})
