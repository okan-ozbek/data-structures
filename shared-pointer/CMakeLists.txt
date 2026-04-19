cmake_minimum_required(VERSION 4.0)
project(shared-pointer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(shared-pointer
        src/main.cpp
        include/SharedPointer.h
)

#add_executable(ceasar-cypher
#    src/main.cpp
#    src/ceasar_cypher.cpp
#    include/ceasar_cypher.h
#)