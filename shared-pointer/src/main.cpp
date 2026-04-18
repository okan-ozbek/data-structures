//
// Created by Okan Ozbek on 4/18/2026.
//

#include <assert.h>
#include <iostream>
#include <chrono>
#include "../include/SharedPointer.h"

constexpr std::size_t TEST_DEFAULT_CAPACITY = 10;
static int TEST_ERRORS = 0;
static int TEST_RAN = 0;

class Timer {
public:
    Timer() {
        start_time_ = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        stop();
    }

    void stop() const {
        const auto end_time_point = std::chrono::high_resolution_clock::now();

        const auto start_time = std::chrono::time_point_cast<std::chrono::microseconds>(start_time_).time_since_epoch().count();
        const auto end_time = std::chrono::time_point_cast<std::chrono::microseconds>(end_time_point).time_since_epoch().count();

        const auto duration = end_time - start_time;
        std::cout << "Completion in: " << duration << "us, (" << duration * 0.001 << "ms)." << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
};

template<typename T>
void assert_true(T a, T b, const std::string& error_message) {
    if (a != b) {
        std::cout << "Assertion failed: " << error_message << std::endl;
        ++TEST_ERRORS;
    }

    ++TEST_RAN;
}

void assert_true(const bool a, const std::string& error_message) {
    if (!a) {
        std::cout << "Assertion failed: " << error_message << std::endl;
        ++TEST_ERRORS;
    }

    ++TEST_RAN;
}

void test_default_constructor() {

}

void test_value_constructor() {

}

void test_copy_constructor() {

}

void test_destructor() {

}

void test_copy_assignment_operator() {

}

void test_reset() {

}

void test_swap() {

}

void test_get() {

}

void test_is_unique() {

}

void test_share_count() {

}

int main() {
    {
        Timer timer{};

        // Constructors
        test_default_constructor();
        test_value_constructor();
        test_copy_constructor();
        test_destructor();

        // Assignment operators
        test_copy_assignment_operator();

        // Functions
        test_reset();
        test_swap();
        test_get();
        test_is_unique();
        test_share_count();
    }

    std::cout << "Successful test(s): " << TEST_RAN - TEST_ERRORS << ", error(s): " << TEST_ERRORS << "\n";

    std::cout << "--- END OF BENCHMARK ---" << "\n";

    std::cout << "Sizeof std::string: " << sizeof(std::shared_ptr<int>) << ", sizeof dsa::String: " << sizeof(dsa::SharedPointer<int>) << "\n";
}