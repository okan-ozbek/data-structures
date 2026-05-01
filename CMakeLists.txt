cmake_minimum_required(VERSION 4.0)
project(data-structures)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(data-structures
    src/main.cpp
    tests/TestVector.cpp
    tests/TestString.cpp
    tests/TestUniquePointer.cpp
    tests/TestSharedPointer.cpp
    tests/TestDeque.cpp
    tests/TestQueue.cpp
    tests/TestStack.cpp
    tests/TestLinkedList.cpp
)

target_link_libraries(data-structures
    gtest_main
)

enable_testing()
add_test (NAME data-structures-test
    COMMAND data-structures
)
