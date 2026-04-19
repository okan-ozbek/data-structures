//
// Created by Okan Ozbek on 4/17/2026
//

#include <cassert>
#include <iostream>
#include <chrono>

#include "../../vector/include/Vector.h"
#include "../include/UniquePointer.h"

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
            std::cout << "Completion in: " << duration << "us, (" << duration * 0.001L << "ms)." << std::endl;
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

struct Vector2D {
    int x;
    int y;

    Vector2D() : x{0}, y{0} {}
    explicit Vector2D(const int scalar) : x{scalar}, y{scalar} {}
    explicit Vector2D(const int x, const int y) : x{x}, y{y} {}
};

void test_default_constructor() {
    dsa::UniquePointer<int> pointer{};

    assert_true(pointer.get() == nullptr, "Default constructed pointer should be nullptr");
}

void test_explicit_value_constructor() {
    const dsa::UniquePointer pointer(new Vector2D{10, 10});

    assert_true(pointer->x == 10, "Explicit constructor: x should be 10");
    assert_true(pointer->y == 10, "Explicit constructor: y should be 10");
}

void test_factory_constructor() {
    dsa::UniquePointer<Vector2D> pointer = dsa::make_unique<Vector2D>(10, 10);

    assert_true(pointer->x == 10, "Factory constructor: x should be 10");
    assert_true(pointer->y == 10, "Factory constructor: y should be 10");
}

void test_move_constructor() {
    dsa::UniquePointer<Vector2D> pointer_1 = dsa::make_unique<Vector2D>(10, 10);
    const Vector2D* temp_pointer = pointer_1.get();

    const dsa::UniquePointer pointer_2{std::move(pointer_1)};

    assert_true(pointer_2.get() == temp_pointer, "Move constructor: pointer_2 should own the original resource");
    assert_true(pointer_1.get() == nullptr, "Move constructor: pointer_1 should be nullptr after move");
}

void test_destructor() {
    bool deleted{false};

    {
        auto deleter = [&deleted](const Vector2D* pointer) {
            delete pointer;
            deleted = true;
        };

        dsa::UniquePointer pointer(new Vector2D(10, 10), deleter);

        assert_true(!deleted, "Destruction: should not be deleted yet");
    }

    assert_true(deleted, "Destruction: memory was not deallocated");
}

void test_move_assignment_operator() {
    dsa::UniquePointer<Vector2D> pointer_1 = dsa::make_unique<Vector2D>(10, 10);
    const Vector2D* temp_pointer = pointer_1.get();
    dsa::UniquePointer<Vector2D> pointer_2{};

    pointer_2 = std::move(pointer_1);

    assert_true(pointer_2.get() == temp_pointer, "Move assignment: pointer_2 should own the original resource");
    assert_true(pointer_1.get() == nullptr, "Move assignment: pointer_1 should be nullptr after move");
}

void test_arrow_operator() {
    int x{10};
    int y{15};
    const dsa::UniquePointer<Vector2D> pointer = dsa::make_unique<Vector2D>(x, y);

    assert_true(pointer->x == x, "Arrow operator: x does not match expected value");
    assert_true(pointer->y == y, "Arrow operator: y does not match expected value");
}

void test_dereference_operator() {
    int x{10};
    int y{15};
    const dsa::UniquePointer<Vector2D> pointer = dsa::make_unique<Vector2D>(x, y);

    assert_true((*pointer).x == x, "Dereference operator: x does not match expected value");
    assert_true((*pointer).y == y, "Dereference operator: y does not match expected value");
}

void test_boolean_operator() {
    if (dsa::UniquePointer<Vector2D> pointer = dsa::make_unique<Vector2D>(10, 10)) {
        assert_true(true, "Boolean operator: should not fail");
    } else {
        assert_true(false, "Boolean operator: valid pointer should evaluate to true");
    }
}

void test_release() {
    int n{10};
    dsa::UniquePointer<Vector2D> pointer = dsa::make_unique<Vector2D>(n);

    const Vector2D* stale = pointer.release();

    assert_true(stale->x == n, "Release: x should still be accessible after release");
    assert_true(stale->y == n, "Release: y should still be accessible after release");
    assert_true(!pointer, "Release: pointer should be nullptr after release");
}

void test_reset() {
    int n{10};
    bool deleted{false};
    auto deleter = [&deleted](const Vector2D* pointer) { delete pointer; deleted = true; };

    dsa::UniquePointer pointer_1(new Vector2D(n), deleter);

    pointer_1.reset(new Vector2D(n * 2));

    assert_true(deleted, "Reset: memory was not deallocated");
    assert_true(pointer_1->x == n * 2, "Reset: x should be n*2 after reset with new object");
    assert_true(pointer_1->y == n * 2, "Reset: y should be n*2 after reset with new object");

    deleted = false;
    pointer_1.reset(pointer_1.get());

    assert_true(!deleted, "Reset self: memory was not deallocated");
    assert_true(pointer_1->x == n * 2, "Reset self: x should survive self-reset");
    assert_true(pointer_1->y == n * 2, "Reset self: y should survive self-reset");
}

void test_get() {
    int n{10};
    const dsa::UniquePointer<Vector2D> pointer_1 = dsa::make_unique<Vector2D>(n);

    assert_true(pointer_1.get() != nullptr, "Get: pointer should not be nullptr");
    assert_true(pointer_1.get()->x == n, "Get: x does not match expected value");
    assert_true(pointer_1.get()->y == n, "Get: y does not match expected value");
}

void test_swap() {
    int n{10};
    dsa::UniquePointer<Vector2D> pointer_1 = dsa::make_unique<Vector2D>(n);
    dsa::UniquePointer<Vector2D> pointer_2 = dsa::make_unique<Vector2D>(n * 2);

    pointer_1.swap(pointer_2);

    assert_true(pointer_1->x == n * 2, "Swap: pointer_1 x should be n*2 after swap");
    assert_true(pointer_2->x == n, "Swap: pointer_2 x should be n after swap");
    assert_true(pointer_1->y == n * 2, "Swap: pointer_1 y should be n*2 after swap");
    assert_true(pointer_2->y == n, "Swap: pointer_2 y should be n after swap");
}

int main() {
    {
        Timer timer{};

        // Constructors
        test_default_constructor();
        test_explicit_value_constructor();
        test_factory_constructor();
        test_move_constructor();
        test_destructor();

        // Operators
        test_move_assignment_operator();
        test_arrow_operator();
        test_dereference_operator();
        test_boolean_operator();

        // Functions
        test_release();
        test_reset();
        test_get();
        test_swap();
    }

    std::cout << "Successful test(s): " << TEST_RAN - TEST_ERRORS << ", error(s): " << TEST_ERRORS << "\n";

    std::cout << "--- END OF BENCHMARK ---" << "\n";

    std::cout << "Sizeof std::unique_ptr: " << sizeof(std::unique_ptr<int>) << ", sizeof dsa::UniquePointer: " << sizeof(dsa::UniquePointer<int>) << "\n";

    return 0;
}