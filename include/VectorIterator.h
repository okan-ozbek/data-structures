//
// Created by Okan Ozbek on 4/7/2026.
//

#ifndef DATA_STRUCTURES_VECTORITERATOR_H
#define DATA_STRUCTURES_VECTORITERATOR_H


namespace dsa {
    template<typename TVector>
    class VectorIterator {
    public:
        using ValueType = TVector::ValueType;

        /**
         * Initialize a vector iterator, pointing to the provided pointer.
         *
         * @param pointer
         */
        explicit VectorIterator(ValueType* pointer) : m_pointer{ pointer } {}

        /**
         * Move the iterator to the next element in the array.
         *
         * @return VectorIterator&
         */
        VectorIterator& operator++() {
            ++m_pointer;
            return *this;
        }

        /**
         * Move the iterator to the previous element in the array.
         *
         * @return VectorIterator&
         */
        VectorIterator& operator--() {
            --m_pointer;
            return *this;
        }

        /**
         * (constant)
         * Increase the iterator by the specified offset, moving it forward in the array.
         * The operation also causes the original iterator to be unchanged, and a new iterator to be returned.
         *
         * @return VectorIterator
         */
        VectorIterator operator+(const int offset) const {
            return VectorIterator{ m_pointer + offset };
        }

        /**
         * (constant)
         * Decrease the iterator by the specified offset, moving it backward in the array.
         * The operation also causes the original iterator to be unchanged, and a new iterator to be returned.
         *
         * @param offset
         * @return VectorIterator
         */
        VectorIterator operator-(const int offset) const {
            return VectorIterator{ m_pointer - offset };
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
        bool operator==(const VectorIterator& other) const {
            return m_pointer == other.m_pointer;
        }

        /**
         * (constant)
         * Return true if the iterators do not point to the same element in the array, and false otherwise.
         *
         * @return bool
         */
        bool operator!=(const VectorIterator& other) const {
            return m_pointer != other.m_pointer;
        }

        /**
         * (constant)
         * Return true if this iterator is less than the other iterator, and false otherwise.
         *
         * @return bool
         */
        bool operator<(const VectorIterator& other) const {
            return m_pointer < other.m_pointer;
        }

        /**
         * (constant)
         * Return true if this iterator is greater than the other iterator, and false otherwise.
         *
         * @return bool
         */
        bool operator>(const VectorIterator& other) const {
            return m_pointer > other.m_pointer;
        }

        /**
         * (constant)
         * Return true if this iterator is less or equal than the other iterator, and false otherwise.
         *
         * @return bool
         */
        bool operator<=(const VectorIterator& other) const {
            return m_pointer <= other.m_pointer;
        }

        /**
         * (constant)
         * Return true if this iterator is greater or equal than the other iterator, and false otherwise.
         *
         * @return bool
         */
        bool operator>=(const VectorIterator& other) const {
            return m_pointer >= other.m_pointer;
        }

        ValueType* pointer() const {
            return m_pointer;
        }

    private:
        ValueType* m_pointer;
    };
}


#endif //DATA_STRUCTURES_VECTORITERATOR_H