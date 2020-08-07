cmake_minimum_required(VERSION 3.10)

# Project name
project(shellrun VERSION 1.4)

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_SOURCE_DIR}/config.h @ONLY)

add_executable(shellrun main.c)
