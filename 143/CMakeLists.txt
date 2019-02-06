cmake_minimum_required(VERSION 2.8)

project(main)

add_definitions ("-Wall")

find_package(wxWidgets REQUIRED COMPONENTS net core base)

include(${wxWidgets_USE_FILE})

add_executable(${PROJECT_NAME} "libgaura.h" "wx_pch.h" "MyApp.cpp" "MyFrame.cpp" "Shapes.cpp" "ShapesChilds.cpp" "wxsmith/MyFrame.wxs")
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
