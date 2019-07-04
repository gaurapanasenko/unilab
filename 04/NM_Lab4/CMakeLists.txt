cmake_minimum_required(VERSION 3.0) # setting this is required
project(542)                       # this sets the project name
set (CMAKE_CXX_STANDARD 11)

###############################################################################
## file globbing ##############################################################
###############################################################################

# These instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables
# `sources` and `data`.
file(GLOB_RECURSE sources      src/*.cpp src/*.h)
file(GLOB_RECURSE data resources/*)
# You can use set(sources src/main.cpp) etc if you don't want to
# use globbing to find files automatically.

###############################################################################
## target definitions #########################################################
###############################################################################

# The data is just added to the executable, because in some IDEs (QtCreator)
# files are invisible when they are not explicitly part of the project.
add_executable(${PROJECT_NAME} ${sources} ${data})

# Just for example add some compiler flags.
target_compile_options(${PROJECT_NAME} PUBLIC -Wall)

# This allows to include files relative to the root of the src directory with a <> pair
target_include_directories(${PROJECT_NAME} PUBLIC src/main)

# This copies all resource files in the build directory.
# We need this, because we want to work with paths relative to the executable.
file(COPY ${data} DESTINATION resources)

###############################################################################
## dependencies ###############################################################
###############################################################################

# GTKMM
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
#find_package(GTKMM REQUIRED)
include_directories(${GTKMM_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC
  ${GTKMM_LIBRARIES}
)

###############################################################################
## packaging ##################################################################
###############################################################################

# All install commands get the same destination. this allows us to use paths
# relative to the executable.
install(TARGETS ${PROJECT_NAME} DESTINATION share/${PROJECT_NAME})

# This is basically a repeat of the file copy instruction that copies the
# resources in the build directory, but here we tell cmake that we want it
# in the package.
install(DIRECTORY resources DESTINATION share/${PROJECT_NAME})

# Now comes everything we need, to create a package
# there are a lot more variables you can set, and some
# you need to set for some package types, but we want to
# be minimal here.
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION "1.0.0")

# We don't want to split our program up into several incomplete pieces.
set(CPACK_MONOLITHIC_INSTALL 1)

# This must be last
include(CPack)
