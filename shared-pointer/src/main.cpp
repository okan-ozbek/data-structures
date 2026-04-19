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
    dsa::SharedPointer<int> shared_pointer{};

    assert_true(shared_pointer.get() == nullptr, "message");
    assert_true(shared_pointer.share_count() == 0, "message");
}

void test_value_constructor() {
    dsa::SharedPointer<int> shared_pointer{10};

    assert_true(shared_pointer.get() == 10, "message");
    assert_true(shared_pointer.share_count() == 1, "message");
}

void test_copy_constructor() {
    dsa::SharedPointer<int> shared_pointer_1{10};
    dsa::SharedPointer<int> shared_pointer_2{shared_pointer_1};

    assert_true(shared_pointer_2.get() == 10, "message");
    assert_true(shared_pointer_2.share_count() == 2, "message");
}

void test_destructor() {
    // TODO add deleter
}

void test_copy_assignment_operator() {
    dsa::SharedPointer<int> shared_pointer_1{10};

    {
        dsa::SharedPointer<int> shared_pointer_2{};

        shared_pointer_2 = shared_pointer_1;

        assert_true(shared_pointer_2.get() == 10, "message");
        assert_true(shared_pointer_2.share_count() == 2, "message");
    }

    assert_true(shared_pointer_1.share_count() == 1, "message");
}

void test_reset() {
    /**
     * Reset with default parameter value
     */
    dsa::SharedPointer<int> shared_pointer_1{10};

    shared_pointer.reset();

    assert_true(shared_pointer.get() == nullptr, "message");
    assert_true(shared_pointer.share_count() == 0, "message");

    /**
     * Reset with value
     */
    dsa::SharedPointer<int> shared_pointer_2{10};

    int* raw_pointer = new int(20);
    shared_pointer_2.reset(raw_pointer);

    assert_true(shared_pointer_2.get() == 20, "message");
    assert_true(shared_pointer_2.share_count() == 1, "message");

    delete raw_pointer;
}

void test_swap() {
    dsa::SharedPointer<int> shared_pointer_1{10};
    dsa::SharedPointer<int> shared_pointer_1_cp{shared_pointer_1};
    dsa::SharedPointer<int> shared_pointer_2{30};

    shared_pointer_1.swap(shared_pointer_2);

    assert_true(shared_pointer_1.get() == 30, "message");
    assert_true(shared_pointer_1.share_count() == 1, "message");
    assert_true(shared_pointer_2.get() == 10, "message");
    assert_true(shared_pointer_2.share_count() == 2, "message");
}

void test_get() {
    dsa::SharedPointer<int> shared_pointer{10};

    assert_true(shared_pointer.get() == 10);
}

void test_is_unique() {
    dsa::SharedPointer<int> shared_pointer_1{10};

    assert_true(shared_pointer_1.share_count() == 1, "message");

    dsa::SharedPointer<int> shared_pointer_2{share_pointer_1};

    assert_true(shared_pointer_1.share_count() == 2, "message");
}

void test_share_count() {
    dsa::SharedPointer<int> shared_pointer_1{10};
    dsa::SharedPointer<int> shared_pointer_2{shared_pointer_1};
    dsa::SharedPointer<int> shared_pointer_3{shared_pointer_1};
    
    assert_true(shared_pointer_1.share_count() == 3, "message");

    shared_pointer_3.reset();

    assert_true(shared_pointer_1.share_count() == 2, "message");

    shared_pointer_2.reset();

    assert_true(shared_pointer_1.share_count() == 1, "message");
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