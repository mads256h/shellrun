cmake_minimum_required(VERSION 3.10)

# Project name
project(shellrun VERSION 1.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_SOURCE_DIR}/config.h @ONLY)

add_executable(shellrun main.c)

if (UNIX)
  add_definitions(-DUNIX)
else()
  add_definitions(-DWINDOWS)
endif()

if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
  add_definitions(-DBITS32)
else()
  add_definitions(-DBITS64)
endif()
