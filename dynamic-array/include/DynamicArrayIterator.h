//
// Created by Dorza on 4/7/2026.
//

#ifndef TEMP_CPP_DYNAMICARRAYITERATOR_H
#define TEMP_CPP_DYNAMICARRAYITERATOR_H


template<typename TDynamicArray>
class DynamicArrayIterator {
public:
    using ValueType = TDynamicArray::ValueType;

    explicit DynamicArrayIterator(ValueType* ptr) : ptr_{ptr} {}

    DynamicArrayIterator& operator++() {
        ++ptr_;
        return *this;
    }

    DynamicArrayIterator& operator--() {
        --ptr_;
        return *this;
    }

    DynamicArrayIterator operator+(const int offset) const {
        return DynamicArrayIterator{ptr_ + offset};
    }

    DynamicArrayIterator operator-(const int offset) const {
        return DynamicArrayIterator{ptr_ - offset};
    }

    ValueType& operator[](const int index) {
        return ptr_[index];
    }

    ValueType& operator[](const int index) const {
        return ptr_[index];
    }

    ValueType& operator*() {
        return *ptr_;
    }

    ValueType& operator*() const {
        return *ptr_;
    }

    ValueType* operator->() {
        return ptr_;
    }

    ValueType* operator->() const {
        return ptr_;
    }

    bool operator==(const DynamicArrayIterator& other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const DynamicArrayIterator& other) const {
        return ptr_ != other.ptr_;
    }

    bool operator<(const DynamicArrayIterator& other) const {
        return ptr_ < other.ptr_;
    }

    bool operator>(const DynamicArrayIterator& other) const {
        return ptr_ > other.ptr_;
    }

    bool operator<=(const DynamicArrayIterator& other) const {
        return ptr_ <= other.ptr_;
    }

    bool operator>=(const DynamicArrayIterator& other) const {
        return ptr_ >= other.ptr_;
    }

    ValueType* ptr_;
};

#endif //TEMP_CPP_DYNAMICARRAYITERATOR_H