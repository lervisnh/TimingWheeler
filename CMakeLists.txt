cmake_minimum_required(VERSION 3.2)

project(timingWheeler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CXX_FLAGS
    -DCHECK_PTHREAD_RETURN_VALUE
    -D_FILE_OFFSET_BITS=64
    -Wall
    -Wextra
    -Werror
    # -Wconversion # remove for gtest release-1.12.1
    -Wno-unused-parameter
    -Wold-style-cast
    -Woverloaded-virtual
    -Wpointer-arith
    -Wshadow
    -Wwrite-strings
    -march=native
    -rdynamic
    -pthread
)
# if(CMAKE_BUILD_BITS EQUAL 32)
#   list(APPEND CXX_FLAGS "-m32")
# endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_DEBUG_POSTFIX debug)
    set(CMAKE_VERBOSE_MAKEFILE 1)
endif()

message(STATUS "Compile mode: ${CMAKE_BUILD_TYPE}")

include(cmake/utils.cmake)

option(BUILD_PY_LIB "Build python library with pybind11" OFF)

add_subdirectory(deps)
add_subdirectory(src)
