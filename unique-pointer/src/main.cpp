//
// Created by Okan Ozbek on 4/17/2026
//

#include <assert.h>
#include <iostream>
#include <chrono>
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

        void stop() {
            const auto end_time_point = std::chrono::high_resolution_clock::now();

            const auto start_time = std::chrono::time_point_cast<std::chrono::microseconds>(start_time_).time_since_epoch().count();
            const auto end_time = std::chrono::time_point_cast<std::chrono::microseconds>(end_time_point).time_since_epoch().count();

            const auto duration = end_time - start_time;
            std::cout << "Completion in: " << duration << "us, (" << duration * 0.001 << "ms)." << std::endl;
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
}

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
}

void test_default_constructor() {
    UniquePointer<int> pointer{};

    assert_true(pointer.get() == nullptr, "message");
}

void test_explicit_value_constructor() {
    UniquePointer<Vector2D> pointer{new Vector2D(10, 10)};

    // Assert that the value is 10 10
}

void test_factory_construtor() {
    UniquePointer<Vector2D> pointer = make_unique<Vector2D>(10, 10);

    // Assert that the value is 10 10
}

void test_deleted_copy_constructor() {

}

void test_move_constructor() {
    UniquePointer<Vector2D> pointer_1 = make_unique<Vector2D>(10, 10);
    Vector2D* temp_pointer = pointer_1.get();

    UniquePointer<Vector2D> pointer_2{std::move(pointer_1)};

    assert_true(pointer_2 == temp_pointer, "message");
    assert_true(pointer_1 == nullptr, "message");
}

void test_destructor() {

}

void test_deleted_copy_assignment_operator() {

}

void test_move_assignment_operator() {
    UniquePointer<Vector2D> pointer_1 = make_unique<Vector2D>(10, 10);
    Vector2D* temp_pointer = pointer_1.get();

    UniquePointer<Vector2D> pointer_2{};

    pointer_2 = std::move(pointer_1);

    assert_true(pointer_2 == temp_pointer, "message");
    assert_true(pointer_1 == nullptr, "message");
}

void test_arrow_operator() {
    int x{10};
    int y{15};
    UniquePointer<Vector2D> pointer = make_unique<Vector2D>(x, y);

    assert_true(pointer->x == x, "message");
    assert_true(pointer->y == y, "message");

    const UniquePointer<Vector2D> const_pointer = make_unique<Vector2D>(x, y);

    assert_true(const_pointer->x == x, "message");
    assert_true(const_pointer->y == y, "message");
}

void test_dereference_operator() {
    int x{10};
    int y{15};
    UniquePointer<Vector2D> pointer = make_unique<Vector2D>(x, y);

    assert_true(*pointer.x == x, "message");
    assert_true(*pointer.y == y, "message");

    const UniquePointer<Vector2D> const_pointer = make_unique<Vector2D>(x, y);

    assert_true(*const_pointer.x == x, "message");
    assert_true(*const_pointer.y == y, "message");
}

void test_boolean_operator() {
    UniquePointer<Vector2D> pointer = make_unique<Vector2D>(10, 10);

    if (pointer) {
        assert_true(true, "message");
    } else {
        assert_true(false, "message");
    }
}

void test_release() {
    int n{10};
    UniquePointer<Vector2D> pointer = make_unique<Vector2D>(n);

    Vector2D* stale = pointer.release();

    assert_true(stale->x == n, "message");
    assert_true(stale->y == n, "message");
    assert_true(!pointer, "message");
}

void test_reset() {
    int n{10};
    UniquePointer<Vector2D> pointer_1 = make_unique<Vector2D>(n);
    Vector2D* pointer_2 = new Vector2D(n * 2);

    pointer_1.reset(pointer_2);

    assert_true(pointer_1->x == n * 2, "message");
    assert_true(pointer_1->y == n * 2, "message");
    
    pointer_1.reset(pointer_1.get());

    assert_true(pointer_1->x == n * 2, "message");
    assert_true(pointer_1->y == n * 2, "message");
}

void test_get() {
    int n{10};
    UniquePointer<Vector2D> pointer_1 = make_unique<Vector2D>(n);

    assert_true(pointer_1.get()->x == n, "message");
    assert_True(pointer_1.get()->y == n, "message");
}

void test_swap() {
    int n{10};
    UniquePointer<Vector2D> pointer_1 = make_unique<Vector2D>(n);
    UniquePointer<Vector2D> pointer_2 = make_unique<Vector2D>(n * 2);

    pointer_1.swap(pointer_2);

    assert_true(pointer_1->x == n * 2, "message");
    assert_true(pointer_2->x == n, "message");
    assert_true(pointer_1->y == n * 2, "message");
    assert_true(pointer_2->y == n, "message");
}

int main() {
    {
        Timer timer();

        // Constructors
        test_default_constructor();
        test_explicit_value_constructor();
        test_factory_construtor();
        test_deleted_copy_constructor();
        test_move_constructor();
        test_destructor();

        // Operators
        test_deleted_copy_assignment_operator();
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

    return 0;
}