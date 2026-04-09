//
// Created by Dorza on 4/7/2026.
//

#ifndef TEMP_CPP_STRING_H
#define TEMP_CPP_STRING_H


#include <cstddef>
#include <iostream>

#include "StringIterator.h"

class String {
public:
    using Iterator = StringIterator;

    String() :
        data_{static_cast<char*>(::operator new(DEFAULT_CAPACITY * sizeof(char)))},
        size_{0},
        capacity_{DEFAULT_CAPACITY}
    {}

    String(const char* string) {
        const std::size_t capacity = (strlen(string) < DEFAULT_CAPACITY)
            ? DEFAULT_CAPACITY
            : strlen(string);

        data_ = static_cast<char*>(::operator new(capacity * sizeof(char)));
        capacity_ = capacity;
        size_ = strlen(string);

        for (std::size_t i{}; i < size_; ++i) {
            new (&data_[i]) char(string[i]);
        }

        data_[size_] = '\0';
    }

    String(const String& string) {
        const std::size_t capacity = (string.size() < DEFAULT_CAPACITY)
            ? DEFAULT_CAPACITY
            : string.size();

        data_ = static_cast<char*>(::operator new(capacity * sizeof(char)));
        capacity_ = capacity;
        size_ = string.size();

        for (std::size_t i{}; i < size_; ++i) {
            new (&data_[i]) char(string.data_[i]);
        }

        data_[size_] = '\0';
    }

    String(String&& string) noexcept :
        data_{string.data_},
        size_{string.size_},
        capacity_{string.capacity_}
    {
        string.data_ = nullptr;
        string.size_ = 0;
        string.capacity_ = 0;
    }

    ~String() {
        ::operator delete(data_);
    }

    char operator[](const std::size_t index) const {
        return data_[index];
    }

    String& operator=(const char* string) {
        assign(string);

        return *this;
    }

    String& operator=(const String& string) {
        assign(string.data());

        return *this;
    }

    String& operator=(String&& string) noexcept {
        ::operator delete(data_);

        data_ = string.data_;
        size_ = string.size_;

        string.data_ = nullptr;
        string.size_ = 0;

        return *this;
    }

    String& operator+=(const String& string) {
        append(string.data());
        return *this;
    }

    String& operator+=(const char* string) {
        append(string);
        return *this;
    }

    String& operator+=(const char character) {
        append(character);
        return *this;
    }

    bool operator==(const String& string) const {
        return is_equal(string);
    }

    bool operator==(const char* string) const {
        return is_equal(string);
    }

    bool operator!=(const String& string) const {
        return is_equal(string) == false;
    }

    bool operator!=(const char* string) const {
        return is_equal(string) == false;
    }

    friend std::ostream& operator<<(std::ostream& os, const String& string);

    [[nodiscard]] Iterator begin() const {
        return Iterator{data_};
    }

    [[nodiscard]] Iterator end() const {
        return Iterator{data_ + size_};
    }

    [[nodiscard]] char at(const int index) const {
        return data_[index];
    }

    [[nodiscard]] char back() const {
        return data_[size_ - 1];
    }

    [[nodiscard]] char front() const {
        return data_[0];
    }

    // TODO: add replace() aswel

    [[nodiscard]] String find(const char* string) const {
        if (strlen(string) > size_) {
            return {};
        }

        for (std::size_t i{}; i < size_; ++i) {
            if (substr(static_cast<int>(i)).is_equal(string)) {
                return substr(static_cast<int>(i));
            }
        }

        return {};
    }

    [[nodiscard]] String find(const String& string) const {
        if (string.size() > size_) {
            return {};
        }

        for (std::size_t i{}; i < size_; ++i) {
            if (string.is_equal(substr(static_cast<int>(i)))) {
                return substr(static_cast<int>(i));
            }
        }

        return {};
    }

    [[nodiscard]] String substr(const int index, int length = -1) const {
        String result{};

        if (length == 0 || index >= size_) {
            return {};
        }

        if (length <= -1 || length > size_ - index) {
            length = static_cast<int>(size_) - index;
        }

        for (int i{}; i < length; ++i) {
            result += data_[index + i];
        }

        return result;
    }

    [[nodiscard]] std::size_t size() const {
        return size_;
    }

    [[nodiscard]] std::size_t capacity() const {
        return capacity_;
    }

    [[nodiscard]] const char* data() const {
        return data_;
    }

    [[nodiscard]] bool is_empty() const {
        return size_ == 0;
    }

    [[nodiscard]] bool is_equal(const char* string) const {
        if (size_ != strlen(string)) {
            return false;
        }

        for (std::size_t i{}; i < size_; ++i) {
            if (data_[i] != string[i]) {
                return false;
            }
        }

        return true;
    }

    [[nodiscard]] bool is_equal(const String& string) const {
        if (size_ != string.size_) {
            return false;
        }

        for (std::size_t i{}; i < size_; ++i) {
            if (data_[i] != string.data_[i]) {
                return false;
            }
        }

        return true;
    }

    [[nodiscard]] static std::size_t strlen(const char* string) {
        std::size_t length{};

        while (string[length] != '\0') {
            ++length;
        }

        return length;
    }

private:
    constexpr static std::size_t DEFAULT_CAPACITY = 10;

    char* data_;
    std::size_t size_;
    std::size_t capacity_;

    void assign(const char* string) {
        const std::size_t string_length = strlen(string);

        if (string_length + 1 > capacity_) {
            const std::size_t new_capacity = (capacity_ * 2 > string_length + 1)
                ? capacity_ * 2
                : string_length + 1;

            resize(new_capacity);
        }

        for (std::size_t i{}; i < string_length; ++i) {
            data_[i] = string[i];
        }

        size_ = string_length;
        data_[size_] = '\0';
    }

    void append(const char* string) {
        const size_t string_length = strlen(string);
        const size_t new_size = size_ + strlen(string);

        if (new_size + 1 > capacity_) {
            resize(capacity_ * 2 + string_length);
        }

        for (std::size_t i{}; i < string_length; ++i) {
            new (&data_[size_ + i]) char(string[i]);
        }

        size_ = new_size;
        data_[size_] = '\0';
    }

    void append(const char character) {
        if (size_ + 1 > capacity_) {
            resize(capacity_ * 2 + 1);
        }

        new (&data_[size_]) char(character);
        ++size_;

        data_[size_] = '\0';
    }

    void resize(const std::size_t capacity) {
        auto* new_data = static_cast<char*>(::operator new(capacity * sizeof(char)));

        const std::size_t movable_capacity = (capacity < size_)
            ? capacity
            : size_;

        for (std::size_t i{}; i < movable_capacity; ++i) {
            new (&new_data[i]) char(data_[i]);
        }

        ::operator delete(data_);

        data_ = new_data;
        capacity_ = capacity;
    }
};

inline std::ostream& operator<<(std::ostream& os, const String& string) {
    for (std::size_t i{}; i < string.size(); ++i) {
        os << string[i];
    }

    return os;
}


#endif