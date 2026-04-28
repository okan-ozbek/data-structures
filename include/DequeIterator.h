#ifndef DATA_STRUCTURES_DEQUEITERATOR_H
#define DATA_STRUCTURES_DEQUEITERATOR_H

namespace dsa {
    template <typename TDeque>
    class DequeIterator {
    public:
        using ValueType = TDeque::ValueType;

        DequeIterator(const TDeque* deque, const std::size_t logicalIndex)
            : m_deque(deque), m_index(logicalIndex) {}

        /**
         * Move the iterator to the next element in the deque, which is the element at the logical index + 1.
         *
         * @return DequeIterator&
         */
        DequeIterator& operator++() {
            ++m_index;
            return *this;
        }

        /**
         * Move the iterator to the previous element in the deque, which is the element at the logical index - 1.
         *
         * @return DequeIterator&
         */
        DequeIterator& operator--() {
            --m_index;
            return *this;
        }

        /**
         * (constant)
         * Increase the iterator by the specified offset, moving it forward in the deque.
         * The operation also causes the original iterator to be unchanged, and a new iterator to be returned.
         *
         * @param offset
         * @return DequeIterator
         */
        DequeIterator operator+(const int offset) const {
            return DequeIterator{ m_deque, m_index + offset };
        }

        /**
         * (constant)
         * Decrease the iterator by the specified offset, moving it forward in the deque.
         * The operation also causes the original iterator to be unchanged, and a new iterator to be returned.
         *
         * @param offset
         * @return DequeIterator
         */
        DequeIterator operator-(const int offset) const {
            return DequeIterator{ m_deque, m_index - offset };
        }

        /**
         * Return the value at the specified index, wrap around the ring-buffer if necessary.
         * The offset is added to the logical index of the iterator, and then converted to a
         * physical index in the underlying array.
         *
         * @param offset
         * @return ValueType&
         */
        ValueType& operator[](const int offset) const {
            std::size_t physical = (m_deque->m_front + m_index + offset) % m_deque->m_capacity;
            return m_deque->m_data[physical];
        }

        /**
         * Return the value at the current logical index, wrap around the ring-buffer if necessary.
         * The logical index of the iterator is converted to a physical index in the underlying array.
         *
         * @return ValueType&
         */
        ValueType& operator*() const {
            std::size_t physical = (m_deque->m_front + m_index) % m_deque->m_capacity;
            return m_deque->m_data[physical];
        }

        /**
         * Return the pointer to the value at the current logical index, wrap around the ring-buffer if necessary.
         * The logical index of the iterator is converted to a physical index in the underlying array.
         *
         * @return ValueType*
         */
        ValueType* operator->() const {
            std::size_t physical = (m_deque->m_front + m_index) % m_deque->m_capacity;
            return &m_deque->m_data[physical];
        }

        /**
         * Conditional operators
         * These operators are here to allow for comparison of iterators, and to allow for the use
         * of iterators in standard algorithms that require comparison operators.
         *
         * @param other
         * @return bool
         */
        bool operator==(const DequeIterator& other) const {
            return m_deque == other.m_deque && m_index == other.m_index;
        }

        bool operator!=(const DequeIterator& other) const {
            return !(*this == other);
        }

        bool operator<(const DequeIterator& other) const {
            return m_index < other.m_index;
        }

        bool operator>(const DequeIterator& other) const {
            return m_index > other.m_index;
        }

        bool operator<=(const DequeIterator& other) const {
            return m_index <= other.m_index;
        }

        bool operator>=(const DequeIterator& other) const {
            return m_index >= other.m_index;
        }

    private:
        const TDeque* m_deque;
        std::size_t m_index{};
    };
}

#endif //DATA_STRUCTURES_DEQUEITERATOR_H