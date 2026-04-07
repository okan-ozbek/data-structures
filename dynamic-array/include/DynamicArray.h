//
// Created by Dorza on 4/7/2026.
//

#ifndef TEMP_CPP_DYNAMICARRAY_H
#define TEMP_CPP_DYNAMICARRAY_H


class DynamicArray {
public:
    DynamicArray();
    DynamicArray(int capacity);
    DynamicArray(int capacity, int value);
    DynamicArray(const DynamicArray& other);

    ~DynamicArray();

    int& operator[](int index) const;

    void push_back(int value);
    [[maybe_unused]] int pop_back();

    void resize(int capacity);

    void clear();
    [[nodiscard]] constexpr bool empty() const {
        return size_ == 0;
    }

    [[nodiscard]] constexpr int size() const {
        return size_;
    }

    [[nodiscard]] constexpr int capacity() const {
        return capacity_;
    }

private:
    constexpr static int DEFAULT_CAPACITY = 10;

    int* data_{nullptr};
    int size_{};
    int capacity_{};
};


#endif //TEMP_CPP_DYNAMICARRAY_H