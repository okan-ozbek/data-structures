//
// Created by Dorza on 4/7/2026.
//

#ifndef TEMP_CPP_DYNAMICARRAY_H
#define TEMP_CPP_DYNAMICARRAY_H


#include <cstddef>
#include "DynamicArrayIterator.h"

template<typename TValueType>
class DynamicArray {
public:
    using ValueType = TValueType;
    using Iterator = DynamicArrayIterator<DynamicArray>;

    DynamicArray() :
        data_{static_cast<ValueType*>(::operator new(DEFAULT_CAPACITY * sizeof(ValueType)))},
        capacity_{DEFAULT_CAPACITY}
    {
    }

    explicit DynamicArray(const std::size_t capacity) :
        data_{static_cast<ValueType*>(::operator new(capacity * sizeof(ValueType)))},
        size_{capacity},
        capacity_{capacity}
    {
        for (std::size_t i{}; i < size_; ++i) {
            new (&data_[i]) ValueType{};
        }
    }

    explicit DynamicArray(const std::size_t capacity, const ValueType& value) :
        data_{static_cast<ValueType*>(::operator new(capacity * sizeof(ValueType)))},
        size_{capacity},
        capacity_{capacity}
    {
        for (std::size_t i{}; i < size_; ++i) {
            new (&data_[i]) ValueType(value);
        }
    }

    DynamicArray(const DynamicArray& other) :
        data_{static_cast<ValueType*>(::operator new(other.capacity_ * sizeof(ValueType)))},
        size_{other.size_},
        capacity_{other.capacity_}
    {
        for (std::size_t i{}; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    ~DynamicArray() {
        clear();
        ::operator delete(data_);
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (&other == this) {
            return *this;
        }

        clear();
        ::operator delete(data_);

        data_ = static_cast<ValueType*>(::operator new(other.capacity_ * sizeof(ValueType)));
        capacity_ = other.capacity_;
        size_ = other.size_;

        for (std::size_t i{}; i < size_; ++i) {
            new (&data_[i]) ValueType(other.data_[i]);
        }

        return *this;
    }

    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (&other == this) {
            return *this;
        }

        clear();
        ::operator delete(data_);

        data_ = std::move(other.data_);
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }

    const ValueType& operator[](const int index) const {
        if (is_out_of_bounds(index)) {
            throw std::out_of_range("Index out of range");
        }

        return data_[index];
    }

    ValueType& operator[](const int index) {
        if (is_out_of_bounds(index)) {
            throw std::out_of_range("Index out of range");
        }

        return data_[index];
    }

    ValueType& at(int index) {
        if (is_out_of_bounds(index)) {
            throw std::out_of_range("Index out of range");
        }

        return data_[index];
    }

    ValueType& front() {
        return data_[0];
    }

    ValueType& back() {
        return data_[size_ - 1];
    }

    void push_back(const ValueType& value) {
        if (is_full()) {
            resize(capacity_ * 2);
        }

        new (&data_[size_]) ValueType(value);
        ++size_;
    }

    /**
     * A quick note on what the (&&) actually mean in this context:
     * && means that the function takes an rvalue reference, which allows us to move the value instead of copying it.
     * @param value
     */
    void push_back(ValueType&& value) {
        if (is_full()) {
            resize(capacity_ * 2);
        }

        new (&data_[size_]) ValueType(std::move(value));
        ++size_;
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (is_full()) {
            resize(capacity_ * 2);
        }

        new (&data_[size_]) ValueType(std::forward<Args>(args)...);
        ++size_;
    }

    [[maybe_unused]] void pop_back() {
        decrement_size();
        data_[size_].~ValueType();
    }

    Iterator erase(const Iterator& iterator) {
        if (is_empty()) {
            return end();
        }

        if (iterator == end()) {
            return end();
        }

        std::size_t index{static_cast<std::size_t>(iterator.ptr_ - data_)};

        iterator->~ValueType();

        for (std::size_t i{index}; i + 1 < size_; ++i) {
            new (&data_[i]) ValueType(std::move(data_[i + 1]));
            data_[i + 1].~ValueType();
        }

        --size_;
        return Iterator{data_ + index};
    }

    Iterator erase(const Iterator& first, const Iterator& last) {
        if (is_empty()) {
            return end();
        }

        if (valid_iterator_range(first, last)) {
            return end();
        }

        std::size_t first_index{static_cast<std::size_t>(first.ptr_ - data_)};
        std::size_t last_index{static_cast<std::size_t>(last.ptr_ - data_)};

        for (std::size_t i{first_index}; i < last_index; ++i) {
            new (&data_[i]) ValueType(std::move(data_[i + 1]));
            data_[i + 1].~ValueType();
        }

        size_ -= last_index - first_index;
        return Iterator{data_ + first_index};
    }

    void clear() {
        for (std::size_t i{}; i < size_; ++i) {
            data_[i].~ValueType();
        }

        size_ = 0;
    }

    void resize(const std::size_t capacity) {
        const std::size_t movable_capacity = (capacity < size_)
            ? capacity
            : size_;

        auto* data = static_cast<ValueType*>(::operator new(capacity * sizeof(ValueType)));

        for (std::size_t i{}; i < movable_capacity; ++i) {
            new (&data[i]) ValueType(std::move(data_[i]));
            data_[i].~ValueType();
        }

        for (std::size_t i{movable_capacity}; i < capacity_; ++i) {
            data_[i].~ValueType();
        }

        ::operator delete(data_);

        data_ = data;
        capacity_ = capacity;
        size_ = movable_capacity;
    }

    void reserve(const std::size_t capacity) {
        if (capacity <= capacity_) {
            return;
        }

        auto* data = static_cast<ValueType*>(::operator new(capacity * sizeof(ValueType)));

        for (std::size_t i{}; i < capacity; ++i) {
            new (&data[i]) ValueType(std::move(data_[i]));
            data_[i].~ValueType();
        }

        ::operator delete(data_);

        data_ = data;
        capacity_ = capacity;
    }

    void shrink_to_fit() {
        if (is_empty()) {
            return;
        }

        if (is_full()) {
            return;
        }

        auto* data = static_cast<ValueType*>(::operator new(size_ * sizeof(ValueType)));

        for (std::size_t i{}; i < size_; ++i) {
            new (&data[i]) ValueType(std::move(data_[i]));
            data_[i].~ValueType();
        }

        for (std::size_t i{size_}; i < capacity_; ++i) {
            data_[i].~ValueType();
        }

        ::operator delete(data_);

        data_ = data;
        capacity_ = size_;
    }

    Iterator begin() const {
        return Iterator{data_};
    }

    Iterator end() const {
        return Iterator{data_ + size_};
    }

    [[nodiscard]] constexpr bool is_empty() const {
        return size_ == 0;
    }

    [[nodiscard]] constexpr bool is_full() const {
        return size_ == capacity_;
    }

    [[nodiscard]] constexpr std::size_t size() const {
        return size_;
    }

    [[nodiscard]] constexpr std::size_t capacity() const {
        return capacity_;
    }

private:
    constexpr static std::size_t DEFAULT_CAPACITY = 10;

    ValueType* data_{nullptr};
    std::size_t size_{};
    std::size_t capacity_{};

    void decrement_size() {
        --size_;

        if (size_ < 0) {
            size_ = 0;
        }
    }

    [[nodiscard]] constexpr bool is_out_of_bounds(const int index) const {
        return index < 0 || index >= size_;
    }

    [[nodiscard]] constexpr bool valid_iterator_range(const Iterator& first, const Iterator& last) const {
        return (first > last || last > end() || first > end() || first == last);
    }
};


#endif //TEMP_CPP_DYNAMICARRAY_H