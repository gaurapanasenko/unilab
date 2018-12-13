cmake_minimum_required(VERSION 2.8)

project(main)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-extensions/)
find_package(PkgConfig)
pkg_check_modules(GL gl glu)

link_directories(${GL_LIBRARY_DIRS})
include_directories(${GL_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} "main.cpp" "classes.cpp")
target_link_libraries(${PROJECT_NAME} ${GL_LIBRARIES} glut)
