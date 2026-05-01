//
// Created by Dorza on 4/19/2026.
//

// #include <queue>
// #include <stack>
//
// #include "../include/utils/Timer.h"
// #include "../include/tests/TestSharedPointer.h"
// #include "../include/tests/TestUniquePointer.h"
// #include "../include/tests/TestString.h"
// #include "../include/tests/TestVector.h"
// #include "../include/tests/TestRingBufferQueue.h"
// #include "../include/tests/TestDeque.h"
// #include "../include/tests/TestQueue.h"
// #include "../include/tests/TestStack.h"

// int main() {
//     std::cout << "--- START OF BENCHMARK ---" << "\n\n";
//
//     // Test suite
//     {
//         Timer timer{};
//
//         TestSharedPointer{};
//         TestUniquePointer{};
//         TestString{};
//         TestVector{};
//         TestRingBufferQueue{};
//         TestDeque{};
//         TestQueue{};
//         TestStack{};
//     }
//
//     std::cout << "\nSuccessful test(s): " << TEST_RAN - TEST_ERRORS << ", error(s): " << TEST_ERRORS << "\n\n";
//
//     std::cout << "--- END OF BENCHMARK ---" << "\n\n";
//
//     std::cout << "Sizeof std::shared_ptr: " << sizeof(std::shared_ptr<int>) << ", sizeof dsa::SharedPointer: " << sizeof(dsa::SharedPointer<int>) << "\n";
//     std::cout << "Sizeof std::unique_ptr: " << sizeof(std::unique_ptr<int>) << ", sizeof dsa::UniquePointer: " << sizeof(dsa::UniquePointer<int>) << "\n";
//     std::cout << "Sizeof std::string: " << sizeof(std::string) << ", sizeof dsa::String: " << sizeof(dsa::String) << "\n";
//     std::cout << "Sizeof std::vector: " << sizeof(std::vector<int>) << ", sizeof dsa::Vector: " << sizeof(dsa::Vector<int>) << "\n";
//     std::cout << "Sizeof std::queue: " << sizeof(std::queue<int>) << ", sizeof dsa::Queue: " << sizeof(dsa::RingBufferQueue<int>) << "\n";
//     std::cout << "Sizeof std::deque: " << sizeof(std::deque<int>) << ", sizeof dsa::Deque: " << sizeof(dsa::Deque<int>) << "\n";
//     std::cout << "Sizeof std::queue: " << sizeof(std::queue<int>) << ", sizeof dsa::Queue: " << sizeof(dsa::Queue<int>) << "\n";
//     std::cout << "Sizeof std::stack: " << sizeof(std::stack<int>) << ", sizeof dsa::Stack: " << sizeof(dsa::Stack<int>) << "\n";
//
//     return 0;
// }

#include "gtest/gtest.h"

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
