//
// Created by Dorza on 4/19/2026.
//

#ifndef TESTVECTOR_H
#define TESTVECTOR_H

#include "../utils/TempAssert.h"
#include "../Vector.h"

class TestVector {
public:
    TestVector() {
        run();
    }

    ~TestVector() = default;

private:
    std::size_t TEST_DEFAULT_CAPACITY = 10;

    void test_default_constructor() {
        constexpr std::size_t zero = 0;

        const dsa::Vector<int> intArray{};

        assert_true(intArray.Size(), zero, "Default constructor size is not equal to default capacity");
        assert_true(intArray.Capacity(), TEST_DEFAULT_CAPACITY, "Default constructor capacity is not equal to default capacity");
    }

    void test_capacity_constructor() {
        constexpr std::size_t capacity = 100;

        const dsa::Vector<int> intArray{capacity};

        assert_true(intArray.Capacity(), capacity, "Capacity constructor capacity is not equal to capacity");
    }

    void test_capacity_value_constructor() {
        constexpr std::size_t capacity = 100;
        constexpr int value = 5;

        const dsa::Vector intArray{capacity, value};

        assert_true(intArray.Capacity(), capacity, "Capacity and value constructor capacity is not equal to capacity");

        for (std::size_t i{}; i < intArray.Size(); ++i) {
            assert_true(intArray[i], value, "Capacity and value constructor value is not equal to value");
            if (intArray[i] != value) {
                break;
            }
        }
    }

    void test_copy_constructor() {
        const dsa::Vector<int> intArray{3, 3};
        const dsa::Vector<int> intArrayCopy{intArray};

        assert_true(*intArray.begin(), *intArrayCopy.begin(), "Copy constructor begin is not equal to original begin");
        assert_true(intArrayCopy.Size(), intArray.Size(), "Copy constructor size is not equal to original size");
        assert_true(intArrayCopy.Capacity(), intArray.Capacity(), "Copy constructor capacity is not equal to original capacity");
    }

    void test_move_constructor() {
        dsa::Vector<int> intArray{};
        const dsa::Vector intArrayMove{std::move(intArray)};

        constexpr std::size_t zero = 0;

        assert_true(intArrayMove.Size(), zero, "Move constructor size is not equal to 0");
        assert_true(intArrayMove.Capacity(), TEST_DEFAULT_CAPACITY, "Move constructor capacity is not equal to 0");
    }

    void test_assignment_operator_copy() {
        constexpr std::size_t capacity = 100;
        constexpr int value = 5;

        const dsa::Vector intArray{capacity, value};
        dsa::Vector<int> intArrayCopy{};

        intArrayCopy = intArray;

        assert_true(*intArrayCopy.begin(), *intArray.begin(), "Assignment operator begin is not equal to original begin");
        assert_true(intArrayCopy.Size(), intArray.Size(), "Assignment operator size is not equal to original size");
        assert_true(intArrayCopy.Capacity(), intArray.Capacity(), "Assignment operator capacity is not equal to original capacity");
    }

    void test_assignment_operator_move() {
        constexpr std::size_t capacity = 100;
        constexpr int value = 5;

        dsa::Vector intArray{capacity, value};

        dsa::Vector<int> intArrayMove{};
        intArrayMove = std::move(intArray);

        constexpr std::size_t zero = 0;

        assert_true(intArray.Size(), zero, "Assignment operator move size is not equal to 0");
        assert_true(intArray.Capacity(), zero, "Assignment operator move capacity is not equal to 0");
        assert_true(intArrayMove.Size(), capacity, "Assignment operator move size is not equal to original size");
        assert_true(intArrayMove.Capacity(), capacity, "Assignment operator move capacity is not equal to original capacity");
    }

    void test_index_operator() {
        dsa::Vector<int> intArray{};

        intArray.PushBack(5);

        assert_true(intArray[0], 5, "Index operator value is not equal to value");

        intArray[0] = 15;

        assert_true(intArray[0], 15, "Index operator value is not equal to value");
    }

    void test_index_operator_out_of_bounds() {
        dsa::Vector<int> intArray{};

        try {
            intArray[100] = 10;
        } catch (const std::out_of_range& e) {
            assert_true(true, true, "Index operator out of bounds does not throw correct error message");
        }
    }

    void test_iterators() {
        constexpr std::size_t capacity = 20;
        constexpr int value = 5;

        dsa::Vector intArray{capacity, value};

        for (int& x : intArray) {
            x = 10;
        }

        assert_true(intArray[0], 10, "Iterators value is not equal to value");

        for (auto & it : intArray) {
            it = 15;
        }

        assert_true(intArray[5], 15, "Iterators value is not equal to value");
    }

    void test_at() {
        dsa::Vector<int> intArray{};

        intArray.PushBack(5);
        intArray.PushBack(10);

        assert_true(intArray.At(0), 5, "At operator value is not equal to value");
        assert_true(intArray.At(1), 10, "At operator value is not equal to value");
    }

    void test_at_out_of_bounds() {
        dsa::Vector<int> intArray{};

        try {
            intArray.At(100);
        } catch (const std::out_of_range& e) {
            assert_true(true, true, "At operator out of bounds does not throw correct error message");
        }
    }

    void test_front() {
        dsa::Vector intArray{10, 1};

        intArray.Front() = 5;

        assert_true(intArray.Front(), 5, "Front operator value is not equal to value");
    }

    void test_back() {
        dsa::Vector intArray{10, 1};

        intArray.Back() = 5;

        assert_true(intArray.Back(), 5, "Back operator value is not equal to value");
    }

    void test_push_back() {
        dsa::Vector<int> intArray{};

        intArray.PushBack(5);
        intArray.PushBack(10);

        assert_true<int>(intArray[0], 5, "Push back operator value is not equal to value");
        assert_true<int>(intArray[1], 10, "Push back operator value is not equal to value");
    }

    void test_emplace_back() {
        struct Vector2D {
            int x;
            int y;

            Vector2D() : x{0}, y{0} {}
            explicit Vector2D(const int scalar) : x{scalar}, y{scalar} {}
            explicit Vector2D(const int x, const int y) : x{x}, y{y} {}
        };

        dsa::Vector<Vector2D> intArray{};

        intArray.EmplaceBack(5, 10);
        intArray.EmplaceBack(5);
        intArray.EmplaceBack();

        assert_true<int>(intArray[0].x, 5, "Emplace back operator value is not equal to value 1");
        assert_true<int>(intArray[0].y, 10, "Emplace back operator value is not equal to value 2");
        assert_true<int>(intArray[1].x, 5, "Emplace back operator value is not equal to value 3");
        assert_true<int>(intArray[1].y, 5, "Emplace back operator value is not equal to value 4 ");
        assert_true<int>(intArray[2].x, 0, "Emplace back operator value is not equal to value 5");
        assert_true<int>(intArray[2].y, 0, "Emplace back operator value is not equal to value 6");
    }

    void test_pop_back() {
        dsa::Vector<int> intArray{};

        intArray.PushBack(5);
        intArray.PushBack(10);

        intArray.PopBack();

        assert_true<int>(intArray.Back(), 5, "Pop back operator value is not equal to value");
        assert_true<std::size_t>(intArray.Size(), 1, "Pop back operator size is not equal to value");
    }

    void test_erase_single() {
        dsa::Vector<int> intArray{};

        intArray.PushBack(5);
        intArray.PushBack(10);
        intArray.PushBack(15);
        intArray.PushBack(20);

        intArray.Erase(intArray.begin() + 1);

        assert_true(intArray[1], 15, "Erase operator value is not equal to value");
        assert_true<std::size_t>(intArray.Size(), 3, "Erase operator size is not equal to value");
    }

    void test_erase_range() {
        dsa::Vector<int> intArray{};

        intArray.PushBack(5);
        intArray.PushBack(10);
        intArray.PushBack(15);
        intArray.PushBack(20);
        intArray.PushBack(25);
        intArray.PushBack(30);

        intArray.Erase(intArray.begin() + 1, intArray.begin() + 3);

        assert_true<int>(intArray[0], 5, "Erase operator value is not equal to value");
        assert_true<int>(intArray[1], 25, "Erase operator value is not equal to value");
        assert_true<int>(intArray[2], 30, "Erase operator value is not equal to value");
        assert_true<std::size_t>(intArray.Size(), 3, "Erase operator size is not equal to value");

        intArray.Clear();
        intArray.PushBack(5);
        intArray.PushBack(10);
        intArray.PushBack(15);
        intArray.PushBack(20);
        intArray.PushBack(25);
        intArray.PushBack(30);

        intArray.Erase(intArray.begin() + 1, intArray.end());

        assert_true<int>(intArray[0], 5, "Erase operator value is not equal to value");
        assert_true<std::size_t>(intArray.Size(), 1, "Erase operator size is not equal to value");

        intArray.Clear();
        intArray.PushBack(5);
        intArray.PushBack(10);
        intArray.PushBack(15);

        intArray.Erase(intArray.begin(), intArray.end());

        assert_true<std::size_t>(intArray.Size(), 0, "Erase operator size is not equal to value");
    }

    void test_clear() {
        dsa::Vector<int> intArray{};

        intArray.PushBack(5);
        intArray.PushBack(10);
        intArray.PushBack(15);
        intArray.PushBack(20);

        intArray.Clear();

        assert_true<std::size_t>(intArray.Size(), 0, "Clear operator size is not equal to value");
    }

    void test_resize() {
        dsa::Vector<int> intArray{};

        intArray.Resize(30);
        assert_true<std::size_t>(intArray.Capacity(), 30, "Resize operator size is not equal to value");

        intArray.Resize(20);
        assert_true<std::size_t>(intArray.Capacity(), 20, "Resize operator size is not equal to value");

        intArray.Resize(30);
        assert_true<std::size_t>(intArray.Capacity(), 30, "Resize operator size is not equal to value");
    }

    void test_reserve() {
        dsa::Vector<int> intArray{};

        intArray.Reserve(30);
        assert_true<std::size_t>(intArray.Capacity(), 30, "Reserve operator size is not equal to value 1");

        // Reason: Reserve cannot shrink its capacity.
        intArray.Reserve(20);
        assert_true<std::size_t>(intArray.Capacity(), 30, "Reserve operator size is not equal to value 2");

        intArray.Reserve(40);
        assert_true<std::size_t>(intArray.Capacity(), 40, "Reserve operator size is not equal to value 3");
    }

    void test_shrink_to_fit() {
        dsa::Vector<int> intArray{};

        intArray.PushBack(5);
        intArray.PushBack(10);

        assert_true<std::size_t>(intArray.Capacity(), TEST_DEFAULT_CAPACITY, "Shrink to fit operator size is not equal to value");

        intArray.ShrinkToFit();

        assert_true<std::size_t>(intArray.Capacity(), 2, "Shrink to fit operator size is not equal to value");
        assert_true<int>(intArray[0], 5, "Shrink to fit operator size is not equal to value");
        assert_true<int>(intArray[1], 10, "Shrink to fit operator size is not equal to value");
    }

    void run() {
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
};

#endif // TESTVECTOR_H

