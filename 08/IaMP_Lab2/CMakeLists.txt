cmake_minimum_required(VERSION 3.5)

project(IaMP_Lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DE_LIBRARIES m)
set(DE_INCLUDE_DIRS
    imgui
    imgui/backends
    ImGui-Addons/FileBrowser
    stb
    )

# Math
find_library(M_LIBRARIES m REQUIRED)
list(APPEND DE_LIBRARIES ${M_LIBRARIES})

# OpenGL
find_package(OpenGL REQUIRED)
list(APPEND DE_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
list(APPEND DE_LIBRARIES ${OPENGL_LIBRARIES})

# GLFW
find_package(glfw3 REQUIRED)
find_library(glfw3_LIBRARY glfw REQUIRED)
list(APPEND DE_LIBRARIES ${glfw3_LIBRARY})

add_executable(${PROJECT_NAME}
    main.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl2.cpp
    ImGui-Addons/FileBrowser/ImGuiFileBrowser.cpp
    app.cpp app.h image.cpp image.h common.h
    )

target_include_directories(${PROJECT_NAME} PRIVATE ${DE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DE_LIBRARIES})
