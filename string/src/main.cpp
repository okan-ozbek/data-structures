//
// Created by Okan Ozbek on 4/9/2026.
//

#include <assert.h>
#include <iostream>
#include <chrono>
#include "../include/String.h"

constexpr std::size_t TEST_DEFAULT_CAPACITY = 10;
static int TEST_ERRORS = 0;
static int TEST_RAN = 0;

class Timer {
public:
    Timer() {
        start_time_ = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        stop();
    }

    void stop() const {
        const auto end_time_point = std::chrono::high_resolution_clock::now();

        const auto start_time = std::chrono::time_point_cast<std::chrono::microseconds>(start_time_).time_since_epoch().count();
        const auto end_time = std::chrono::time_point_cast<std::chrono::microseconds>(end_time_point).time_since_epoch().count();

        const auto duration = end_time - start_time;
        std::cout << "Completion in: " << duration << "us, (" << duration * 0.001 << "ms)." << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
};

template<typename T>
void assert_true(T a, T b, const std::string& error_message) {
    if (a != b) {
        std::cout << "Assertion failed: " << error_message << std::endl;
        ++TEST_ERRORS;
    }

    ++TEST_RAN;
}

void assert_true(const bool a, const std::string& error_message) {
    if (!a) {
        std::cout << "Assertion failed: " << error_message << std::endl;
        ++TEST_ERRORS;
    }

    ++TEST_RAN;
}

void test_default_constructor() {
    const dsa::String str{};
    constexpr std::size_t zero{0};

    assert_true(str.size(), zero, "Size is not expected size of 0");
    assert_true(str.capacity(), TEST_DEFAULT_CAPACITY, "Capacity is not expected capacity of 10");
}

void test_c_string_constructor() {
    const char* c_string = "Hello world!";
    const dsa::String str{c_string};

    assert_true(str.size(), dsa::String::strlen(c_string), "Size is not expected size of strlen(c_string)");
    assert_true(str.capacity(), dsa::String::strlen(c_string) + TEST_DEFAULT_CAPACITY, "Capacity is not expected capacity of 10");
    assert_true(str == c_string, "dsa::String is not equal to c_string");
}

void test_copy_constructor() {
    const dsa::String str{"Hello world!"};
    const dsa::String str2{str};

    assert_true(str2 == str, "dsa::String is not equal to str");
    assert_true(str2.size(), str.size(), "Size is not equal to str size");
    assert_true(str2.capacity(), str.capacity(), "Capacity is not equal to str capacity");
}

void test_move_constructor() {
    dsa::String str{"Hello world!"};
    const dsa::String str2{std::move(str)};

    assert_true(str != str2, "dsa::String is equal to str");
    assert_true(str.capacity() != str2.capacity(), "Capacity is equal to str capacity");
    assert_true(str.size() != str2.size(), "Size is equal to str size");
}

void test_c_string_assignment_operator() {
    dsa::String str{"Hello world!"};

    str = "Hello other world!";

    assert_true(str == "Hello other world!", "dsa::String is not equal to Hello other world!");
    assert_true(str.size(), dsa::String::strlen("Hello other world!"), "Size is not equal to strlen(Hello other world!)");
    assert_true(str.capacity(), dsa::String::strlen("Hello world!") + TEST_DEFAULT_CAPACITY, "Capacity is not equal to strlen(Hello world!) + TEST_DEFAULT_CAPACITY");
}

void test_copy_assignment_operator() {
    dsa::String str{"Hello world!"};
    const dsa::String str2{"Hello other world!"};

    str = str2;

    assert_true(str == str2, "dsa::String is not equal to str2");
    assert_true(str.size(), str2.size(), "Size is not equal to str2 size");
    assert_true(str.capacity() != str2.capacity(), "Capacity is equal to str2 capacity");
}

void test_move_assignment_operator() {
    dsa::String str{"Hello world!"};
    dsa::String str2{"Hello other world!"};

    str = std::move(str2);

    assert_true(str != str2, "dsa::String is equal to str2");
    assert_true(str.capacity() != str2.capacity(), "Capacity is equal to str2 capacity");
    assert_true(str.size() != str2.size(), "Size is equal to str2 size");
}

void test_index_operator() {
    const dsa::String str{"hello"};

    assert_true(str[0], 'h', "Index operator value is not equal to value");
    assert_true(str[1], 'e', "Index operator value is not equal to value");
    assert_true(str[2], 'l', "Index operator value is not equal to value");
    assert_true(str[3], 'l', "Index operator value is not equal to value");
    assert_true(str[4], 'o', "Index operator value is not equal to value");
}

void test_append_operator() {
    dsa::String str{"Hello"};
    const char* c_string = "world!";
    const dsa::String str2{"world!"};

    str += 'w';
    str += 'o';
    str += 'r';
    str += 'l';
    str += 'd';
    str += '!';

    assert_true(str == "Helloworld!", "dsa::String is not equal to Hello world!");
    assert_true(str.size(), dsa::String::strlen("Helloworld!"), "Size is not equal to strlen(Hello world!)");

    str = "Hello";
    str += c_string;

    assert_true(str == "Helloworld!", "dsa::String is not equal to Hello world!");
    assert_true(str.size(), dsa::String::strlen("Helloworld!"), "Size is not equal to strlen(Hello world!)");

    str = "Hello";
    str += str2;

    assert_true(str == "Helloworld!", "dsa::String is not equal to Hello world!");
    assert_true(str.size(), dsa::String::strlen("Helloworld!"), "Size is not equal to strlen(Hello world!)");
}

void test_is_equal_operator() {
    const dsa::String str{"Hello"};
    const char* c_string = "Hello";
    const dsa::String str2{"Hello"};

    assert_true(str == c_string, "dsa::String is not equal to c_string");
    assert_true(str == str2, "dsa::String is not equal to str2");
}

void test_is_not_equal_operator() {
    const dsa::String str{"Hello"};
    const char* c_string = "Hello world!";
    const dsa::String str2{"Hello world!"};

    assert_true(str != c_string, "dsa::String is equal to c_string");
    assert_true(str != str2, "dsa::String is equal to str2");
}

void test_begin_iterator() {
    const dsa::String str{"Hello"};

    assert_true(*str.begin(), 'H', "Begin iterator value is not equal to value");
    assert_true(*(str.begin() + 1), 'e', "Begin iterator value is not equal to value");
    assert_true(*(str.begin() + 2), 'l', "Begin iterator value is not equal to value");
    assert_true(*(str.begin() + 3), 'l', "Begin iterator value is not equal to value");
    assert_true(*(str.begin() + 4), 'o', "Begin iterator value is not equal to value");
}

void test_end_iterator() {
    const dsa::String str{"Hello"};

    assert_true(*(str.end()), '\0', "End iterator value is not equal to value");
    assert_true(*(str.end() - 1), 'o', "End iterator value is not equal to value");
    assert_true(*(str.end() - 2), 'l', "End iterator value is not equal to value");
    assert_true(*(str.end() - 3), 'l', "End iterator value is not equal to value");
    assert_true(*(str.end() - 4), 'e', "End iterator value is not equal to value");
    assert_true(*(str.end() - 5), 'H', "End iterator value is not equal to value");
}

void test_at() {
    const dsa::String str{"Hello"};

    assert_true(str.at(0), 'H', "At operator value is not equal to value");
    assert_true(str.at(1), 'e', "At operator value is not equal to value");
    assert_true(str.at(2), 'l', "At operator value is not equal to value");
    assert_true(str.at(3), 'l', "At operator value is not equal to value");
    assert_true(str.at(4), 'o', "At operator value is not equal to value");
}

void test_back() {
    const dsa::String str{"Hello"};

    assert_true(str.back(), 'o', "Back operator value is not equal to value");
}

void test_front() {
    const dsa::String str{"Hello"};

    assert_true(str.front(), 'H', "Front operator value is not equal to value");
}

void test_replace() {
    dsa::String str{"Hello world!"};
    const dsa::String str2{"world"};
    const dsa::String str3{"other world!"};

    str.replace(str2, str3);
    assert_true(str == "Hello other world!!", "Replace operator value is not equal to value");

    str = "Hello world!";

    str.replace("world", "other world!");
    assert_true(str == "Hello other world!!", "Replace operator value is not equal to value");

    str = "Hello world!";

    str.replace(str2, "other world!");
    assert_true(str == "Hello other world!!", "Replace operator value is not equal to value");

    str = "Hello world!";

    str.replace("world", str3);
    assert_true(str == "Hello other world!!", "Replace operator value is not equal to value");
}

void test_find() {
    const dsa::String str{"Hello world!"};
    const dsa::String str2{"world"};
    const char* c_string = "world";

    assert_true(str.find(str2) == "world", "Find operator value is not equal to value");
    assert_true(str.find(c_string) == "world", "Find operator value is not equal to value");
}

void test_substr() {
    const dsa::String str{"Hello world!"};

    assert_true(str.substr(0, 5) == "Hello", "Substr operator 1 value is not equal to value");
    assert_true(str.substr(5, 5) == " worl", "Substr operator 2 value is not equal to value");
    assert_true(str.substr(10, 5) == "d!", "Substr operator 3 value is not equal to value");
}

void test_size() {
    const dsa::String str{"Hello world!"};

    assert_true(str.size(), dsa::String::strlen("Hello world!"), "Size is not equal to strlen(Hello world!)");
}

void test_capacity() {
    const dsa::String str{"Hello world!"};

    assert_true(str.capacity(), dsa::String::strlen("Hello world!") + TEST_DEFAULT_CAPACITY, "Capacity is not equal to strlen(Hello world!) + TEST_DEFAULT_CAPACITY");
}

void test_is_empty() {
    const dsa::String str{};

    assert_true(str.is_empty(), true, "Is empty is not equal to true");

    const dsa::String str2{"Hello world!"};

    assert_true(str2.is_empty(), false, "Is empty is not equal to false");
}

void test_is_equal() {
    const dsa::String str{"Hello world!"};
    const dsa::String str2{"Hello world!"};

    assert_true(str.is_equal(str2), true, "Is equal is not equal to true");
    assert_true(str.is_equal("Hello"), false, "Is equal is not equal to false");
}

void test_strlen() {
    const dsa::String str{"Hello world!"};

    constexpr std::size_t length = 12;

    assert_true(dsa::String::strlen("Hello world!"), length, "Strlen is not equal to 12");
}

int main() {
    {
        Timer timer{};

        // Constructors
        test_default_constructor();
        test_c_string_constructor();
        test_copy_constructor();
        test_move_constructor();

        // Operators
        test_c_string_assignment_operator();
        test_copy_assignment_operator();
        test_move_assignment_operator();
        test_index_operator();
        test_append_operator();
        test_is_equal_operator();
        test_is_not_equal_operator();

        // Iterators
        test_begin_iterator();
        test_end_iterator();

        // Indexing
        test_at();
        test_back();
        test_front();

        // Searching
        test_replace();
        test_substr();

        // Memory allocation
        test_size();
        test_capacity();

        // Helpers
        test_is_empty();
        test_is_equal();
    }


    std::cout << "Successful test(s): " << TEST_RAN - TEST_ERRORS << ", error(s): " << TEST_ERRORS << "\n";

    std::cout << "--- END OF BENCHMARK ---" << "\n";

    std::cout << "Sizeof std::string: " << sizeof(std::string) << ", sizeof dsa::String: " << sizeof(dsa::String) << "\n";

    return 0;
}
