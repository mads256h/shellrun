cmake_minimum_required(VERSION 3.10)

# Project name
project(ShellCodeRunner VERSION 0.3)

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_SOURCE_DIR}/config.h @ONLY)

add_executable(ShellcodeRunner main.c)
