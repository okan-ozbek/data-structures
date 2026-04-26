//
// Created by Okan Ozbek on 4/7/2026.
//

#ifndef TEMP_CPP_STRING_H
#define TEMP_CPP_STRING_H


#include <cstddef>
#include <iostream>
#include "StringIterator.h"

namespace dsa {
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
            m_data{ static_cast<char*>(::operator new(DEFAULT_CAPACITY * sizeof(char))) },
            m_size{ 0 },
            m_capacity{ DEFAULT_CAPACITY }
        {}

        /**
        * Initialize a string with the provided C-style string, copying its characters into the new string.
        *
        * Time complexity: O(n) due to the need to copy each character from the provided C-style string into the new string.
        * @param string
        */
        String(const char* string) {
            const std::size_t capacity = (Strlen(string) < DEFAULT_CAPACITY)
                ? DEFAULT_CAPACITY
                : Strlen(string) + DEFAULT_CAPACITY;

            m_data = static_cast<char*>(::operator new(capacity * sizeof(char)));
            m_capacity = capacity;
            m_size = Strlen(string);

            for (std::size_t i{}; i < m_size; ++i) {
                new (&m_data[i]) char(string[i]);
            }

            m_data[m_size] = '\0';
        }

        /**
        * Initialize a string with the provided String&, copying its characters into the new string.
        *
        * Time complexity: O(n) due to the need to copy each character from the provided String& into the new string.
        * @param string
        */
        String(const String& string) :
            m_data{ static_cast<char*>(::operator new(string.m_capacity * sizeof(char))) },
            m_size{ string.Size() }
        {
            const std::size_t capacity = (string.Size() < DEFAULT_CAPACITY)
                ? DEFAULT_CAPACITY
                : Strlen(string.Data()) + DEFAULT_CAPACITY;

            m_capacity = capacity;

            for (std::size_t i{}; i < m_size; ++i) {
                new (&m_data[i]) char(string.m_data[i]);
            }

            m_data[m_size] = '\0';
        }

        /**
        * Initialize a string with the provided String&&, moving its characters into the new string.
        *
        * Time complexity: O(1) since we are only moving pointers and size/capacity values without copying individual characters.
        * @param string
        */
        String(String&& string) noexcept :
            m_data{ static_cast<char*>(::operator new(string.m_capacity * sizeof(char))) },
            m_size{ string.m_size },
            m_capacity{ string.m_capacity }
        {
            for (std::size_t i{}; i < m_size; ++i) {
                new (&m_data[i]) char(string.m_data[i]);
            }

            m_data[m_size] = '\0';

            ::operator delete(string.m_data);
            string.m_data = nullptr;
            string.m_size = 0;
            string.m_capacity = 0;
        }

        /**
        * Destruct the string object deallocating memory and resetting size and capacity.
        *
        * Time complexity: O(1) since it only involves deallocating memory and resetting size and capacity values, without the need to destruct individual characters.
        */
        ~String() {
            ::operator delete(m_data);

            m_data = nullptr;
            m_size = 0;
            m_capacity = 0;
        }

        /**
        * Get the character at the given index.
        *
        * Time complexity: O(1) we already have the index and pointer ready.
        * @param index
        * @return char
        */
        char operator[](const std::size_t index) const {
            return m_data[index];
        }

        /**
        * Copy over the other string using the (=) operator.
        *
        * Time complexity: O(n) due to the need to resize the string if the new size exceeds the current capacity, which involves copying all existing characters to a new memory location.
        * @param string
        * @return String&
        */
        String& operator=(const char* string) {
            Assign(string);

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
            Assign(string.Data());

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
            ::operator delete(m_data);

            m_data = string.m_data;
            m_size = string.m_size;

            string.m_data = nullptr;
            string.m_size = 0;

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
            Append(string.Data());
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
            Append(string);
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
            Append(character);
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
            return IsEqual(string);
        }

        /**
        * Check if the other const char* is equal to the current string using the (==) operator.
        *
        * Time complexity: O(n) due to the need to compare each character in the string for equality.
        * @param string
        * @return bool
        */
        bool operator==(const char* string) const {
            return IsEqual(string);
        }

        /**
        * Check if the other string is NOT equal to the current string using the (==) operator.
        *
        * Time complexity: O(n) due to the need to compare each character in the string for equality.
        * @param string
        * @return bool
        */
        bool operator!=(const String& string) const {
            return IsEqual(string) == false;
        }

        /**
        * Check if the other const char* is NOT equal to the current string using the (==) operator.
        *
        * Time complexity: O(n) due to the need to compare each character in the string for equality.
        * @param string
        * @return bool
        */
        bool operator!=(const char* string) const {
            return IsEqual(string) == false;
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
        [[nodiscard]] Iterator Begin() const {
            return Iterator{ m_data };
        }

        /**
        * Get the iterator pointing to the end of the string.
        *
        * Time complexity: O(1)
        * @return Iterator
        */
        [[nodiscard]] Iterator End() const {
            return Iterator{ m_data + m_size };
        }

        /**
        * Get the char at the given index.
        *
        * Time complexity: O(1)
        * @param index
        * @return char
        */
        [[nodiscard]] char At(const int index) const {
            return m_data[index];
        }

        /**
        * Get the last char.
        *
        * Time complexity: O(1)
        * @return char
        */
        [[nodiscard]] char Back() const {
            return m_data[m_size - 1];
        }

        /**
        * Get the first char.
        *
        * Time complexity: O(1)
        * @return char
        */
        [[nodiscard]] char Front() const {
            return m_data[0];
        }

        /**
        * Replace the first occurrence of `pattern` with `replacement`.
        *
        * Uses Rabin-Karp (via find_index) to locate the pattern, then reconstructs the
        * string as: [prefix] + [replacement] + [suffix].
        *
        * Time complexity: O(n + m) average for the search, O(n + r) for reconstruction,
        *                  where n = string length, m = pattern length, r = replacement length.
        *
        * @param pattern     The substring to search for.
        * @param replacement The string to substitute in place of the pattern.
        * @return Reference to the modified string, or unmodified if pattern not found.
        */
        String& Replace(const char* pattern, const char* replacement) {
            const int index = FindIndex(pattern);
            if (index == -1) {
                return *this;
            }

            const std::size_t patternLength = Strlen(pattern);
            const std::size_t replacementLength = Strlen(replacement);

            const std::size_t newSize = m_size - patternLength + replacementLength;
            const std::size_t newCapacity = (m_capacity * 2 > newSize + 1)
                ? m_capacity * 2
                : newSize + 1;

            auto* newData = static_cast<char*>(::operator new(newCapacity * sizeof(char)));

            for (int i{}; i < index; ++i) {
                new (&newData[i]) char(m_data[i]);
            }

            for (std::size_t i{}; i < replacementLength; ++i) {
                new (&newData[index + i]) char(replacement[i]);
            }

            for (std::size_t i{}; i < m_size - index - patternLength; ++i) {
                new (&newData[index + replacementLength + i]) char(m_data[index + patternLength + i]);
            }

            newData[newSize] = '\0';

            ::operator delete(m_data);
            m_data = newData;
            m_size = newSize;
            m_capacity = newCapacity;

            return *this;
        }

        /**
        * Replace the first occurrence of `pattern` with `replacement`.
        *
        * Uses Rabin-Karp (via find_index) to locate the pattern, then reconstructs the
        * string as: [prefix] + [replacement] + [suffix].
        *
        * Time complexity: O(n + m) average for the search, O(n + r) for reconstruction,
        *                  where n = string length, m = pattern length, r = replacement length.
        *
        * @param pattern     The substring to search for.
        * @param replacement The string to substitute in place of the pattern.
        * @return Reference to the modified string, or unmodified if pattern not found.
        */
        String& Replace(const String& pattern, const String& replacement) {
            return Replace(pattern.Data(), replacement.Data());
        }

        /**
        * Replace the first occurrence of `pattern` with `replacement`.
        *
        * Uses Rabin-Karp (via find_index) to locate the pattern, then reconstructs the
        * string as: [prefix] + [replacement] + [suffix].
        *
        * Time complexity: O(n + m) average for the search, O(n + r) for reconstruction,
        *                  where n = string length, m = pattern length, r = replacement length.
        *
        * @param pattern     The substring to search for.
        * @param replacement The string to substitute in place of the pattern.
        * @return Reference to the modified string, or unmodified if pattern not found.
        */
        String& Replace(const char* pattern, const String& replacement) {
            return Replace(pattern, replacement.Data());
        }

        /**
        * Replace the first occurrence of `pattern` with `replacement`.
        *
        * Uses Rabin-Karp (via find_index) to locate the pattern, then reconstructs the
        * string as: [prefix] + [replacement] + [suffix].
        *
        * Time complexity: O(n + m) average for the search, O(n + r) for reconstruction,
        *                  where n = string length, m = pattern length, r = replacement length.
        *
        * @param pattern     The substring to search for.
        * @param replacement The string to substitute in place of the pattern.
        * @return Reference to the modified string, or unmodified if pattern not found.
        */
        String& Replace(const String& pattern, const char* replacement) {
            return Replace(pattern.Data(), replacement);
        }

        /**
        * Get the first occurrence of the given pattern within this string, returning a substring starting from the
        * found index and with the same length as the pattern, or an empty string if the pattern is not found.
        *
        * Time complexity: O(n + m) or O(n * m) in the worst case due to the need to search for the pattern within the
        * string, which involves iterating through the string and comparing characters for matches and then creating
        * a new substring if a match is found.
        * @param pattern
        * @return String
        */
        [[nodiscard]] String Find(const String& pattern) const {
            const int index = FindIndex(pattern.Data());

            if (index == -1) {
                return {};
            }

            return Substr(index, static_cast<int>(pattern.Size()));
        }

        /**
        * Get the first occurrence of the given pattern within this string, returning a substring starting from the
        * found index and with the same length as the pattern, or an empty string if the pattern is not found.
        *
        * Time complexity: O(n + m) or O(n * m) in the worst case due to the need to search for the pattern within the
        * string, which involves iterating through the string and comparing characters for matches and then creating
        * a new substring if a match is found.
        * @param pattern
        * @return String
        */
        [[nodiscard]] String Find(const char* pattern) const {
            const int index = FindIndex(pattern);

            if (index == -1) {
                return {};
            }

            return Substr(index, static_cast<int>(Strlen(pattern)));
        }

        /**
        * Get a substring of the current string starting from the given index and with the given length, or until the end of the string if the length is not provided or exceeds the remaining characters.
        *
        * Time complexity: O(n) due to the need to create a new string for the substring, which involves copying characters from the original string to the new string.
        * @param index
        * @param length
        * @return String
        */
        [[nodiscard]] String Substr(const int index, int length = -1) const {
            if (length == 0 || index >= m_size) {
                return {};
            }

            if (length <= -1 || length > m_size - index) {
                length = static_cast<int>(m_size) - index;
            }

            String result{};
            for (int i{}; i < length; ++i) {
                result += m_data[index + i];
            }

            return result;
        }

        /**
        * Get the size of the string, which is the number of characters currently stored in the string.
        *
        * Time complexity: O(1)
        * @return std::size_t
        */
        [[nodiscard]] std::size_t Size() const {
            return m_size;
        }

        /**
        * Get the capacity of the string, which refers to the total allocated memory for the string.
        *
        * Time complexity: O(1)
        * @return std::size_t
        */
        [[nodiscard]] std::size_t Capacity() const {
            return m_capacity;
        }

        /**
        * Get the raw string data as a C-style string.
        *
        * Time complexity: O(1)
        * @return const char*
        */
        [[nodiscard]] const char* Data() const {
            return m_data;
        }

        /**
        * Check if there are no characters in the string, which is determined by checking if the size is zero.
        *
        * Time complexity: O(1)
        * @return bool
        */
        [[nodiscard]] bool IsEmpty() const {
            return m_size == 0;
        }

        /**
        * Check if const char* is equal to our String
        *
        * Time complexity: O(n) due to the need to compare each character in the string for equality.
        * @param string
        * @return bool
        */
        [[nodiscard]] bool IsEqual(const char* string) const {
            if (m_size != Strlen(string)) {
                return false;
            }

            for (std::size_t i{}; i < m_size; ++i) {
                if (m_data[i] != string[i]) {
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
        [[nodiscard]] bool IsEqual(const String& string) const {
            if (m_size != string.Size()) {
                return false;
            }

            for (std::size_t i{}; i < m_size; ++i) {
                if (m_data[i] != string[i]) {
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
        [[nodiscard]] static std::size_t Strlen(const char* string) {
            std::size_t length{ 0 };

            while (string[length] != '\0') {
                ++length;
            }

            return length;
        }

    private:
        constexpr static std::size_t DEFAULT_CAPACITY = 10;

        char* m_data;
        std::size_t m_size;
        std::size_t m_capacity;

        /**
        * Assign characters to our C-style string, if the we don't have enough memory allocated, we will resize the array and reserve enough memory to fit the new string.
        *
        * Time complexity: O(n + m) when we need to resize, else it is O(n)
        * @param string
        */
        void Assign(const char* string) {
            const std::size_t stringLength = Strlen(string);

            if (stringLength + 1 > m_capacity) {
                const std::size_t newCapacity = (m_capacity * 2 > stringLength + 1)
                    ? m_capacity * 2
                    : stringLength + 1;

                Resize(newCapacity);
            }

            for (std::size_t i{}; i < stringLength; ++i) {
                m_data[i] = string[i];
            }

            m_size = stringLength;
            m_data[m_size] = '\0';
        }

        /**
        * Append characters to the end of our C-style string, if we don't have enough memory allocated, we will resize the array and reserve enough memory to fir the new string.
        *
        * Time complexity: O(n + m) when we need to resize, else it is O(n)
        * @param string
        */
        void Append(const char* string) {
            const std::size_t stringLength = Strlen(string);
            const std::size_t newSize = m_size + Strlen(string);

            if (newSize + 1 > m_capacity) {
                Resize(m_capacity * 2 + stringLength);
            }

            for (std::size_t i{}; i < stringLength; ++i) {
                new (&m_data[m_size + i]) char(string[i]);
            }

            m_size = newSize;
            m_data[m_size] = '\0';
        }

        /**
        * Append a single character to the end of our C-style string.
        *
        * Time complexity: O(n) when we need to resize, else it would be O(1)
        * @param character
        */
        void Append(const char character) {
            if (m_size + 1 > m_capacity) {
                Resize(m_capacity * 2 + 1);
            }

            new (&m_data[m_size]) char(character);
            ++m_size;

            m_data[m_size] = '\0';
        }

        /**
        * Resize the array to the specified capacity, moving existing elements to the new memory location.
        *
        * Time complexity: O(n) due to the need to copy each character from the old array to the new array, and then deallocate the old array.
        * @param capacity
        */
        void Resize(const std::size_t capacity) {
            auto* newData = static_cast<char*>(::operator new(capacity * sizeof(char)));

            const std::size_t movableCapacity = (capacity < m_size)
                ? capacity
                : m_size;

            for (std::size_t i{}; i < movableCapacity; ++i) {
                new (&newData[i]) char(m_data[i]);
            }

            ::operator delete(m_data);

            m_data = newData;
            m_capacity = capacity;
        }

        /**
        * Find the first occurrence of the given pattern within this string using the Rabin-Karp algorithm.
        *
        * The algorithm works by computing a rolling hash over a sliding window of the text:
        * 1. Compute the hash of the pattern and the first window of the text (both of length m).
        * 2. Slide the window one character at a time, updating the hash in O(1) by removing the
        *    contribution of the outgoing character and adding the incoming character.
        * 3. When the window hash matches the pattern hash, verify character-by-character to
        *    guard against hash collisions.
        *
        * Uses base-256 (one value per byte) and a large prime modulus (10^9 + 7) to minimize
        * collision probability. The precomputed value `hash` = base^(m-1) mod m allows the
        * leading character to be stripped from the rolling hash in constant time.
        *
        * Time complexity:  O(n + m) average case; O(n * m) worst case (many hash collisions).
        *
        * @param pattern The string to search for.
        * @return A copy of the matched substring starting at the first occurrence, or an empty
        *         String if the pattern is not found.
        *
        * @see https://en.wikipedia.org/wiki/Rabin%E2%80%93Karp_algorithm
        */
        [[nodiscard]] int FindIndex(const char* pattern) const {
            const std::size_t patternSize = Strlen(pattern);

            if (patternSize == 0 || patternSize > m_size)
                return -1;

            constexpr std::size_t base{ 256 }, mod{ 1000000007 };
            std::size_t hash{ 1 }, patternHash{ 0 }, textWindowHash{ 0 };

            for (std::size_t i{}; i < patternSize - 1; ++i) {
                hash = (hash * base) % mod;
            }

            for (std::size_t i{}; i < patternSize; ++i) {
                patternHash = (base * patternHash + pattern[i]) % mod;
                textWindowHash = (base * textWindowHash + m_data[i]) % mod;
            }

            for (std::size_t i{}; i <= m_size - patternSize; ++i) {
                if (patternHash == textWindowHash) {
                    std::size_t j{};

                    while (j < patternSize && m_data[i + j] == pattern[j]) {
                        ++j;
                    }

                    if (j == patternSize) {
                        return static_cast<int>(i);
                    }
                }

                if (i < m_size - patternSize) {
                    textWindowHash = (base * (textWindowHash + mod - (m_data[i] * hash) % mod) + m_data[i + patternSize]) % mod;
                }
            }

            return -1;
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
        for (std::size_t i{}; i < string.Size(); ++i) {
            os << string[i];
        }

        return os;
    }
}


#endif
