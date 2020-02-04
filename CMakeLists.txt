cmake_minimum_required(VERSION 3.10)

# Project name
project(shellrun VERSION 1.2)

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_SOURCE_DIR}/config.h @ONLY)

add_executable(shellrun main.c)

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
  # using clang
  set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fno-stack-protector -z execstack -no-pie")
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
  # using gcc
  set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fno-stack-protector -z execstack -no-pie")
elseif (CMAKE_C_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /GS-")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DYNAMICBASE:NO /NXCOMPAT:NO")
endif()
