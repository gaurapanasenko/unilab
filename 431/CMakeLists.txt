cmake_minimum_required(VERSION 3.7)

project(main)

# GLUT
IF (WIN32 OR WIN64)
  set(GLUT_ROOT_PATH "C:/Program Files/freeglut" CACHE PATH "Glut root path")
  set(FREEGLUT_STATIC ON CACHE BOOL "Use freeglut static library?")

  set(GLUT_DEFINITIONS "")

  if (WIN32)
    set(FREEGLUT_LIBRARY ${GLUT_ROOT_PATH}/lib)
  ELSE()
    set(FREEGLUT_LIBRARY ${GLUT_ROOT_PATH}/lib/x64)
  ENDIF()

  if (FREEGLUT_STATIC)
    set(GLUT_DEFINITIONS "-DFREEGLUT_STATIC")
    set(GLUT_win_LIBRARIES winmm gdi32)
    set(_GLUT_NAMES freeglut_static)
  ELSE()
    set(_GLUT_NAMES glut glut32 freeglut)
  ENDIF()

  unset(GLUT_glut_LIBRARY CACHE)

  find_path( GLUT_INCLUDE_DIR NAMES GL/glut.h
    PATHS  ${GLUT_ROOT_PATH}/include )
  find_library( GLUT_glut_LIBRARY NAMES ${_GLUT_NAMES}
    PATHS
    ${OPENGL_LIBRARY_DIR}
    ${GLUT_ROOT_PATH}/Release
    ${FREEGLUT_LIBRARY}
    )

  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLUT REQUIRED_VARS GLUT_glut_LIBRARY GLUT_INCLUDE_DIR)

  if(GLUT_FOUND)
    set( GLUT_LIBRARIES
      ${GLUT_glut_LIBRARY}
      ${GLUT_win_LIBRARIES}
      )
  endif()

  mark_as_advanced(
    GLUT_INCLUDE_DIR
    GLUT_glut_LIBRARY
    GLUT_winmm_LIBRARY
    GLUT_gdi32_LIBRARY
    GLUT_DEFINITIONS
    )

  unset(_GLUT_NAMES CACHE)

ELSE()
  find_package(GLUT REQUIRED)
ENDIF()

include_directories(${GLUT_INCLUDE_DIR})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
  message(ERROR "GLUT not found!")
endif(NOT GLUT_FOUND)


# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR "OPENGL not found!")
endif(NOT OPENGL_FOUND)


add_executable(${PROJECT_NAME} "main.cpp" "classes.cpp")
target_link_libraries(${PROJECT_NAME} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
