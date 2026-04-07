//
// Created by Dorza on 4/7/2026.
//

#include "../include/DynamicArray.h"

DynamicArray::DynamicArray() {
    data_ = new int[DEFAULT_CAPACITY];

    capacity_ = DEFAULT_CAPACITY;
    size_ = 0;

}

DynamicArray::DynamicArray(const int capacity) {
    data_ = new int[capacity];

    capacity_ = capacity;
    size_ = capacity;

    for (int i{}; i < size_; ++i) {
        data_[i] = 0;
    }
}

DynamicArray::DynamicArray(const int capacity, const int value) {
    data_ = new int[capacity];

    capacity_ = capacity;
    size_ = capacity;

    for (int i{}; i < size_; ++i) {
        data_[i] = value;
    }
}

DynamicArray::DynamicArray(const DynamicArray& other) {
    data_ = new int[other.capacity_];

    capacity_ = other.capacity_;
    size_ = other.size_;

    for (int i{}; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

DynamicArray::~DynamicArray() {
    delete[] data_;

    data_ = nullptr;

    capacity_ = 0;
    size_ = 0;
}

int& DynamicArray::operator[](const int index) const {
    return data_[index];
}

void DynamicArray::push_back(const int value) {
    if (size_ == capacity_) {
        resize(capacity_ * 2);
    }

    data_[size_] = value;
    ++size_;

}

int DynamicArray::pop_back() {
    --size_;

    return data_[size_];
}

void DynamicArray::resize(const int capacity) {
    auto* data{new int[capacity]};

    for (int i{}; i < size_; ++i) {
        data[i] = data_[i];
    }

    delete[] data_;

    data_ = data;
    capacity_ = capacity;
}

void DynamicArray::clear() {
    delete[] data_;

    data_ = new int[DEFAULT_CAPACITY];

    capacity_ = DEFAULT_CAPACITY;
    size_ = 0;
}



