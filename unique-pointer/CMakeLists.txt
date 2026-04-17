cmake_minimum_required(VERSION 4.0)
project(string)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(string
    src/main.cpp
    include/UniquePointer.h
)