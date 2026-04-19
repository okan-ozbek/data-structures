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
    const dsa::SharedPointer<int> shared_pointer{};

    assert_true(shared_pointer.get() == nullptr, "Default constructed pointer should be nullptr");
    assert_true(shared_pointer.share_count() == 0, "Default constructed pointer should have share count of 0");
}

void test_value_constructor() {
    const dsa::SharedPointer shared_pointer{new int(10)};

    assert_true(*(shared_pointer.get()) == 10, "Value constructed pointer should dereference to 10");
    assert_true(shared_pointer.share_count() == 1, "Value constructed pointer should have share count of 1");
}

void test_copy_constructor() {
    const dsa::SharedPointer shared_pointer_1{new int(10)};
    const dsa::SharedPointer shared_pointer_2{shared_pointer_1};

    assert_true(*(shared_pointer_2.get()) == 10, "Copy constructed pointer should dereference to 10");
    assert_true(shared_pointer_2.share_count() == 2, "Copy constructed pointer should have share count of 2");
}

void test_destructor() {
    // TODO add deleter
}

void test_copy_assignment_operator() {
    const dsa::SharedPointer shared_pointer_1{new int(10)};

    {
        dsa::SharedPointer<int> shared_pointer_2{};

        shared_pointer_2 = shared_pointer_1;

        assert_true(*(shared_pointer_2.get()) == 10, "Copy assigned pointer should dereference to 10");
        assert_true(shared_pointer_2.share_count() == 2, "Copy assigned pointer should have share count of 2");
    }

    assert_true(shared_pointer_1.share_count() == 1, "Share count should be 1 after copy goes out of scope");
}

void test_reset() {
    /**
     * Reset with default parameter value
     */
    dsa::SharedPointer shared_pointer_1{new int(10)};

    shared_pointer_1.reset();

    assert_true(shared_pointer_1.get() == nullptr, "Reset pointer should be nullptr");
    assert_true(shared_pointer_1.share_count() == 0, "Reset pointer should have share count of 0");

    /**
     * Reset with value
     */
    dsa::SharedPointer shared_pointer_2{new int(10)};

    const auto raw_pointer = new int(20);
    shared_pointer_2.reset(raw_pointer);

    assert_true(*(shared_pointer_2.get()) == 20, "Pointer reset with new value should dereference to 20");
    assert_true(shared_pointer_2.share_count() == 1, "Pointer reset with new value should have share count of 1");
}

void test_swap() {
    dsa::SharedPointer shared_pointer_1{new int(10)};
    dsa::SharedPointer shared_pointer_1_cp{shared_pointer_1};
    dsa::SharedPointer shared_pointer_2{new int(30)};

    shared_pointer_1.swap(shared_pointer_2);

    assert_true(*(shared_pointer_1.get()) == 30, "Swapped pointer 1 should dereference to 30");
    assert_true(shared_pointer_1.share_count() == 1, "Swapped pointer 1 should have share count of 1");
    assert_true(*(shared_pointer_2.get()) == 10, "Swapped pointer 2 should dereference to 10");
    assert_true(shared_pointer_2.share_count() == 2, "Swapped pointer 2 should have share count of 2");
}

void test_get() {
    const dsa::SharedPointer shared_pointer{new int(10)};

    assert_true(*(shared_pointer.get()) == 10, "get() should return pointer to value 10");
}

void test_is_unique() {
    const dsa::SharedPointer shared_pointer_1{new int(10)};

    assert_true(shared_pointer_1.share_count() == 1, "Single owner should have share count of 1");

    dsa::SharedPointer shared_pointer_2{shared_pointer_1};

    assert_true(shared_pointer_1.share_count() == 2, "Two owners should have share count of 2");
}

void test_share_count() {
    const dsa::SharedPointer shared_pointer_1{new int(10)};

    dsa::SharedPointer shared_pointer_2{shared_pointer_1};
    dsa::SharedPointer shared_pointer_3{shared_pointer_1};
    
    assert_true(shared_pointer_1.share_count() == 3, "Three owners should have share count of 3");

    shared_pointer_3.reset();

    assert_true(shared_pointer_1.share_count() == 2, "Share count should be 2 after one owner resets");

    shared_pointer_2.reset();

    assert_true(shared_pointer_1.share_count() == 1, "Share count should be 1 after two owners reset");
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

    std::cout << "Sizeof std::shared_pointer: " << sizeof(std::shared_ptr<int>) << ", sizeof dsa::SharedPointer: " << sizeof(dsa::SharedPointer<int>) << "\n";
}