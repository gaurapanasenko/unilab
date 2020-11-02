cmake_minimum_required(VERSION 3.5)

project(PaDC_Lab3 LANGUAGES C)

find_package(MPI REQUIRED)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(main main.c)

target_link_libraries(main ${MPI_C_LIBRARIES})
