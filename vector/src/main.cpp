//
// Created by Dorza on 4/7/2026.
//

#include <iostream>
#include <vector>

#include "../include/Vector.h"

struct Vector2 {
    int x{};
    int y{};

    Vector2() = default;
    explicit Vector2(const int scalar) : x{scalar}, y{scalar} {}
    explicit Vector2(const int x, const int y) : x{x}, y{y} {}

    Vector2(const Vector2&& other) noexcept : x{other.x}, y{other.y} {
        static int i{};

        ++i;

        // std::cout << "Moving vector: " << "[" << x << ", " << y << "]" << std::endl;
    }

    ~Vector2() {
        // std::cout << "Destroying vector" << std::endl;
    }

    Vector2& operator=(const Vector2&& other) noexcept {
        static int i{};

        ++i;

        // std::cout << "Assigning vector (move): " << i << "[" << x << ", " << y << "]" << std::endl;

        x = other.x;
        y = other.y;
        return *this;
    }
};

template<typename T>
void PrintArray(const Vector<T>& array) {
    std::cout << "[";

    for (std::size_t i{}; i < array.size(); ++i) {
        if (i + 1 == array.size()) {
            std::cout << array[i];
            break;
        }

        std::cout << array[i] << ", ";
    }

    std::cout << "]\n";
}

template<>
void PrintArray(const Vector<Vector2>& array) {
    std::cout << "[";

    for (std::size_t i{}; i < array.size(); ++i) {
        if (i + 1 == array.size()) {
            std::cout << "[" << array[i].x << ", " << array[i].y << "]";
            break;
        }

        std::cout << "[" << array[i].x << ", " << array[i].y << "], ";
    }

    std::cout << "]\n";
}

void TestIntArray() {
    std::cout << "Testing object array" << std::endl;

    Vector<int> array{};

    array.push_back(1);
    array.push_back(2);
    array.push_back(3);
    array.push_back(4);
    array.push_back(5);
    array.emplace_back(6);
    array.emplace_back(7);

    PrintArray(array);

    array.pop_back();
    array.pop_back();

    PrintArray(array);

    array.push_back(8);
    array.push_back(9);

    PrintArray(array);

    array.clear();

    PrintArray(array);
}

void TestBoolArray() {

}

void TestObjectArray() {
    std::cout << "Testing object array" << std::endl;

    Vector<Vector2> array{};

    // array.push_back(Vector2());
    // array.push_back(Vector2(3));
    // array.push_back(Vector2(5, 6));
    // array.push_back(Vector2(7, 8));
    // array.push_back(Vector2(9, 10));
    // array.emplace_back(11, 12);
    // array.emplace_back(13);
    // array.emplace_back(11, 12);
    // array.emplace_back(13);
    // array.emplace_back(11, 12);
    // array.emplace_back(13);
    //
    // PrintArray(array);
    //
    // array.pop_back();
    // array.pop_back();
    //
    // PrintArray(array);
    //
    // array.push_back(Vector2(66, 67));
    // array.push_back(Vector2(77, 78));
    //
    // PrintArray(array);
    //
    // array.clear();
    //
    // PrintArray(array);
    //


    // std::cout << "\nUsing iterators:\n";
    //
    // for (const auto& element : array) {
    //     std::cout << "[" << element.x << ", " << element.y << "]\n";
    // }
    //
    // std::cout << "----------------------\n";
    //
    // for (Vector<Vector2>::Iterator it = array.begin(); it != array.end(); ++it) {
    //     std::cout << "[" << it->x << ", " << it->y << "]\n";
    // }

    for (std::size_t i{}; i < 10; ++i) {
        array.emplace_back(i, i + 1);
    }

    array.erase(array.begin() + 2);

    PrintArray(array);

    std::cout << "----------------------\n";

    array.erase(array.begin() + 2, array.end());

    PrintArray(array);

    std::cout << "----------------------\n";

    array.erase(array.begin(), array.end());

    PrintArray(array);
}

int main() {
    TestObjectArray();
    //TestIntArray();

    return 0;
}
