//
// Created by Dorza on 4/19/2026.
//

#include "../include/utils/Timer.h"
#include "../include/tests/TestSharedPointer.h"
#include "../include/tests/TestUniquePointer.h"
#include "../include/tests/TestString.h"
#include "../include/tests/TestVector.h"

int main() {
    std::cout << "--- START OF BENCHMARK ---" << "\n\n";

    // Test suite
    {
        Timer timer{};

        TestSharedPointer{};
        TestUniquePointer{};
        TestString{};
        TestVector{};
    }

    std::cout << "\nSuccessful test(s): " << TEST_RAN - TEST_ERRORS << ", error(s): " << TEST_ERRORS << "\n\n";

    std::cout << "--- END OF BENCHMARK ---" << "\n\n";

    std::cout << "Sizeof std::shared_ptr: " << sizeof(std::shared_ptr<int>) << ", sizeof dsa::SharedPointer: " << sizeof(dsa::SharedPointer<int>) << "\n";
    std::cout << "Sizeof std::unique_ptr: " << sizeof(std::unique_ptr<int>) << ", sizeof dsa::UniquePointer: " << sizeof(dsa::UniquePointer<int>) << "\n";
    std::cout << "Sizeof std::string: " << sizeof(std::string) << ", sizeof dsa::String: " << sizeof(dsa::String) << "\n";
    std::cout << "Sizeof std::vector: " << sizeof(std::vector<int>) << ", sizeof dsa::Vector: " << sizeof(dsa::Vector<int>) << "\n";

    return 0;
}