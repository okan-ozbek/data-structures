//
// Created by Dorza on 4/28/2026.
//

#ifndef DEQUE_H
#define DEQUE_H


#include <cstddef>
#include <stdexcept>
#include <utility>

#include "DequeIterator.h"


namespace dsa {
    /**
     * Deque is short for double-ended queue, where you can push from both the front and back,
     * as wel as pop from the front and back. In the standard library it is implemented as a dynamic array
     * of fixed-size arrays, but here we will implement it as a circular buffer, which allows us to achieve O(1)
     * time complexity for push and pop operations from both ends.
     *
     * The circular buffer implementation of the deque uses a fixed-size array to store the elements,
     * and two indices (front and back) to keep track of the positions of the first and last elements in the deque.
     * When an element is added to the front or back of the deque, the corresponding index is updated accordingly,
     * and when an element is removed from the front or back, the index is also updated.
     * This allows for efficient use of memory and fast access to both ends of the deque.
     *
     * Other standard library data structures that use std::deque<T> as a wrapper include:
     * - std::queue<T>
     * - std::stack<T>
     *
     * @tparam TValue
     */
    template<typename TValue>
    class Deque {
    public:
        using Iterator = DequeIterator<Deque>;
        using ValueType = TValue;

        /**
         * Default constructor
         */
        Deque()
            : m_capacity{ DEFAULT_CAPACITY }
            , m_data{ static_cast<TValue*>(::operator new(m_capacity * sizeof(TValue))) }
            , m_size{ 0 }
            , m_front{ 0 }
            , m_back{ 0 }
        {
        }

        /**
         * Copy constructor
         *
         * Time complexity: O(n) as it has to copy all elements from the other Deque&
         * @param other
         */
        Deque(const Deque& other)
            : m_capacity{ other.m_capacity }
            , m_data{ static_cast<TValue*>(::operator new(other.m_capacity * sizeof(TValue))) }
            , m_size{ other.m_size }
            , m_front{ other.m_front }
            , m_back{ other.m_back }
        {
            for (std::size_t i{}; i < other.m_size; ++i) {
                std::size_t index{ (other.m_front + i) % other.m_capacity };

                new (&m_data[i]) TValue(other.m_data[index]);
            }
        }

        /**
         * Move constructor
         *
         * Time complexity: O(1) as it simply moves the data from the moved object into this object
         * @param other
         */
        Deque(Deque&& other) noexcept
            : m_capacity{ other.m_capacity }
            , m_data{ other.m_data }
            , m_size{ other.m_size }
            , m_front{ other.m_front }
            , m_back{ other.m_back }
        {
            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_size = 0;
            other.m_front = 0;
            other.m_back = 0;
        }

        /**
         * Destructor
         */
        ~Deque() {
            Clear();
            ::operator delete(m_data);

            m_capacity = 0;
            m_size = 0;
            m_front = 0;
            m_back = 0;
        }

        /**
         * Copy assignment operator
         *
         * Time complexity: O(n) as it has to copy all the elements from the other Deque&
         * @param other
         * @return Deque&
         */
        Deque& operator=(const Deque& other) {
            if (&other == this) {
                return *this;
            }

            Clear();
            ::operator delete(m_data);

            m_data = static_cast<TValue*>(::operator new(other.m_capacity * sizeof(TValue)));
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            m_front = other.m_front;
            m_back = other.m_back;

            for (std::size_t i{}; i < other.m_size; ++i) {
                std::size_t index{ (other.m_front + i) % other.m_capacity };

                new (&m_data[i]) TValue(other.m_data[index]);
            }

            return *this;
        }

        /**
         * Move assignment operator
         *
         * Time complexity: O(n) since it needs to destructure all previous elements
         * @param other
         * @return Deque&
         */
        Deque& operator=(Deque&& other) noexcept {
            if (&other == this) {
                return *this;
            }

            Clear();
            ::operator delete(m_data);

            m_data = other.m_data;
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            m_front = other.m_front;
            m_back = other.m_back;

            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_size = 0;
            other.m_front = 0;
            other.m_back = 0;

            return *this;
        }

        /**
         * Index-of operator
         *
         * Time complexity: O(1)
         * @param index
         * @return TValue&
         */
        TValue& operator[](const std::size_t index) const {
            return m_data[
                (m_front + index) % m_capacity
            ];
        }

        /**
        * Get the iterator pointing to the beginning of the array.
        * The iterator looks at the logical index and not the physical index
        *
        * Time complexity: O(1)
        * @return Iterator
        */
        [[nodiscard]] Iterator begin() const {
            return Iterator{ this, 0 };
        }

        /**
        * Get the iterator pointing to the end + 1 of the array.
        * The iterator looks at the logical index and not the physical index
        *
        * Time complexity: O(1)
        * @return Iterator
        */
        [[nodiscard]] Iterator end() const {
            return Iterator{ this, m_size };
        }

        TValue& At(const std::size_t index) const {
            return m_data[
                (m_front + index) % m_capacity
            ];
        }

        /**
         * Get the first element in the deque
         *
         * Time complexity: O(1)
         * @return TValue&
         */
        TValue& Front() const {
            return m_data[m_front];
        }

        /**
         * Get the last element in the deque
         *
         * Time complexity: O(1)
         * @return TValue&
         */
        TValue& Back() const {
            return m_data[
                (m_back + m_capacity - 1) % m_capacity
            ];
        }

        /**
         * Get the number of elements in the deque
         *
         * Time complexity: O(1)
         * @return std::size_t
         */
        std::size_t Size() const {
            return m_size;
        }

        /**
         * Get the current max-capacity (resizable) of the deque
         *
         * Time complexity: O(1)
         * @return std::size_t
         */
        std::size_t Capacity() const {
            return m_capacity;
        }

        /**
         * Returns true if there are no elements in the deque, and false otherwise.
         *
         * @return bool
         */
        bool IsEmpty() const {
            return m_size == 0;
        }

        /**
         * Allocates new memory for the deque with the new capacity, and moves all the elements from the
         * old memory to the new memory. It also linearizes the current front and back index, resetting both
         * to 0 and size % newCapacity respectively. If the new capacity is the same as the current capacity,
         * it does nothing.
         *
         * Time complexity: O(n) since we need to iterate through all previous elements in the deque and move
         * them over to the new memory.
         * @param newCapacity
         */
        void Resize(const std::size_t newCapacity) {
            if (newCapacity == m_capacity) {
                return;
            }

            auto* newData{ static_cast<TValue*>(::operator new(newCapacity * sizeof(TValue))) };

            for (std::size_t i{}; i < m_size; ++i) {
                std::size_t index{ (m_front + i) % m_capacity };

                new (&newData[i]) TValue(std::move(m_data[index]));
                m_data[index].~TValue();
            }

            ::operator delete(m_data);
            m_data = newData;

            m_capacity = newCapacity;
            m_front = 0;
            m_back = m_size % m_capacity;
        }

        /**
         * Ring buffer push back operation. If the deque is full,
         * it will resize the deque to double its current capacity.
         *
         * Time complexity: O(1) if the deque is not full, and O(n) if the deque is full since it needs to resize the deque.
         * @param item
         */
        void PushBack(const TValue& item) {
            if (IsFull()) {
                Resize(m_capacity * 2);
            }

            new (&m_data[m_back]) TValue(item);
            m_back = (m_back + 1) % m_capacity;
            ++m_size;
        }

        /**
         * Ring buffer push front operation. If the deque is full,
         * it will resize the deque to double its current capacity.
         *
         * Time complexity: O(1) if the deque is not full, and O(n) if the deque is full since it needs to resize the deque.
         * @param item
         */
        void PushFront(const TValue& item) {
            if (IsFull()) {
                Resize(m_capacity * 2);
            }

            m_front = (m_front + m_capacity - 1) % m_capacity;
            new (&m_data[m_front]) TValue(item);
            ++m_size;
        }

        /**
         * Ring buffer pop back operation. If the deque is empty, it will throw an out_of_range exception.
         * This operation moves the element out of the deque, and destructs at the index.
         * Moving the back index back by 1.
         *
         * Time complexity: O(1)
         * @return TValue
         */
        TValue PopBack() {
            if (IsEmpty()) {
                throw std::out_of_range("Deque is empty");
            }

            m_back = (m_back + m_capacity - 1) % m_capacity;
            std::size_t index{ m_back };
            --m_size;

            TValue value = std::move(m_data[index]);
            m_data[index].~TValue();

            return value;
        }

        /**
         * Ring buffer pop front operation. If the deque is empty, it will throw an out_of_range exception.
         * This operation moves the element out of the deque, and destructs at the index.
         * Moving the front index forward by 1.
         *
         * Time complexity: O(1)
         * @return TValue
         */
        TValue PopFront() {
            if (IsEmpty()) {
                throw std::out_of_range("Deque is empty");
            }

            std::size_t index{ m_front };
            m_front = (m_front + 1) % m_capacity;
            --m_size;

            TValue value = std::move(m_data[index]);
            m_data[index].~TValue();

            return value;
        }

        /**
         * Ring buffer push back emplace operation. If the deque is full,
         * it will resize the deque to double its current capacity.
         *
         * Time complexity: O(1) if the deque is not full, and O(n) if the deque is full since it needs to resize the deque.
         * @param item
         */
        template<typename... Args>
        void EmplaceFront(Args&&... args) {
            if (IsFull()) {
                Resize(m_capacity * 2);
            }

            m_front = (m_front + m_capacity - 1) % m_capacity;
            new (&m_data[m_front]) TValue(std::forward<Args>(args)...);
            ++m_size;
        }

        /**
         * Ring buffer push front emplace operation. If the deque is full,
         * it will resize the deque to double its current capacity.
         *
         * Time complexity: O(1) if the deque is not full, and O(n) if the deque is full since it needs to resize the deque.
         * @param item
         */
        template<typename... Args>
        void EmplaceBack(Args&&... args) {
            if (IsFull()) {
                Resize(m_capacity * 2);
            }

            new (&m_data[m_back]) TValue(std::forward<Args>(args)...);

            m_back = (m_back + 1) % m_capacity;
            ++m_size;
        }

        /**
         * Swaps the data of this deque with the other deque. This operation is noexcept since it only swaps
         * pointers and size/capacity variables, and does not perform any operations that can throw an exception.
         *
         * Time complexity: O(1) since it only swaps pointers and size/capacity variables,
         * and does not perform any operations that can throw an exception.
         * @param other
         */
        void Swap(Deque& other) noexcept {
            if (&other == this) {
                return;
            }

            std::swap(m_data, other.m_data);
            std::swap(m_capacity, other.m_capacity);
            std::swap(m_size, other.m_size);
            std::swap(m_front, other.m_front);
            std::swap(m_back, other.m_back);
        }

        /**
         * Destructs all the elements in the deque, and resets the size to 0. This operation does
         * not change the capacity of the deque, and does not deallocate any memory.
         *
         * Time complexity: O(n) since it has to destruct each element in the deque.
         */
        void Clear() {
            for (std::size_t i{}; i < m_size; ++i) {
                std::size_t index{ (m_front + i) % m_capacity };

                m_data[index].~TValue();
            }

            m_size = 0;
        }


    private:
        template<typename>
        friend class DequeIterator;

        constexpr static int DEFAULT_CAPACITY{ 10 };

        std::size_t m_capacity;
        TValue* m_data{ nullptr };
        std::size_t m_size;
        std::size_t m_front;
        std::size_t m_back;

        /**
         * Helper function to check if index is out of bounds
         *
         * @param index
         * @return bool
         */
        [[nodiscard]] constexpr bool IsOutOfBounds(const std::size_t& index) const {
            return index >= m_size;
        }

        /**
         * Helper function to check if our deque is full
         *
         * @return bool
         */
        [[nodiscard]] constexpr bool IsFull() const {
            return m_size == m_capacity;
        }
    };
}



#endif //DATA_STRUCTURES_DEQUE_H