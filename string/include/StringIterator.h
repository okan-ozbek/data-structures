//
// Created by Dorza on 4/9/2026.
//

#ifndef TEMP_CPP_STRINGITERATOR_H
#define TEMP_CPP_STRINGITERATOR_H

class StringIterator {
public:
    explicit StringIterator(const char* ptr) : ptr_{ptr} {}

    StringIterator& operator++() {
        ++ptr_;
        return *this;
    }

    StringIterator& operator--() {
        --ptr_;
        return *this;
    }

    StringIterator operator+(const int offset) const {
        return StringIterator{ptr_ + offset};
    }

    StringIterator operator-(const int offset) const {
        return StringIterator{ptr_ - offset};
    }

    const char& operator[](const int index) {
        return ptr_[index];
    }

    const char& operator[](const int index) const {
        return ptr_[index];
    }

    const char& operator*() {
        return *ptr_;
    }

    const char& operator*() const {
        return *ptr_;
    }

    const char* operator->() {
        return ptr_;
    }

    const char* operator->() const {
        return ptr_;
    }

    bool operator==(const StringIterator& other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const StringIterator& other) const {
        return ptr_ != other.ptr_;
    }

    bool operator<(const StringIterator& other) const {
        return ptr_ < other.ptr_;
    }

    bool operator>(const StringIterator& other) const {
        return ptr_ > other.ptr_;
    }

    bool operator<=(const StringIterator& other) const {
        return ptr_ <= other.ptr_;
    }

    bool operator>=(const StringIterator& other) const {
        return ptr_ >= other.ptr_;
    }

    const char* ptr() const {
        return ptr_;
    }

private:
    const char* ptr_;
};

#endif // TEMP_CPP_STRINGITERATOR_H