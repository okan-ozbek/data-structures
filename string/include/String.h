//
// Created by Okan Ozbek on 4/7/2026.
//

#ifndef TEMP_CPP_STRING_H
#define TEMP_CPP_STRING_H


#include <cstddef>
#include <iostream>

#include "StringIterator.h"

/**
 * A resizable string implementation that provides capabilities for
 * storing and managing a collection of characters, it builds upon the
 * old-school C-style string by implementing dynamic resizing and manual memory management
 * while also providing a more user-friendly interface for string manipulation.
 */
class String {
public:
    using Iterator = StringIterator;

    /**
     * Initialize an empty string
     *
     * Time complexity: O(1) since it only involves allocating memory for the default capacity, without initializing any characters.
     */
    String() :
        data_{static_cast<char*>(::operator new(DEFAULT_CAPACITY * sizeof(char)))},
        size_{0},
        capacity_{DEFAULT_CAPACITY}
    {}

    /**
     * Initialize a string with the provided C-style string, copying its characters into the new string.
     *
     * Time complexity: O(n) due to the need to copy each character from the provided C-style string into the new string.
     * @param string
     */
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

    /**
     * Initialize a string with the provided String&, copying its characters into the new string.
     *
     * Time complexity: O(n) due to the need to copy each character from the provided String& into the new string.
     * @param string
     */
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

    /**
     * Initialize a string with the provided String&&, moving its characters into the new string.
     *
     * Time complexity: O(1) since we are only moving pointers and size/capacity values without copying individual characters.
     * @param string
     */
    String(String&& string) noexcept :
        data_{string.data_},
        size_{string.size_},
        capacity_{string.capacity_}
    {
        string.data_ = nullptr;
        string.size_ = 0;
        string.capacity_ = 0;
    }

    /**
     * Destruct the string object deallocating memory and resetting size and capacity.
     *
     * Time complexity: O(1) since it only involves deallocating memory and resetting size and capacity values, without the need to destruct individual characters.
     */
    ~String() {
        ::operator delete(data_);

        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    /**
     * Get the character at the given index.
     *
     * Time complexity: O(1) we already have the index and pointer ready.
     * @param index
     * @return char
     */
    char operator[](const std::size_t index) const {
        return data_[index];
    }

    /**
     * Copy over the other string using the (=) operator.
     *
     * Time complexity: O(n) due to the need to resize the string if the new size exceeds the current capacity, which involves copying all existing characters to a new memory location.
     * @param string
     * @return String&
     */
    String& operator=(const char* string) {
        assign(string);

        return *this;
    }

    /**
     * Copy over the other string using the (=) operator.
     *
     * Time complexity: O(n) due to the need to resize the string if the new size exceeds the current capacity, which involves copying all existing characters to a new memory location.
     * @param string
     * @return String&
     */
    String& operator=(const String& string) {
        assign(string.data());

        return *this;
    }

    /**
     * Move over the other string using the (=) operator.
     *
     * Time complexity: O(1) since we are only moving pointers and size/capacity values without copying individual characters.
     * @param string
     * @return String&
     */
    String& operator=(String&& string) noexcept {
        ::operator delete(data_);

        data_ = string.data_;
        size_ = string.size_;

        string.data_ = nullptr;
        string.size_ = 0;

        return *this;
    }

    /**
     * Append the provided string to the end of the current string using the (+=) operator.
     *
     * Time complexity: O(n) due to the need to resize the string if the new size exceeds the current capacity, which involves copying all existing characters to a new memory location.
     * @param string
     * @return String&
     */
    String& operator+=(const String& string) {
        append(string.data());
        return *this;
    }

    /**
     * Append the provided string to the end of the current string using the (+=) operator.
     *
     * Time complexity: O(n) due to the need to resize the string if the new size exceeds the current capacity, which involves copying all existing characters to a new memory location.
     * @param string
     * @return String&
     */
    String& operator+=(const char* string) {
        append(string);
        return *this;
    }

    /**
     * Append the provided character to the end of the current string using the (+=) operator.
     *
     * Time complexity: O(n) due to the need to resize the string if the new size exceeds the current capacity, which involves copying all existing characters to a new memory location.
     * @param character
     * @return String&
     */
    String& operator+=(const char character) {
        append(character);
        return *this;
    }

    /**
     * Check if the other string is equal to the current string using the (==) operator.
     *
     * Time complexity: O(n) due to the need to compare each character in the string for equality.
     * @param string
     * @return bool
     */
    bool operator==(const String& string) const {
        return is_equal(string);
    }

    /**
     * Check if the other const char* is equal to the current string using the (==) operator.
     *
     * Time complexity: O(n) due to the need to compare each character in the string for equality.
     * @param string
     * @return bool
     */
    bool operator==(const char* string) const {
        return is_equal(string);
    }

    /**
     * Check if the other string is NOT equal to the current string using the (==) operator.
     *
     * Time complexity: O(n) due to the need to compare each character in the string for equality.
     * @param string
     * @return bool
     */
    bool operator!=(const String& string) const {
        return is_equal(string) == false;
    }

    /**
     * Check if the other const char* is NOT equal to the current string using the (==) operator.
     *
     * Time complexity: O(n) due to the need to compare each character in the string for equality.
     * @param string
     * @return bool
     */
    bool operator!=(const char* string) const {
        return is_equal(string) == false;
    }

    /**
     * Output the string to the provided output stream using the (<<) operator.
     *
     * Time complexity: O(n) due to the need to output each character in the string to the output stream.
     * @param os
     * @param string
     * @return std::ostream&
     */
    friend std::ostream& operator<<(std::ostream& os, const String& string);

    /**
     * Get the iterator pointing to the beginning of the string.
     *
     * Time complexity: O(1)
     * @return Iterator
     */
    [[nodiscard]] Iterator begin() const {
        return Iterator{data_};
    }

    /**
     * Get the iterator pointing to the end of the string.
     *
     * Time complexity: O(1)
     * @return Iterator
     */
    [[nodiscard]] Iterator end() const {
        return Iterator{data_ + size_};
    }

    /**
     * Get the char at the given index.
     *
     * Time complexity: O(1)
     * @param index
     * @return char
     */
    [[nodiscard]] char at(const int index) const {
        return data_[index];
    }

    /**
     * Get the last char.
     *
     * Time complexity: O(1)
     * @return char
     */
    [[nodiscard]] char back() const {
        return data_[size_ - 1];
    }

    /**
     * Get the first char.
     *
     * Time complexity: O(1)
     * @return char
     */
    [[nodiscard]] char front() const {
        return data_[0];
    }

    // TODO: add replace() as wel (maybe we can use iterators to make it more efficient)

    // TODO: check for improvements... (use iterators maybe? instead of having a O(n^2) speed
    /**
     * Return a copy of the found string or an empty string if nothing is found.
     *
     * Time complexity: O(n^2) due to the need to compare each character in the string for equality, and then create a new string for the found substring.
     * @param string
     * @return String
     */
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

    // TODO: check for improvements... (use iterators maybe? instead of having a O(n^2) speed
    /**
     * Return a copy of the found string or an empty string if nothing is found.
     *
     * Time complexity: O(n^2) due to the need to compare each character in the string for equality, and then create a new string for the found substring.
     * @param string
     * @return String
     */
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

    // TODO: check for improvements... Check if we can use iterators or something else so we don't have to copy it over like this, maybe copying is the only way..
    /**
     * Get a substring of the current string starting from the given index and with the given length, or until the end of the string if the length is not provided or exceeds the remaining characters.
     *
     * Time complexity: O(n) due to the need to create a new string for the substring, which involves copying characters from the original string to the new string.
     * @param index
     * @param length
     * @return String
     */
    [[nodiscard]] String substr(const int index, int length = -1) const {
        if (length == 0 || index >= size_) {
            return {};
        }

        if (length <= -1 || length > size_ - index) {
            length = static_cast<int>(size_) - index;
        }

        String result{};
        for (int i{}; i < length; ++i) {
            result += data_[index + i];
        }

        return result;
    }

    /**
     * Get the size of the string, which is the number of characters currently stored in the string.
     *
     * Time complexity: O(1)
     * @return std::size_t
     */
    [[nodiscard]] std::size_t size() const {
        return size_;
    }

    /**
     * Get the capacity of the string, which refers to the total allocated memory for the string.
     *
     * Time complexity: O(1)
     * @return std::size_t
     */
    [[nodiscard]] std::size_t capacity() const {
        return capacity_;
    }

    /**
     * Get the raw string data as a C-style string.
     *
     * Time complexity: O(1)
     * @return const char*
     */
    [[nodiscard]] const char* data() const {
        return data_;
    }

    /**
     * Check if there are no characters in the string, which is determined by checking if the size is zero.
     *
     * Time complexity: O(1)
     * @return bool
     */
    [[nodiscard]] bool is_empty() const {
        return size_ == 0;
    }

    /**
     * Check if const char* is equal to our String
     *
     * Time complexity: O(n) due to the need to compare each character in the string for equality.
     * @param string
     * @return bool
     */
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

    /**
     * Check if const String& is equal to our String
     *
     * Time complexity: O(n) due to the need to compare each character in the string for equality.
     * @param string
     * @return bool
     */
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

    /**
     * Public static function to determine the length of a C-style string.
     *
     * Time complexity: O(n) due to the need to iterate through each character in the C-style string until the null terminator is reached to count the number of characters.
     * @param string
     * @return std::size_t
     */
    [[nodiscard]] static std::size_t strlen(const char* string) {
        std::size_t length{0};
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

    /**
     * Assign characters to our C-style string, if the we don't have enough memory allocated, we will resize the array and reserve enough memory to fit the new string.
     *
     * Time complexity: O(n + m) when we need to resize, else it is O(n)
     * @param string
     */
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

    /**
     * Append characters to the end of our C-style string, if we don't have enough memory allocated, we will resize the array and reserve enough memory to fir the new string.
     *
     * Time complexity: O(n + m) when we need to resize, else it is O(n)
     * @param string
     */
    void append(const char* string) {
        const std::size_t string_length = strlen(string);
        const std::size_t new_size = size_ + strlen(string);

        if (new_size + 1 > capacity_) {
            resize(capacity_ * 2 + string_length);
        }

        for (std::size_t i{}; i < string_length; ++i) {
            new (&data_[size_ + i]) char(string[i]);
        }

        size_ = new_size;
        data_[size_] = '\0';
    }

    /**
     * Append a single character to the end of our C-style string.
     *
     * Time complexity: O(n) when we need to resize, else it would be O(1)
     * @param character
     */
    void append(const char character) {
        if (size_ + 1 > capacity_) {
            resize(capacity_ * 2 + 1);
        }

        new (&data_[size_]) char(character);
        ++size_;

        data_[size_] = '\0';
    }

    /**
     * Resize the array to the specified capacity, moving existing elements to the new memory location.
     *
     * Time complexity: O(n) due to the need to copy each character from the old array to the new array, and then deallocate the old array.
     * @param capacity
     */
    void resize(const std::size_t capacity) {
        char* new_data = static_cast<char*>(::operator new(capacity * sizeof(char)));

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

/**
 * Output the string to the provided output stream using the (<<) operator.
 *
 * @param os
 * @param string
 * @return std::ostream&
 */
inline std::ostream& operator<<(std::ostream& os, const String& string) {
    for (std::size_t i{}; i < string.size(); ++i) {
        os << string[i];
    }

    return os;
}


#endif