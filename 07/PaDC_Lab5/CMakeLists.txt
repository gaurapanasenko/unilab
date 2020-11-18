cmake_minimum_required(VERSION 3.5)

project(PaDC_Lab5 LANGUAGES C)

find_package(MPI REQUIRED)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(matvec matvec.c)
add_executable(vecmat vecmat.c)

target_link_libraries(matvec ${MPI_C_LIBRARIES} m)
target_link_libraries(vecmat ${MPI_C_LIBRARIES})
