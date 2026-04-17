cmake_minimum_required(VERSION 4.0)
project(unique-pointer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(unique-pointer
    src/main.cpp
    include/UniquePointer.h
)