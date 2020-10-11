cmake_minimum_required(VERSION 3.5)

project(PaDC_Lab1 LANGUAGES C)

find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

add_executable(main_simple main_simple.c common.c common.h)
add_executable(main_openmp main_openmp.c common.c common.h)
add_executable(generator generator.c common.c common.h)

target_link_libraries(main_simple m)
target_link_libraries(main_openmp m)
