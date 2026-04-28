//
// Created by Okan Ozbek on 4/9/2026.
//

#ifndef DATA_STRUCTURES_STRINGITERATOR_H
#define DATA_STRUCTURES_STRINGITERATOR_H

namespace dsa {
    class StringIterator {
    public:
        /**
        * Initialize a string iterator, pointing to the provided pointer.
        *
        * @param pointer
        */
        explicit StringIterator(const char* pointer) : m_pointer{ pointer } {}

        /**
        * Move the iterator to the next element in the string.
        *
        * @return StringIterator&
        */
        StringIterator& operator++() {
            ++m_pointer;
            return *this;
        }

        /**
        * Move the iterator to the previous element in the string.
        *
        * @return StringIterator&
        */
        StringIterator& operator--() {
            --m_pointer;
            return *this;
        }

        /**
        * (constant)
        * Increase the iterator by the specified offset, moving it forward in the string.
        * The operation also causes the original iterator to be unchanged, and a new iterator to be returned.
        *
        * @return StringIterator
        */
        StringIterator operator+(const int offset) const {
            return StringIterator{m_pointer + offset};
        }

        /**
        * (constant)
        * Decrease the iterator by the specified offset, moving it backward in the string.
        * The operation also causes the original iterator to be unchanged, and a new iterator to be returned.
        *
        * @param offset
        * @return StringIterator
        */
        StringIterator operator-(const int offset) const {
            return StringIterator{m_pointer - offset};
        }

        /**
        * Return the element at the specified index.
        *
        * @param index
        * @return const char&
        */
        const char& operator[](const int index) {
            return m_pointer[index];
        }

        /**
        * (constant)
        * Return the element at the specified index.
        *
        * @param index
        * @return const char&
        */
        const char& operator[](const int index) const {
            return m_pointer[index];
        }

        /**
        * Return the dereferenced version of the iterator, which is the element it points to.
        *
        * @return const char&
        */
        const char& operator*() {
            return *m_pointer;
        }

        /**
        * (constant)
        * Return the dereferenced version of the iterator, which is the element it points to.
        *
        * @return const char&
        */
        const char& operator*() const {
            return *m_pointer;
        }

        /**
        * Return the pointer version of the iterator, which is the element it points to.
        *
        * @return const char*
        */
        const char* operator->() {
            return m_pointer;
        }

        /**
        * (constant)
        * Return the pointer version of the iterator, which is the element it points to.
        *
        * @return const char*
        */
        const char* operator->() const {
            return m_pointer;
        }

        /**
        * (constant)
        * Return true if the iterators point to the same element in the string, and false otherwise.
        *
        * @return bool
        */
        bool operator==(const StringIterator& other) const {
            return m_pointer == other.m_pointer;
        }

        /**
        * (constant)
        * Return true if the iterators do not point to the same element in the string, and false otherwise.
        *
        * @return bool
        */
        bool operator!=(const StringIterator& other) const {
            return m_pointer != other.m_pointer;
        }

        /**
        * (constant)
        * Return true if this iterator is less than the other iterator, and false otherwise.
        *
        * @return bool
        */
        bool operator<(const StringIterator& other) const {
            return m_pointer < other.m_pointer;
        }

        /**
        * (constant)
        * Return true if this iterator is greater than the other iterator, and false otherwise.
        *
        * @return bool
        */
        bool operator>(const StringIterator& other) const {
            return m_pointer > other.m_pointer;
        }

        /**
        * (constant)
        * Return true if this iterator is less or equal than the other iterator, and false otherwise.
        *
        * @return bool
        */
        bool operator<=(const StringIterator& other) const {
            return m_pointer <= other.m_pointer;
        }

        /**
        * (constant)
        * Return true if this iterator is greater or equal than the other iterator, and false otherwise.
        *
        * @return bool
        */
        bool operator>=(const StringIterator& other) const {
            return m_pointer >= other.m_pointer;
        }

        [[nodiscard]] const char* pointer() const {
            return m_pointer;
        }

    private:
        const char* m_pointer;
    };
}

#endif // DATA_STRUCTURES_STRINGITERATOR_H