cmake_minimum_required(VERSION 3.8)
project(2_1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCE_FILES main.cpp Cell.h Cell.cpp Constraint.cpp Constraint.h)
add_executable(2_1 ${SOURCE_FILES})