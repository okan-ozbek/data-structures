//
// Created by Okan Ozbek on 4/9/2026.
//

#ifndef TEMP_CPP_STRINGITERATOR_H
#define TEMP_CPP_STRINGITERATOR_H

namespace dsa {
    class StringIterator {
    public:
        /**
        * Initialize a string iterator, pointing to the provided pointer.
        *
        * @param ptr
        */
        explicit StringIterator(const char* ptr) : ptr_{ptr} {}

        /**
        * Move the iterator to the next element in the string.
        *
        * @return StringIterator&
        */
        StringIterator& operator++() {
            ++ptr_;
            return *this;
        }

        /**
        * Move the iterator to the previous element in the string.
        *
        * @return StringIterator&
        */
        StringIterator& operator--() {
            --ptr_;
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
            return StringIterator{ptr_ + offset};
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
            return StringIterator{ptr_ - offset};
        }

        /**
        * Return the element at the specified index.
        *
        * @param index
        * @return const char&
        */
        const char& operator[](const int index) {
            return ptr_[index];
        }

        /**
        * (constant)
        * Return the element at the specified index.
        *
        * @param index
        * @return const char&
        */
        const char& operator[](const int index) const {
            return ptr_[index];
        }

        /**
        * Return the dereferenced version of the iterator, which is the element it points to.
        *
        * @return const char&
        */
        const char& operator*() {
            return *ptr_;
        }

        /**
        * (constant)
        * Return the dereferenced version of the iterator, which is the element it points to.
        *
        * @return const char&
        */
        const char& operator*() const {
            return *ptr_;
        }

        /**
        * Return the pointer version of the iterator, which is the element it points to.
        *
        * @return const char*
        */
        const char* operator->() {
            return ptr_;
        }

        /**
        * (constant)
        * Return the pointer version of the iterator, which is the element it points to.
        *
        * @return const char*
        */
        const char* operator->() const {
            return ptr_;
        }

        /**
        * (constant)
        * Return true if the iterators point to the same element in the string, and false otherwise.
        *
        * @return bool
        */
        bool operator==(const StringIterator& other) const {
            return ptr_ == other.ptr_;
        }

        /**
        * (constant)
        * Return true if the iterators do not point to the same element in the string, and false otherwise.
        *
        * @return bool
        */
        bool operator!=(const StringIterator& other) const {
            return ptr_ != other.ptr_;
        }

        /**
        * (constant)
        * Return true if this iterator is less than the other iterator, and false otherwise.
        *
        * @return bool
        */
        bool operator<(const StringIterator& other) const {
            return ptr_ < other.ptr_;
        }

        /**
        * (constant)
        * Return true if this iterator is greater than the other iterator, and false otherwise.
        *
        * @return bool
        */
        bool operator>(const StringIterator& other) const {
            return ptr_ > other.ptr_;
        }

        /**
        * (constant)
        * Return true if this iterator is less or equal than the other iterator, and false otherwise.
        *
        * @return bool
        */
        bool operator<=(const StringIterator& other) const {
            return ptr_ <= other.ptr_;
        }

        /**
        * (constant)
        * Return true if this iterator is greater or equal than the other iterator, and false otherwise.
        *
        * @return bool
        */
        bool operator>=(const StringIterator& other) const {
            return ptr_ >= other.ptr_;
        }

        [[nodiscard]] const char* ptr() const {
            return ptr_;
        }

    private:
        const char* ptr_;
    };
}

#endif // TEMP_CPP_STRINGITERATOR_H