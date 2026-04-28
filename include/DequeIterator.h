//
// Created by Dorza on 4/28/2026.
//

#ifndef DATA_STRUCTURES_DEQUEITERATOR_H
#define DATA_STRUCTURES_DEQUEITERATOR_H


namespace dsa {
    template <typename TDeque>
    class DequeIterator {
    public:
        using ValueType = TDeque::ValueType;

        /**
        * Initialize a deque iterator, pointing to the provided pointer.
        *
        * @param pointer
        */
        explicit DequeIterator(ValueType* pointer) : m_pointer{ pointer } {}

        /**
        * Move the iterator to the next element in the array.
        *
        * @return DequeIterator&
        */
        DequeIterator& operator++() {
            ++m_pointer;
            return *this;
        }

        /**
        * Move the iterator to the previous element in the array.
        *
        * @return DequeIterator&
        */
        DequeIterator& operator--() {
            --m_pointer;
            return *this;
        }

        /**
        * (constant)
        * Increase the iterator by the specified offset, moving it forward in the array.
        * The operation also causes the original iterator to be unchanged, and a new iterator to be returned.
        *
        * @return DequeIterator
        */
        DequeIterator operator+(const int offset) const {
            return DequeIterator{ m_pointer + offset };
        }

        /**
        * (constant)
        * Decrease the iterator by the specified offset, moving it backward in the array.
        * The operation also causes the original iterator to be unchanged, and a new iterator to be returned.
        *
        * @param offset
        * @return DequeIterator
        */
        DequeIterator operator-(const int offset) const {
            return DequeIterator{ m_pointer - offset };
        }

        /**
        * Return the element at the specified index.
        *
        * @param index
        * @return ValueType&
        */
        ValueType& operator[](const int index) {
            return m_pointer[index];
        }

        /**
        * (constant)
        * Return the element at the specified index.
        *
        * @param index
        * @return ValueType&
        */
        ValueType& operator[](const int index) const {
            return m_pointer[index];
        }

        /**
        * Return the dereferenced version of the iterator, which is the element it points to.
        *
        * @return ValueType&
        */
        ValueType& operator*() {
            return *m_pointer;
        }

        /**
        * (constant)
        * Return the dereferenced version of the iterator, which is the element it points to.
        *
        * @return ValueType&
        */
        ValueType& operator*() const {
            return *m_pointer;
        }

        /**
        * Return the pointer version of the iterator, which is the element it points to.
        *
        * @return ValueType*
        */
        ValueType* operator->() {
            return m_pointer;
        }

        /**
        * (constant)
        * Return the pointer version of the iterator, which is the element it points to.
        *
        * @return ValueType*
        */
        ValueType* operator->() const {
            return m_pointer;
        }

        /**
        * (constant)
        * Return true if the iterators point to the same element in the array, and false otherwise.
        *
        * @return bool
        */
        bool operator==(const DequeIterator& other) const {
            return m_pointer == other.m_pointer;
        }

        /**
        * (constant)
        * Return true if the iterators do not point to the same element in the array, and false otherwise.
        *
        * @return bool
        */
        bool operator!=(const DequeIterator& other) const {
            return m_pointer != other.m_pointer;
        }

        /**
        * (constant)
        * Return true if this iterator is less than the other iterator, and false otherwise.
        *
        * @return bool
        */
        bool operator<(const DequeIterator& other) const {
            return m_pointer < other.m_pointer;
        }

        /**
        * (constant)
        * Return true if this iterator is greater than the other iterator, and false otherwise.
        *
        * @return bool
        */
        bool operator>(const DequeIterator& other) const {
            return m_pointer > other.m_pointer;
        }

        /**
        * (constant)
        * Return true if this iterator is less or equal than the other iterator, and false otherwise.
        *
        * @return bool
        */
        bool operator<=(const DequeIterator& other) const {
            return m_pointer <= other.m_pointer;
        }

        /**
        * (constant)
        * Return true if this iterator is greater or equal than the other iterator, and false otherwise.
        *
        * @return bool
        */
        bool operator>=(const DequeIterator& other) const {
            return m_pointer >= other.m_pointer;
        }

        ValueType* pointer() const {
            return m_pointer;
        }

    private:
        ValueType* m_pointer{ nullptr };
    };
}

#endif //DATA_STRUCTURES_DEQUEITERATOR_H