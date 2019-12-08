cmake_minimum_required(VERSION 3.10)

# Project name
project(ShellCodeRunner VERSION 0.1)

configure_file(config.h.in config.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

add_executable(ShellcodeRunner main.cpp)
