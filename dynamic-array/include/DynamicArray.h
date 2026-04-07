//
// Created by Dorza on 4/7/2026.
//

#ifndef TEMP_CPP_DYNAMICARRAY_H
#define TEMP_CPP_DYNAMICARRAY_H


#include <cstddef>

class DynamicArray {
public:
    DynamicArray();

    explicit DynamicArray(int capacity);
    explicit DynamicArray(int capacity, int value);
    explicit DynamicArray(const DynamicArray& other);

    ~DynamicArray();

    int& operator[](int index) const;

    void push_back(int value);
    [[maybe_unused]] int pop_back();

    void clear();
    [[nodiscard]] constexpr bool empty() const {
        return size_ == 0;
    }

    [[nodiscard]] constexpr std::size_t size() const {
        return size_;
    }

    [[nodiscard]] constexpr std::size_t capacity() const {
        return capacity_;
    }

private:
    constexpr static int DEFAULT_CAPACITY = 10;

    int* data_{nullptr};
    std::size_t size_{};
    std::size_t capacity_{};

    void resize(std::size_t capacity);
};


#endif //TEMP_CPP_DYNAMICARRAY_H