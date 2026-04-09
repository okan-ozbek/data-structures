cmake_minimum_required(VERSION 4.0)
project(vector)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(vector
    src/main.cpp
    include/Vector.h
)

#add_executable(ceasar-cypher
#    src/main.cpp
#    src/ceasar_cypher.cpp
#    include/ceasar_cypher.h
#)