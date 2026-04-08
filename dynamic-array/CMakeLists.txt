cmake_minimum_required(VERSION 4.0)
project(dynamic-array)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dynamic-array
    src/main.cpp
    include/DynamicArray.h
)

#add_executable(ceasar-cypher
#    src/main.cpp
#    src/ceasar_cypher.cpp
#    include/ceasar_cypher.h
#)