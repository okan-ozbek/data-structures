//
// Created by Okan Ozbek on 4/7/2026.
//

#ifndef TEMP_CPP_VECTOR_H
#define TEMP_CPP_VECTOR_H


#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include "VectorIterator.h"

namespace dsa {
    /**
    * A resizable dynamic array implementation that provides capabilities for
    * storing and managing a collection of objects similar to std::vector, with
    * features like dynamic resizing and manual memory management.
    *
    * @tparam TValue Type of elements stored in the array.
    */
    template<typename TValue>
    class Vector {
    public:
        using Iterator = VectorIterator<Vector>;
        using ValueType = TValue;

        /**
        * Initialize a dynamic array with default capacity
        *
        * Time complexity: O(1) since it only involves allocating memory for the default capacity, without initializing any elements.
        */
        Vector()
            : m_data{ static_cast<TValue*>(::operator new(DEFAULT_CAPACITY * sizeof(TValue))) }
            , m_size{ 0 }
            , m_capacity{ DEFAULT_CAPACITY }
        {
        }

        /**
        * Initialize a dynamic array with a set capacity, inserting empty values of type ValueType
        *
        * Time complexity: O(n) due to having to default-construct each element.
        * @param capacity
        */
        explicit Vector(const std::size_t capacity)
            : m_data{ static_cast<TValue*>(::operator new(capacity * sizeof(TValue))) }
            , m_size{ capacity }
            , m_capacity{ capacity }
        {
            for (std::size_t i{}; i < m_size; ++i) {
                new (&m_data[i]) TValue{};
            }
        }

        /**
        * Initialize a dynamic array with a set capacity, and default value of type ValueType
        *
        * Time complexity: O(n) due to having to copy-construct each element with the provided value.
        * @param capacity
        * @param value
        */
        explicit Vector(const std::size_t capacity, const TValue& value)
            : m_data{ static_cast<TValue*>(::operator new(capacity * sizeof(TValue))) }
            , m_size{ capacity }
            , m_capacity{ capacity }
        {
            for (std::size_t i{}; i < m_size; ++i) {
                new (&m_data[i]) TValue(value);
            }
        }

        /**
        * Copy other dynamic data over to the current dynamic array.
        *
        * Time complexity: O(n) due to having to copy-construct each element.
        * @param other
        */
        Vector(const Vector& other)
            : m_data{ static_cast<TValue*>(::operator new(other.m_capacity * sizeof(TValue))) }
            , m_size{ other.m_size }
            , m_capacity{ other.m_capacity }
        {
            for (std::size_t i{}; i < m_size; ++i) {
                new (&m_data[i]) TValue(other.m_data[i]);
            }
        }

        /**
        * Move other dynamic data over to the current dynamic array.
        *
        * Time complexity: O(n) due to having to move each element.
        * @param other
        */
        Vector(Vector&& other) noexcept
            : m_data{ other.m_data }
            , m_size{ other.m_size }
            , m_capacity{ other.m_capacity }
        {
            other.m_data = nullptr;
            other.m_size = 0;
        }

        /**
        * Destruct the dynamic array by clearing (destructuring all objects) and deallocating
        * the memory used for the array
        *
        * Time complexity: O(n) due to the need to destruct each element in the array before deallocating memory.
        */
        ~Vector() {
            Clear();
            ::operator delete(m_data);
        }

        /**
        * Copy over the other dynamic array using the (=) operator
        *
        * Time complexity: O(n) due to the need to copy each element from the other array.
        * @param other
        * @return Vector&
        */
        Vector& operator=(const Vector& other) {
            if (&other == this) {
                return *this;
            }

            Clear();
            ::operator delete(m_data);

            m_data = static_cast<TValue*>(::operator new(other.m_capacity * sizeof(TValue)));
            m_capacity = other.m_capacity;
            m_size = other.m_size;

            for (std::size_t i{}; i < m_size; ++i) {
                new (&m_data[i]) TValue(other.m_data[i]);
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
        * @return Vector&
        */
        Vector& operator=(Vector&& other) noexcept {
            if (&other == this) {
                return *this;
            }

            Clear();
            ::operator delete(m_data);

            m_data = std::move(other.m_data);
            m_size = other.m_size;
            m_capacity = other.m_capacity;

            other.m_data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;

            return *this;
        }

        /**
        * Access the element at the specified index with bound checking.
        *
        * Time complexity: O(1)
        * @param index
        * @return ValueType&
        */
        const TValue& operator[](const std::size_t index) const {
            if (IsOutOfBounds(index)) {
                throw std::out_of_range("Index out of range");
            }

            return m_data[index];
        }

        /**
        * Access the element and allow for modification at the specified index with bound checking.
        *
        * Time complexity: O(1)
        * @param index
        * @return ValueType&
        */
        TValue& operator[](const std::size_t index) {
            if (IsOutOfBounds(index)) {
                throw std::out_of_range("Index out of range");
            }

            return m_data[index];
        }

        /**
        * Get the iterator pointing to the beginning of the array.
        *
        * Time complexity: O(1)
        * @return Iterator
        */
        [[nodiscard]] Iterator begin() const {
            return Iterator{ m_data };
        }

        /**
        * Get the iterator pointing to the end + 1 of the array.
        *
        * Time complexity: O(1)
        * @return Iterator
        */
        [[nodiscard]] Iterator end() const {
            return Iterator{ m_data + m_size };
        }

        /**
        * Access the element at the specified index with bound checking.
        *
        * Time complexity: O(1)
        * @param index
        * @return ValueType&
        */
        TValue& At(const std::size_t index) {
            if (IsOutOfBounds(index)) {
                throw std::out_of_range("Index out of range");
            }

            return m_data[index];
        }

        /**
        * Access the first element of the array.
        *
        * Time complexity: O(1)
        * @return ValueType&
        */
        TValue& Front() {
            return m_data[0];
        }

        /**
        * Access the last eleemnt of the array.
        *
        * Time complexity: O(1)
        * @return ValueType&
        */
        TValue& Back() {
            return m_data[m_size - 1];
        }

        /**
        * Add a new element to the end of the array, resizing (reallocation of memory) if necessary.
        *
        * Time complexity: O(1) on average, but can be O(n) when resizing is required.
        * @param value
        */
        void PushBack(const TValue& value) {
            if (IsFull()) {
                Resize(m_capacity * 2);
            }

            new (&m_data[m_size]) TValue(value);
            ++m_size;
        }

        /**
        * A quick note on what the (&&) actually mean in this context:
        * && means that the function takes a rvalue reference, which allows us to move the value instead of copying it.
        *
        * Time complexity: O(1) on average, but can be O(n) when resizing is required.
        * @param value
        */
        void PushBack(TValue&& value) {
            if (IsFull()) {
                Resize(m_capacity * 2);
            }

            new (&m_data[m_size]) TValue(std::move(value));
            ++m_size;
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
        void EmplaceBack(Args&&... args) {
            if (IsFull()) {
                Resize(m_capacity * 2);
            }

            new (&m_data[m_size]) TValue(std::forward<Args>(args)...);
            ++m_size;
        }

        /**
        * Remove the last element of the array.
        *
        * Time complexity: O(1)
        */
        [[maybe_unused]] void PopBack() {
            DecrementSize();
            m_data[m_size].~TValue();
        }

        /**
        * Remove the element at the specified position, shifting subsequent elements to fill the gap
        * .
        * Time complexity: O(n) in the worst case, where n is the number of elements after the erased element, due to the need to shift elements.
        * @param iterator
        * @return Iterator
        */
        Iterator Erase(const Iterator& iterator) {
            if (IsEmpty()) {
                return end();
            }

            if (iterator == end()) {
                return end();
            }

            std::size_t index{static_cast<std::size_t>(iterator.pointer() - m_data)};
            iterator->~ValueType();

            for (std::size_t i{index}; i + 1 < m_size; ++i) {
                new (&m_data[i]) TValue(std::move(m_data[i + 1]));
                m_data[i + 1].~TValue();
            }

            --m_size;
            return Iterator{ m_data + index };
        }

        /**
        * Remove the elements between first, and last, shifting subsequent elements to fill the gap.
        *
        * Time complexity: O(n) in the worst case, where n is the number of elements after the erased element, due to the need to shift elements.
        * @param first
        * @param last
        * @return
        */
        Iterator Erase(const Iterator& first, const Iterator& last) {
            if (IsEmpty()) {
                return end();
            }

            if (IsInvalidIteratorRange(first, last)) {
                return end();
            }

            const std::size_t index{ static_cast<std::size_t>(first.pointer() - m_data) };
            std::size_t count{ static_cast<std::size_t>(last.pointer() - first.pointer()) };

            if (last != end()) {
                ++count;
            }

            for (std::size_t i{ index }; i + count < m_size; ++i) {
                new (&m_data[i]) TValue(std::move(m_data[i + count]));
                m_data[i + count].~TValue();
            }

            m_size -= count;
            return Iterator{ m_data + index };
        }

        /**
        * Destruct all elements in the array, and reset the size.
        *
        * Time complexity: O(n) due to the need to destruct each element in the array.
        */
        void Clear() {
            for (std::size_t i{}; i < m_size; ++i) {
                m_data[i].~TValue();
            }

            m_size = 0;
        }

        /**
        * Resize the array to the specified capacity, moving existing elements to the new memory location.
        *
        * Time complexity: O(n) due to the destructuring of all elements in the old array.
        * @param capacity
        */
        void Resize(const std::size_t capacity) {
            const std::size_t movableCapacity = (capacity < m_size)
                ? capacity
                : m_size;

            auto* data = static_cast<TValue*>(::operator new(capacity * sizeof(TValue)));

            for (std::size_t i{}; i < movableCapacity; ++i) {
                new (&data[i]) TValue(std::move(m_data[i]));

                if (i < m_size) {
                    m_data[i].~TValue();
                }
            }

            for (std::size_t i{ movableCapacity }; i < m_size; ++i) {
                m_data[i].~TValue();
            }

            ::operator delete(m_data);

            m_data = data;
            m_capacity = capacity;
            m_size = movableCapacity;
        }

        /**
        * Reserve capacity for at least the specified number of elements, moving existing elements to the new memory location if necessary.
        *
        * Time complexity: O(n) due to the destructuring of all elements in the old array.
        * @param capacity
        */
        void Reserve(const std::size_t capacity) {
            if (capacity <= m_capacity) {
                return;
            }

            auto* data = static_cast<TValue*>(::operator new(capacity * sizeof(TValue)));

            for (std::size_t i{}; i < m_size; ++i) {
                new (&data[i]) TValue(std::move(m_data[i]));
                m_data[i].~TValue();
            }

            ::operator delete(m_data);

            m_data = data;
            m_capacity = capacity;
        }

        /**
        * Shrink the array to fit the current size, moving existing elements to the new memory location.
        *
        * Time complexity: O(n) due to the destructuring of all elements in the old array.
        */
        void ShrinkToFit() {
            if (IsEmpty()) {
                return;
            }

            if (IsFull()) {
                return;
            }

            auto* data = static_cast<TValue*>(::operator new(m_size * sizeof(TValue)));

            for (std::size_t i{}; i < m_size; ++i) {
                new (&data[i]) TValue(std::move(m_data[i]));
                m_data[i].~TValue();
            }

            for (std::size_t i{ m_size }; i < m_capacity; ++i) {
                m_data[i].~TValue();
            }

            ::operator delete(m_data);

            m_data = data;
            m_capacity = m_size;
        }

        /**
        * Return true if there are no elements in the array.
        *
        * @return bool
        */
        [[nodiscard]] constexpr bool IsEmpty() const {
            return m_size == 0;
        }

        /**
        * Return the amount of elements in the array.
        *
        * @return std::size_t
        */
        [[nodiscard]] constexpr std::size_t Size() const {
            return m_size;
        }

        /**
        * Return the capacity allocated in memory.
        *
        * @return std::size_t
        */
        [[nodiscard]] constexpr std::size_t Capacity() const {
            return m_capacity;
        }

    private:
        constexpr static std::size_t DEFAULT_CAPACITY = 10;

        TValue* m_data{ nullptr };
        std::size_t m_size;
        std::size_t m_capacity;

        /**
        * Helper function to decrement size_ to a minimum of 0.
        */
        void DecrementSize() {
            if (m_size > 0) {
                --m_size;
            }
        }

        /**
        * Helper function to check if the index is out of bounds.
        *
        * @param index
        * @return bool
        */
        [[nodiscard]] constexpr bool IsOutOfBounds(const std::size_t index) const {
            return index >= m_size;
        }

        /**
        * Helper function to check if the first iterator and last iterator is valid.
        *
        * @param first
        * @param last
        * @return bool
        */
        [[nodiscard]] constexpr bool IsInvalidIteratorRange(const Iterator& first, const Iterator& last) const {
            return first > last || last > end() || first > end() || first == last;
        }

        /**
        * Helper function to check if the array is at full capacity.
        *
        * @return bool
        */
        [[nodiscard]] constexpr bool IsFull() const {
            return m_size == m_capacity;
        }
    };
}


#endif //TEMP_CPP_VECTOR_H