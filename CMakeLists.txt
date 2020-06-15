# Root CMake file for BAYAN Project
#
# Copyright John Doe 2020
# Distributed under the OSI-approved BSD 3-Clause License. See accompanying
# file LICENSE.txt or https://opensource.org/licenses/BSD-3-Clause for details.

cmake_minimum_required(VERSION 3.8)

# Setting name for this project.
set(PROJECT_NAME bayan)

# Adding a path with "cmake" modules to the system search paths.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Setting up a project version.
include(version)
if (DEFINED ENV{BAYAN_BUILD_NUMBER})
  set(TIMUS_PATCH ENV{BAYAN_BUILD_NUMBER})
endif ()
set(BAYAN_VERSION ${BAYAN_MAJOR}.${BAYAN_MINOR}.${BAYAN_PATCH})
configure_file(version.hpp.in ${CMAKE_BINARY_DIR}/src/include/version.hpp @ONLY)

## Set property
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Werror -Wextra -Wpedantic -g -O0)
include_directories(${CMAKE_BINARY_DIR}/src/include)

# Setting up a project.
include(print)
include(function)

project(${PROJECT_NAME} CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
find_package(Boost COMPONENTS program_options filesystem regex thread REQUIRED)

# Setting options on project
option(CLANG_FORMAT "Enable clang-format target" ON)
option(DOCUMENTATION "Build documentation" ON)

if(CLANG_FORMAT)
  include(clang-format)
endif()

if(DOCUMENTATION)
  find_package(Doxygen COMPONENTS dot)
  include(doxygen)
endif()

# Added source directory
add_subdirectory(src)

