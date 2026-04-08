//
// Created by Dorza on 4/7/2026.
//

#ifndef TEMP_CPP_DYNAMICARRAY_H
#define TEMP_CPP_DYNAMICARRAY_H


#include <cstddef>
#include "DynamicArrayIterator.h"

template<typename TValueType>
/**
 * A resizable dynamic array implementation that provides capabilities for
 * storing and managing a collection of objects similar to std::vector, with
 * features like dynamic resizing and manual memory management.
 *
 * @tparam TValueType Type of elements stored in the array.
 */
class DynamicArray {
public:
    using ValueType = TValueType;
    using Iterator = DynamicArrayIterator<DynamicArray>;

    /**
     * Initialize a dynamic array with default capacity
     *
     * Time complexity: O(1) since it only involves allocating memory for the default capacity, without initializing any elements.
     */
    DynamicArray() :
        data_{static_cast<ValueType*>(::operator new(DEFAULT_CAPACITY * sizeof(ValueType)))},
        capacity_{DEFAULT_CAPACITY}
    {
    }

    /**
     * Initialize a dynamic array with a set capacity, inserting empty values of type ValueType
     *
     * Time complexity: O(n) due to having to default-construct each element.
     * @param capacity
     */
    explicit DynamicArray(const std::size_t capacity) :
        data_{static_cast<ValueType*>(::operator new(capacity * sizeof(ValueType)))},
        size_{capacity},
        capacity_{capacity}
    {
        for (std::size_t i{}; i < size_; ++i) {
            new (&data_[i]) ValueType{};
        }
    }

    /**
     * Initialize a dynamic array with a set capacity, and default value of type ValueType
     *
     * Time complexity: O(n) due to having to copy-construct each element with the provided value.
     * @param capacity
     * @param value
     */
    explicit DynamicArray(const std::size_t capacity, const ValueType& value) :
        data_{static_cast<ValueType*>(::operator new(capacity * sizeof(ValueType)))},
        size_{capacity},
        capacity_{capacity}
    {
        for (std::size_t i{}; i < size_; ++i) {
            new (&data_[i]) ValueType(value);
        }
    }

    /**
     * Copy other dynamic data over to the current dynamic array.
     *
     * Time complexity: O(n) due to having to copy-construct each element.
     * @param other
     */
    DynamicArray(const DynamicArray& other) :
        data_{static_cast<ValueType*>(::operator new(other.capacity_ * sizeof(ValueType)))},
        size_{other.size_},
        capacity_{other.capacity_}
    {
        for (std::size_t i{}; i < size_; ++i) {
            new (&data_[i]) ValueType(other.data_[i]);
        }
    }

    /**
     * Destruct the dynamic array by clearing (destructuring all objects) and deallocating
     * the memory used for the array
     *
     * Time complexity: O(n) due to the need to destruct each element in the array before deallocating memory.
     */
    ~DynamicArray() {
        clear();
        ::operator delete(data_);
    }

    /**
     * Copy over the other dynamic array using the (=) operator
     *
     * Time complexity: O(n) due to the need to copy each element from the other array.
     * @param other
     * @return DynamicArray&
     */
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

    /**
     * Move over the other dynamic array using the (=) operator,
     * which is more efficient than copying since it avoids unnecessary copying of elements
     * and instead transfers ownership of resources.
     *
     * Time complexity: O(1) since it only involves moving pointers and size/capacity values, without copying individual elements.
     * @param other
     * @return DynamicArray&
     */
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

    /**
     * Access the element at the specified index with bound checking.
     *
     * Time complexity: O(1)
     * @param index
     * @return ValueType&
     */
    const ValueType& operator[](const int index) const {
        if (is_out_of_bounds(index)) {
            throw std::out_of_range("Index out of range");
        }

        return data_[index];
    }

    /**
     * Access the element and allow for modification at the specified index with bound checking.
     *
     * Time complexity: O(1)
     * @param index
     * @return ValueType&
     */
    ValueType& operator[](const int index) {
        if (is_out_of_bounds(index)) {
            throw std::out_of_range("Index out of range");
        }

        return data_[index];
    }

    /**
     * Get the iterator pointing to the beginning of the array.
     *
     * Time complexity: O(1)
     * @return Iterator
     */
    Iterator begin() const {
        return Iterator{data_};
    }

    /**
     * Get the iterator pointing to the end + 1 of the array.
     *
     * Time complexity: O(1)
     * @return Iterator
     */
    Iterator end() const {
        return Iterator{data_ + size_};
    }

    /**
     * Access the element at the specified index with bound checking.
     *
     * Time complexity: O(1)
     * @param index
     * @return ValueType&
     */
    ValueType& at(int index) {
        if (is_out_of_bounds(index)) {
            throw std::out_of_range("Index out of range");
        }

        return data_[index];
    }

    /**
     * Access the first element of the array.
     *
     * Time complexity: O(1)
     * @return ValueType&
     */
    ValueType& front() {
        return data_[0];
    }

    /**
     * Access the last eleemnt of the array.
     *
     * Time complexity: O(1)
     * @return ValueType&
     */
    ValueType& back() {
        return data_[size_ - 1];
    }

    /**
     * Add a new element to the end of the array, resizing (reallocation of memory) if necessary.
     *
     * Time complexity: O(1) on average, but can be O(n) when resizing is required.
     * @param value
     */
    void push_back(const ValueType& value) {
        if (is_full()) {
            resize(capacity_ * 2);
        }

        new (&data_[size_]) ValueType(value);
        ++size_;
    }

    /**
     * A quick note on what the (&&) actually mean in this context:
     * && means that the function takes a rvalue reference, which allows us to move the value instead of copying it.
     *
     * Time complexity: O(1) on average, but can be O(n) when resizing is required.
     * @param value
     */
    void push_back(ValueType&& value) {
        if (is_full()) {
            resize(capacity_ * 2);
        }

        new (&data_[size_]) ValueType(std::move(value));
        ++size_;
    }

    /**
     * Add a new element to the end of the array, resizing (reallocation of memory) if necessary.
     * Constructs the element in-place using the provided arguments.
     *
     * Time complexity: O(1) on average, but can be O(n) when resizing is required.
     * @tparam Args
     * @param args
     */
    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (is_full()) {
            resize(capacity_ * 2);
        }

        new (&data_[size_]) ValueType(std::forward<Args>(args)...);
        ++size_;
    }

    /**
     * Remove the last element of the array.
     *
     * Time complexity: O(1)
     */
    [[maybe_unused]] void pop_back() {
        decrement_size();
        data_[size_].~ValueType();
    }

    /**
     * Remove the element at the specified position, shifting subsequent elements to fill the gap
     * .
     * Time complexity: O(n) in the worst case, where n is the number of elements after the erased element, due to the need to shift elements.
     * @param iterator
     * @return Iterator
     */
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

    /**
     * Remove the elements between first, and last, shifting subsequent elements to fill the gap.
     *
     * Time complexity: O(n) in the worst case, where n is the number of elements after the erased element, due to the need to shift elements.
     * @param first
     * @param last
     * @return
     */
    Iterator erase(const Iterator& first, const Iterator& last) {
        if (is_empty()) {
            return end();
        }

        if (valid_iterator_range(first, last)) {
            return end();
        }

        const std::size_t first_index{static_cast<std::size_t>(first.ptr_ - data_)};
        const std::size_t last_index{static_cast<std::size_t>(last.ptr_ - data_)};

        for (std::size_t i{first_index}; i < last_index; ++i) {
            new (&data_[i]) ValueType(std::move(data_[i + 1]));
            data_[i + 1].~ValueType();
        }

        size_ -= last_index - first_index;
        return Iterator{data_ + first_index};
    }

    /**
     * Destruct all elements in the array, and reset the size.
     *
     * Time complexity: O(n) due to the need to destruct each element in the array.
     */
    void clear() {
        for (std::size_t i{}; i < size_; ++i) {
            data_[i].~ValueType();
        }

        size_ = 0;
    }

    /**
     * Resize the array to the specified capacity, moving existing elements to the new memory location.
     *
     * Time complexity: O(n) due to the destructuring of all elements in the old array.
     * @param capacity
     */
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

    /**
     * Reserve capacity for at least the specified number of elements, moving existing elements to the new memory location if necessary.
     *
     * Time complexity: O(n) due to the destructuring of all elements in the old array.
     * @param capacity
     */
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

    /**
     * Shrink the array to fit the current size, moving existing elements to the new memory location.
     *
     * Time complexity: O(n) due to the destructuring of all elements in the old array.
     */
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

    /**
     * Return true if there are no elements in the array.
     *
     * @return bool
     */
    [[nodiscard]] constexpr bool is_empty() const {
        return size_ == 0;
    }

    /**
     * Return the amount of elements in the array.
     *
     * @return std::size_t
     */
    [[nodiscard]] constexpr std::size_t size() const {
        return size_;
    }

    /**
     * Return the capacity allocated in memory.
     *
     * @return std::size_t
     */
    [[nodiscard]] constexpr std::size_t capacity() const {
        return capacity_;
    }

private:
    constexpr static std::size_t DEFAULT_CAPACITY = 10;

    ValueType* data_{nullptr};
    std::size_t size_{};
    std::size_t capacity_{};

    /**
     * Helper function to decrement size_ to a minimum of 0.
     */
    void decrement_size() {
        --size_;

        if (size_ < 0) {
            size_ = 0;
        }
    }

    /**
     * Helper function to check if the index is out of bounds.
     *
     * @param index
     * @return bool
     */
    [[nodiscard]] constexpr bool is_out_of_bounds(const int index) const {
        return index < 0 || index >= size_;
    }

    /**
     * Helper function to check if the first iterator and last iterator is valid.
     *
     * @param first
     * @param last
     * @return bool
     */
    [[nodiscard]] constexpr bool valid_iterator_range(const Iterator& first, const Iterator& last) const {
        return (first > last || last > end() || first > end() || first == last);
    }

    /**
     * Helper function to check if the array is at full capacity.
     *
     * @return
     */
    [[nodiscard]] constexpr bool is_full() const {
        return size_ == capacity_;
    }
};


#endif //TEMP_CPP_DYNAMICARRAY_H