//
// Created by Okan Ozbek on 4/9/2026.
//

#include <iostream>
#include <chrono>
#include "../include/String.h"

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
void assert_true(T a, T b, const std::string error_message) {
    if (a != b) {
        std::cout << "Assertion failed: " << error_message << std::endl;
        ++TEST_ERRORS;
    }

    ++TEST_RAN;
}

// TODO add tests here

int main() {
    // String str{"Hello world!"};
    // String str2{"Hello world!"};

    String str = "Hello world!";
    std::cout << str << std::endl;

    const String str2 = "Hello world!";

    if (str == str2) {
        std::cout << "str and str2 are equal" << std::endl;
    } else {
        std::cout << "str and str2 are not equal" << std::endl;
    }

    std::cout << "str capacity: " << str.capacity() << std::endl;
    // String str3 = " WHasdhashdahsdhashdhashdhashdhashdhashdhashdahshd ";
    // str += str3;
    std::cout << str.capacity() << std::endl;

    for (char c : str) {
        std::cout << c << c;
    }

    std::cout << str.substr(3) << std::endl;

    // std::cout << "Is str == str2: " << (str == str2) << std::endl;

    // std::cout << "Length of Hello world!: " << String::strlen("Hello world!") << std::endl;

    return 0;
}
