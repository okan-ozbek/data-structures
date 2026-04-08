//
// Created by Dorza on 4/7/2026.
//

#ifndef TEMP_CPP_DYNAMICARRAYITERATOR_H
#define TEMP_CPP_DYNAMICARRAYITERATOR_H


template<typename TDynamicArray>
class DynamicArrayIterator {
public:
    using ValueType = TDynamicArray::ValueType;

    /**
     * Initialize a dynamic array iterator, pointing to the provided pointer.
     *
     * @param ptr
     */
    explicit DynamicArrayIterator(ValueType* ptr) : ptr_{ptr} {}

    /**
     * Move the iterator to the next element in the array.
     *
     * @return DynamicArrayIterator&
     */
    DynamicArrayIterator& operator++() {
        ++ptr_;
        return *this;
    }

    /**
     * Move the iterator to the previous element in the array.
     *
     * @return DynamicArrayIterator&
     */
    DynamicArrayIterator& operator--() {
        --ptr_;
        return *this;
    }

    /**
     * (constant)
     * Increase the iterator by the specified offset, moving it forward in the array.
     * The operation also causes the original iterator to be unchanged, and a new iterator to be returned.
     *
     * @return DynamicArrayIterator
     */
    DynamicArrayIterator operator+(const int offset) const {
        return DynamicArrayIterator{ptr_ + offset};
    }

    /**
     * (constant)
     * Decrease the iterator by the specified offset, moving it backward in the array.
     * The operation also causes the original iterator to be unchanged, and a new iterator to be returned.
     *
     * @param offset
     * @return DynamicArrayIterator
     */
    DynamicArrayIterator operator-(const int offset) const {
        return DynamicArrayIterator{ptr_ - offset};
    }

    /**
     * Return the element at the specified index.
     *
     * @param index
     * @return ValueType&
     */
    ValueType& operator[](const int index) {
        return ptr_[index];
    }

    /**
     * (constant)
     * Return the element at the specified index.
     *
     * @param index
     * @return ValueType&
     */
    ValueType& operator[](const int index) const {
        return ptr_[index];
    }

    /**
     * Return the dereferenced version of the iterator, which is the element it points to.
     *
     * @return ValueType&
     */
    ValueType& operator*() {
        return *ptr_;
    }

    /**
     * (constant)
     * Return the dereferenced version of the iterator, which is the element it points to.
     *
     * @return ValueType&
     */
    ValueType& operator*() const {
        return *ptr_;
    }

    /**
     * Return the pointer version of the iterator, which is the element it points to.
     *
     * @return ValueType*
     */
    ValueType* operator->() {
        return ptr_;
    }

    /**
     * (constant)
     * Return the pointer version of the iterator, which is the element it points to.
     *
     * @return ValueType*
     */
    ValueType* operator->() const {
        return ptr_;
    }

    /**
     * (constant)
     * Return true if the iterators point to the same element in the array, and false otherwise.
     *
     * @return bool
     */
    bool operator==(const DynamicArrayIterator& other) const {
        return ptr_ == other.ptr_;
    }

    /**
     * (constant)
     * Return true if the iterators do not point to the same element in the array, and false otherwise.
     *
     * @return bool
     */
    bool operator!=(const DynamicArrayIterator& other) const {
        return ptr_ != other.ptr_;
    }

    /**
     * (constant)
     * Return true if this iterator is less than the other iterator, and false otherwise.
     *
     * @return bool
     */
    bool operator<(const DynamicArrayIterator& other) const {
        return ptr_ < other.ptr_;
    }

    /**
     * (constant)
     * Return true if this iterator is greater than the other iterator, and false otherwise.
     *
     * @return bool
     */
    bool operator>(const DynamicArrayIterator& other) const {
        return ptr_ > other.ptr_;
    }

    /**
     * (constant)
     * Return true if this iterator is less or equal than the other iterator, and false otherwise.
     *
     * @return bool
     */
    bool operator<=(const DynamicArrayIterator& other) const {
        return ptr_ <= other.ptr_;
    }

    /**
     * (constant)
     * Return true if this iterator is greater or equal than the other iterator, and false otherwise.
     *
     * @return bool
     */
    bool operator>=(const DynamicArrayIterator& other) const {
        return ptr_ >= other.ptr_;
    }

private:
    ValueType* ptr_;
};

#endif //TEMP_CPP_DYNAMICARRAYITERATOR_H