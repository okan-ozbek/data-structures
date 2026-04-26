//
// Created by Dorza on 4/19/2026.
//

#ifndef TESTSTRING_H
#define TESTSTRING_H

#include "../utils/TempAssert.h"
#include "../String.h"

class TestString {
public:
    TestString() {
        run();
    }

    ~TestString() = default;

private:
    std::size_t TEST_DEFAULT_CAPACITY = 10;

    void TestDefaultConstructor() {
        const dsa::String str{};
        constexpr std::size_t zero{0};

        assert_true(str.Size(), zero, "Size is not expected size of 0");
        assert_true(str.Capacity(), TEST_DEFAULT_CAPACITY, "Capacity is not expected capacity of 10");
    }

    void TestCStringConstructor() {
        const char* c_string = "Hello world!";
        const dsa::String str{c_string};

        assert_true(str.Size(), dsa::String::Strlen(c_string), "Size is not expected size of strlen(c_string)");
        assert_true(str.Capacity(), dsa::String::Strlen(c_string) + TEST_DEFAULT_CAPACITY, "Capacity is not expected capacity of 10");
        assert_true(str == c_string, "dsa::String is not equal to c_string");
    }

    void TestCopyConstructor() {
        const dsa::String str{"Hello world!"};
        const dsa::String str2{str};

        assert_true(str2 == str, "dsa::String is not equal to str");
        assert_true(str2.Size(), str.Size(), "Size is not equal to str size");
        assert_true(str2.Capacity(), str.Capacity(), "Capacity is not equal to str capacity");
    }

    void TestMoveConstructor() {
        dsa::String str{"Hello world!"};
        const dsa::String str2{std::move(str)};

        assert_true(str != str2, "dsa::String is equal to str");
        assert_true(str.Capacity() != str2.Capacity(), "Capacity is equal to str capacity");
        assert_true(str.Size() != str2.Size(), "Size is equal to str size");
    }

    void TestCStringAssignmentOperator() {
        dsa::String str{"Hello world!"};

        str = "Hello other world!";

        assert_true(str == "Hello other world!", "dsa::String is not equal to Hello other world!");
        assert_true(str.Size(), dsa::String::Strlen("Hello other world!"), "Size is not equal to strlen(Hello other world!)");
        assert_true(str.Capacity(), dsa::String::Strlen("Hello world!") + TEST_DEFAULT_CAPACITY, "Capacity is not equal to strlen(Hello world!) + TEST_DEFAULT_CAPACITY");
    }

    void TestCopyAssignmentOperator() {
        dsa::String str{"Hello world!"};
        const dsa::String str2{"Hello other world!"};

        str = str2;

        assert_true(str == str2, "dsa::String is not equal to str2");
        assert_true(str.Size(), str2.Size(), "Size is not equal to str2 size");
        assert_true(str.Capacity() != str2.Capacity(), "Capacity is equal to str2 capacity");
    }

    void TestMoveAssignmentOperator() {
        dsa::String str{"Hello world!"};
        dsa::String str2{"Hello other world!"};

        str = std::move(str2);

        assert_true(str != str2, "dsa::String is equal to str2");
        assert_true(str.Capacity() != str2.Capacity(), "Capacity is equal to str2 capacity");
        assert_true(str.Size() != str2.Size(), "Size is equal to str2 size");
    }

    void TestIndexOperator() {
        const dsa::String str{"hello"};

        assert_true(str[0], 'h', "Index operator value is not equal to value");
        assert_true(str[1], 'e', "Index operator value is not equal to value");
        assert_true(str[2], 'l', "Index operator value is not equal to value");
        assert_true(str[3], 'l', "Index operator value is not equal to value");
        assert_true(str[4], 'o', "Index operator value is not equal to value");
    }

    void TestAppendOperator() {
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
        assert_true(str.Size(), dsa::String::Strlen("Helloworld!"), "Size is not equal to strlen(Hello world!)");

        str = "Hello";
        str += c_string;

        assert_true(str == "Helloworld!", "dsa::String is not equal to Hello world!");
        assert_true(str.Size(), dsa::String::Strlen("Helloworld!"), "Size is not equal to strlen(Hello world!)");

        str = "Hello";
        str += str2;

        assert_true(str == "Helloworld!", "dsa::String is not equal to Hello world!");
        assert_true(str.Size(), dsa::String::Strlen("Helloworld!"), "Size is not equal to strlen(Hello world!)");
    }

    void TestIsEqualOperator() {
        const dsa::String str{"Hello"};
        const char* c_string = "Hello";
        const dsa::String str2{"Hello"};

        assert_true(str == c_string, "dsa::String is not equal to c_string");
        assert_true(str == str2, "dsa::String is not equal to str2");
    }

    void TestIsNotEqualOperator() {
        const dsa::String str{"Hello"};
        const char* c_string = "Hello world!";
        const dsa::String str2{"Hello world!"};

        assert_true(str != c_string, "dsa::String is equal to c_string");
        assert_true(str != str2, "dsa::String is equal to str2");
    }

    void TestBeginIterator() {
        const dsa::String str{"Hello"};

        assert_true(*str.Begin(), 'H', "Begin iterator value is not equal to value");
        assert_true(*(str.Begin() + 1), 'e', "Begin iterator value is not equal to value");
        assert_true(*(str.Begin() + 2), 'l', "Begin iterator value is not equal to value");
        assert_true(*(str.Begin() + 3), 'l', "Begin iterator value is not equal to value");
        assert_true(*(str.Begin() + 4), 'o', "Begin iterator value is not equal to value");
    }

    void TestEndIterator() {
        const dsa::String str{"Hello"};

        assert_true(*(str.End()), '\0', "End iterator value is not equal to value");
        assert_true(*(str.End() - 1), 'o', "End iterator value is not equal to value");
        assert_true(*(str.End() - 2), 'l', "End iterator value is not equal to value");
        assert_true(*(str.End() - 3), 'l', "End iterator value is not equal to value");
        assert_true(*(str.End() - 4), 'e', "End iterator value is not equal to value");
        assert_true(*(str.End() - 5), 'H', "End iterator value is not equal to value");
    }

    void TestAt() {
        const dsa::String str{"Hello"};

        assert_true(str.At(0), 'H', "At operator value is not equal to value");
        assert_true(str.At(1), 'e', "At operator value is not equal to value");
        assert_true(str.At(2), 'l', "At operator value is not equal to value");
        assert_true(str.At(3), 'l', "At operator value is not equal to value");
        assert_true(str.At(4), 'o', "At operator value is not equal to value");
    }

    void TestBack() {
        const dsa::String str{"Hello"};

        assert_true(str.Back(), 'o', "Back operator value is not equal to value");
    }

    void TestFront() {
        const dsa::String str{"Hello"};

        assert_true(str.Front(), 'H', "Front operator value is not equal to value");
    }

    void TestReplace() {
        dsa::String str{"Hello world!"};
        const dsa::String str2{"world"};
        const dsa::String str3{"other world!"};

        str.Replace(str2, str3);
        assert_true(str == "Hello other world!!", "Replace operator value is not equal to value");

        str = "Hello world!";

        str.Replace("world", "other world!");
        assert_true(str == "Hello other world!!", "Replace operator value is not equal to value");

        str = "Hello world!";

        str.Replace(str2, "other world!");
        assert_true(str == "Hello other world!!", "Replace operator value is not equal to value");

        str = "Hello world!";

        str.Replace("world", str3);
        assert_true(str == "Hello other world!!", "Replace operator value is not equal to value");
    }

    void TestFind() {
        const dsa::String str{"Hello world!"};
        const dsa::String str2{"world"};
        const char* c_string = "world";

        assert_true(str.Find(str2) == "world", "Find operator value is not equal to value");
        assert_true(str.Find(c_string) == "world", "Find operator value is not equal to value");
    }

    void TestSubstr() {
        const dsa::String str{"Hello world!"};

        assert_true(str.Substr(0, 5) == "Hello", "Substr operator 1 value is not equal to value");
        assert_true(str.Substr(5, 5) == " worl", "Substr operator 2 value is not equal to value");
        assert_true(str.Substr(10, 5) == "d!", "Substr operator 3 value is not equal to value");
    }

    void TestSize() {
        const dsa::String str{"Hello world!"};

        assert_true(str.Size(), dsa::String::Strlen("Hello world!"), "Size is not equal to strlen(Hello world!)");
    }

    void TestCapacity() {
        const dsa::String str{"Hello world!"};

        assert_true(str.Capacity(), dsa::String::Strlen("Hello world!") + TEST_DEFAULT_CAPACITY, "Capacity is not equal to strlen(Hello world!) + TEST_DEFAULT_CAPACITY");
    }

    void TestIsEmpty() {
        const dsa::String str{};

        assert_true(str.IsEmpty(), true, "Is empty is not equal to true");

        const dsa::String str2{"Hello world!"};

        assert_true(str2.IsEmpty(), false, "Is empty is not equal to false");
    }

    void TestIsEqual() {
        const dsa::String str{"Hello world!"};
        const dsa::String str2{"Hello world!"};

        assert_true(str.IsEqual(str2), true, "Is equal is not equal to true");
        assert_true(str.IsEqual("Hello"), false, "Is equal is not equal to false");
    }

    void TestStrlen() {
        const dsa::String str{"Hello world!"};

        constexpr std::size_t length = 12;

        assert_true(dsa::String::Strlen("Hello world!"), length, "Strlen is not equal to 12");
    }

    void run() {
        // Constructors
        TestDefaultConstructor();
        TestCStringConstructor();
        TestCopyConstructor();
        TestMoveConstructor();

        // Operators
        TestCStringAssignmentOperator();
        TestCopyAssignmentOperator();
        TestMoveAssignmentOperator();
        TestIndexOperator();
        TestAppendOperator();
        TestIsEqualOperator();
        TestIsNotEqualOperator();

        // Iterators
        TestBeginIterator();
        TestEndIterator();

        // Indexing
        TestAt();
        TestBack();
        TestFront();

        // Searching
        TestReplace();
        TestSubstr();

        // Memory allocation
        TestSize();
        TestCapacity();

        // Helpers
        TestIsEmpty();
        TestIsEqual();
    }
};

#endif // TESTSTRING_H

