//
// Created by Okan Ozbek on 4/11/2026.
//

#include <assert.h>
#include <iostream>
#include <chrono>
#include "../include/Vector.h"

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

void test_default_constructor() {
    constexpr std::size_t zero = 0;

    const Vector<int> int_array{};

    assert_true(int_array.size(), zero, "Default constructor size is not equal to default capacity");
    assert_true(int_array.capacity(), TEST_DEFAULT_CAPACITY, "Default constructor capacity is not equal to default capacity");
}

void test_capacity_constructor() {
    constexpr std::size_t capacity = 100;

    const Vector<int> int_array{capacity};

    assert_true(int_array.capacity(), capacity, "Capacity constructor capacity is not equal to capacity");
}

void test_capacity_value_constructor() {
    constexpr std::size_t capacity = 100;
    constexpr int value = 5;

    const Vector int_array{capacity, value};

    assert_true(int_array.capacity(), capacity, "Capacity and value constructor capacity is not equal to capacity");

    for (std::size_t i{}; i < int_array.size(); ++i) {
        assert_true(int_array[i], value, "Capacity and value constructor value is not equal to value");
        if (int_array[i] != value) {
            // TODO dont we want to assert failed?
            break;
        }
    }
}

void test_copy_constructor() {
    const Vector<int> int_array{};
    const Vector int_array_copy{int_array};

    assert_true(*int_array.begin(), *int_array_copy.begin(), "Copy constructor begin is not equal to original begin");
    assert_true(int_array_copy.size(), int_array.size(), "Copy constructor size is not equal to original size");
    assert_true(int_array_copy.capacity(), int_array.capacity(), "Copy constructor capacity is not equal to original capacity");
}

void test_move_constructor() {
    Vector<int> int_array{};
    const Vector int_array_move{std::move(int_array)};

    constexpr std::size_t zero = 0;

    assert_true(int_array_move.size(), zero, "Move constructor size is not equal to 0");
    assert_true(int_array_move.capacity(), TEST_DEFAULT_CAPACITY, "Move constructor capacity is not equal to 0");
}

void test_assignment_operator_copy() {
    constexpr std::size_t capacity = 100;
    constexpr int value = 5;

    const Vector int_array{capacity, value};
    Vector<int> int_array_copy{};

    int_array_copy = int_array;

    assert_true(*int_array_copy.begin(), *int_array.begin(), "Assignment operator begin is not equal to original begin");
    assert_true(int_array_copy.size(), int_array.size(), "Assignment operator size is not equal to original size");
    assert_true(int_array_copy.capacity(), int_array.capacity(), "Assignment operator capacity is not equal to original capacity");
}

void test_assignment_operator_move() {
    constexpr std::size_t capacity = 100;
    constexpr int value = 5;

    Vector int_array{capacity, value};

    Vector<int> int_array_move{};
    int_array_move = std::move(int_array);

    constexpr std::size_t zero = 0;

    assert_true(int_array.size(), zero, "Assignment operator move size is not equal to 0");
    assert_true(int_array.capacity(), zero, "Assignment operator move capacity is not equal to 0");
    assert_true(int_array_move.size(), capacity, "Assignment operator move size is not equal to original size");
    assert_true(int_array_move.capacity(), capacity, "Assignment operator move capacity is not equal to original capacity");
}

void test_index_operator() {
    Vector<int> int_array{};

    int_array.push_back(5);

    assert_true(int_array[0], 5, "Index operator value is not equal to value");

    int_array[0] = 15;

    assert_true(int_array[0], 15, "Index operator value is not equal to value");
}

void test_index_operator_out_of_bounds() {
    Vector<int> int_array{};

    try {
        int_array[100] = 10;
    } catch (const std::out_of_range& e) {
        assert_true(true, true, "Index operator out of bounds does not throw correct error message");
    }
}

void test_iterators() {
    constexpr std::size_t capacity = 20;
    constexpr int value = 5;

    Vector int_array{capacity, value};

    for (int& x : int_array) {
        x = 10;
    }

    assert_true(int_array[0], 10, "Iterators value is not equal to value");

    for (Vector<int>::Iterator it = int_array.begin(); it != int_array.end(); ++it) {
        *it = 15;
    }

    assert_true(int_array[5], 15, "Iterators value is not equal to value");
}

void test_at() {
    Vector<int> int_array{};

    int_array.push_back(5);
    int_array.push_back(10);

    assert_true(int_array.at(0), 5, "At operator value is not equal to value");
    assert_true(int_array.at(1), 10, "At operator value is not equal to value");
}

void test_at_out_of_bounds() {
    Vector<int> int_array{};

    try {
        int_array.at(100);
    } catch (const std::out_of_range& e) {
        assert_true(true, true, "At operator out of bounds does not throw correct error message");
    }
}

void test_front() {
    Vector int_array{10, 1};

    int_array.front() = 5;

    assert_true(int_array.front(), 5, "Front operator value is not equal to value");
}

void test_back() {
    Vector int_array{10, 1};

    int_array.back() = 5;

    assert_true(int_array.back(), 5, "Back operator value is not equal to value");
}

void test_push_back() {
    Vector<int> int_array{};

    int_array.push_back(5);
    int_array.push_back(10);

    assert_true<int>(int_array[0], 5, "Push back operator value is not equal to value");
    assert_true<int>(int_array[1], 10, "Push back operator value is not equal to value");
}

void test_emplace_back() {
    struct Vector2D {
        int x;
        int y;

        Vector2D() : x{0}, y{0} {}
        explicit Vector2D(const int scalar) : x{scalar}, y{scalar} {}
        explicit Vector2D(const int x, const int y) : x{x}, y{y} {}
    };

    Vector<Vector2D> int_array{};

    int_array.emplace_back(5, 10);
    int_array.emplace_back(5);
    int_array.emplace_back();

    assert_true<int>(int_array[0].x, 5, "Emplace back operator value is not equal to value 1");
    assert_true<int>(int_array[0].y, 10, "Emplace back operator value is not equal to value 2");
    assert_true<int>(int_array[1].x, 5, "Emplace back operator value is not equal to value 3");
    assert_true<int>(int_array[1].y, 5, "Emplace back operator value is not equal to value 4 ");
    assert_true<int>(int_array[2].x, 0, "Emplace back operator value is not equal to value 5");
    assert_true<int>(int_array[2].y, 0, "Emplace back operator value is not equal to value 6");
}

void test_pop_back() {
    Vector<int> int_array{};

    int_array.push_back(5);
    int_array.push_back(10);

    int_array.pop_back();

    assert_true<int>(int_array.back(), 5, "Pop back operator value is not equal to value");
    assert_true<int>(int_array.size(), 1, "Pop back operator size is not equal to value");
}

void test_erase_single() {
    Vector<int> int_array{};

    int_array.push_back(5);
    int_array.push_back(10);
    int_array.push_back(15);
    int_array.push_back(20);

    int_array.erase(int_array.begin() + 1);

    assert_true(int_array[1], 15, "Erase operator value is not equal to value");
    assert_true<std::size_t>(int_array.size(), 3, "Erase operator size is not equal to value");
}

void test_erase_range() {
    Vector<int> int_array{};

    int_array.push_back(5);
    int_array.push_back(10); //
    int_array.push_back(15); //
    int_array.push_back(20); //
    int_array.push_back(25);
    int_array.push_back(30);

    int_array.erase(int_array.begin() + 1, int_array.begin() + 3);

    assert_true<int>(int_array[0], 5, "Erase operator value is not equal to value");
    assert_true<int>(int_array[1], 25, "Erase operator value is not equal to value");
    assert_true<int>(int_array[2], 30, "Erase operator value is not equal to value");
    assert_true<int>(int_array.size(), 3, "Erase operator size is not equal to value");

    int_array.clear();
    int_array.push_back(5);
    int_array.push_back(10);
    int_array.push_back(15);
    int_array.push_back(20);
    int_array.push_back(25);
    int_array.push_back(30);

    int_array.erase(int_array.begin() + 1, int_array.end());

    assert_true<int>(int_array[0], 5, "Erase operator value is not equal to value");
    assert_true<int>(int_array.size(), 1, "Erase operator size is not equal to value");

    int_array.clear();
    int_array.push_back(5);
    int_array.push_back(10);
    int_array.push_back(15);

    int_array.erase(int_array.begin(), int_array.end());

    assert_true<int>(int_array.size(), 0, "Erase operator size is not equal to value");
}

void test_clear() {
    Vector<int> int_array{};

    int_array.push_back(5);
    int_array.push_back(10);
    int_array.push_back(15);
    int_array.push_back(20);

    int_array.clear();

    assert_true<int>(int_array.size(), 0, "Clear operator size is not equal to value");
}

void test_resize() {
    Vector<int> int_array{};

    int_array.resize(30);
    assert_true<int>(int_array.capacity(), 30, "Resize operator size is not equal to value");

    int_array.resize(20);
    assert_true<int>(int_array.capacity(), 20, "Resize operator size is not equal to value");

    int_array.resize(30);
    assert_true<int>(int_array.capacity(), 30, "Resize operator size is not equal to value");
}

void test_reserve() {
    Vector<int> int_array{};

    int_array.reserve(30);
    assert_true<int>(int_array.capacity(), 30, "Reserve operator size is not equal to value 1");

    // Reason: Reserve cannot shrink its capacity.
    int_array.reserve(20);
    assert_true<int>(int_array.capacity(), 30, "Reserve operator size is not equal to value 2");

    int_array.reserve(40);
    assert_true<int>(int_array.capacity(), 40, "Reserve operator size is not equal to value 3");
}

void test_shrink_to_fit() {
    Vector<int> int_array{};

    int_array.push_back(5);
    int_array.push_back(10);

    assert_true<int>(int_array.capacity(), TEST_DEFAULT_CAPACITY, "Shrink to fit operator size is not equal to value");

    int_array.shrink_to_fit();

    assert_true<int>(int_array.capacity(), 2, "Shrink to fit operator size is not equal to value");
    assert_true<int>(int_array[0], 5, "Shrink to fit operator size is not equal to value");
    assert_true<int>(int_array[1], 10, "Shrink to fit operator size is not equal to value");
}

int main() {
    {
        Timer timer{};

        // Constructors
        test_default_constructor();
        test_capacity_constructor();
        test_capacity_value_constructor();
        test_copy_constructor();
        test_move_constructor();

        // Operators
        test_assignment_operator_copy();
        test_assignment_operator_move();
        test_index_operator();
        test_index_operator_out_of_bounds();

        // Iterators
        test_iterators();

        // Functions
        test_at();
        test_at_out_of_bounds();
        test_front();
        test_back();
        test_push_back();
        test_emplace_back();
        test_pop_back();
        test_erase_single();
        test_erase_range();

        // Allocation / Deallocation
        test_clear();
        test_resize();
        test_reserve();
        test_shrink_to_fit();
    }

    std::cout << "Successful test(s): " << TEST_RAN - TEST_ERRORS << ", error(s): " << TEST_ERRORS << "\n";

    std::cout << "--- END OF BENCHMARK ---" << "\n";

    std::cout << "Sizeof std::string: " << sizeof(std::vector<int>) << ", sizeof String: " << sizeof(Vector<int>) << "\n";

    return 0;
}

