//
// Created by Dorza on 5/1/2026.
//

#include <gtest/gtest.h>
#include "../include/String.h"

constexpr std::size_t ZERO{ 0 };
constexpr std::size_t DEFAULT_CAPACITY{ 10 };

TEST(StringTest, TestDefaultConstructor) {
    const dsa::String str{};

    EXPECT_EQ(str.Size(), ZERO);
    EXPECT_EQ(str.Capacity(), DEFAULT_CAPACITY);
}

TEST(StringTest, TestCStringConstructor) {
    const char* c_string{ "Hello world!" };
    const dsa::String str{ c_string };

    EXPECT_EQ(str.Size(), dsa::String::Strlen(c_string));
    EXPECT_EQ(str.Capacity(), dsa::String::Strlen(c_string) + DEFAULT_CAPACITY);
    EXPECT_EQ(str, c_string);
}

TEST(StringTest, TestCopyConstructor) {
    const dsa::String str{ "Hello world!" };
    const dsa::String str2{ str };

    EXPECT_EQ(str2, str);
    EXPECT_EQ(str2.Size(), str.Size());
    EXPECT_EQ(str2.Capacity(), str.Capacity());
}

TEST(StringTest, TestMoveConstructor) {
    dsa::String str{ "Hello world!" };
    const dsa::String str2{ std::move(str) };

    EXPECT_NE(str, str2);
    EXPECT_NE(str.Capacity(), str2.Capacity());
    EXPECT_NE(str.Size(), str2.Size());
}

TEST(StringTest, TestCStringAssignmentOperator) {
    dsa::String str{ "Hello world!" };

    str = "Hello other world!";

    EXPECT_EQ(str, "Hello other world!");
    EXPECT_EQ(str.Size(), dsa::String::Strlen("Hello other world!"));
    EXPECT_EQ(str.Capacity(), dsa::String::Strlen("Hello world!") + DEFAULT_CAPACITY);
}

TEST(StringTest, TestCopyAssignmentOperator) {
    dsa::String str{"Hello world!"};
    const dsa::String str2{"Hello other world!"};

    str = str2;

    EXPECT_EQ(str, str2);
    EXPECT_EQ(str.Size(), str2.Size());
    EXPECT_NE(str.Capacity(), str2.Capacity());
}

TEST(StringTest, TestMoveAssignmentOperator) {
    dsa::String str{"Hello world!"};
    dsa::String str2{"Hello other world!"};

    str = std::move(str2);

    EXPECT_NE(str, str2);
    EXPECT_NE(str.Capacity(), str2.Capacity());
    EXPECT_NE(str.Size(), str2.Size());
}

TEST(StringTest, TestIndexOperator) {
    const dsa::String str{"hello"};

    EXPECT_EQ(str[0], 'h');
    EXPECT_EQ(str[1], 'e');
    EXPECT_EQ(str[2], 'l');
    EXPECT_EQ(str[3], 'l');
    EXPECT_EQ(str[4], 'o');
}

TEST(StringTest, TestAppendOperator) {
    dsa::String str{"Hello"};
    const char* c_string = "world!";
    const dsa::String str2{"world!"};

    str += 'w';
    str += 'o';
    str += 'r';
    str += 'l';
    str += 'd';
    str += '!';

    EXPECT_EQ(str, "Helloworld!");
    EXPECT_EQ(str.Size(), dsa::String::Strlen("Helloworld!"));

    str = "Hello";
    str += c_string;

    EXPECT_EQ(str, "Helloworld!");
    EXPECT_EQ(str.Size(), dsa::String::Strlen("Helloworld!"));

    str = "Hello";
    str += str2;

    EXPECT_EQ(str, "Helloworld!");
    EXPECT_EQ(str.Size(), dsa::String::Strlen("Helloworld!"));
}

TEST(StringTest, TestIsEqualOperator) {
    const dsa::String str{ "Hello" };
    const dsa::String str2{ "Hello" };
    const char* c_string = "Hello";

    EXPECT_EQ(str, c_string);
    EXPECT_EQ(str, str2);
}

TEST(StringTest, TestIsNotEqualOperator) {
    const char* c_string = "Hello world!";
    const dsa::String str{ "Hello" };
    const dsa::String str2{ "Hello world!" };

    EXPECT_FALSE(str == c_string);
    EXPECT_NE(str, str2);
}

TEST(StringTest, TestBeginIterator) {
    const dsa::String str{ "Hello" };

    EXPECT_EQ(*str.Begin(), 'H');
    EXPECT_EQ(*(str.Begin() + 1), 'e');
    EXPECT_EQ(*(str.Begin() + 2), 'l');
    EXPECT_EQ(*(str.Begin() + 3), 'l');
    EXPECT_EQ(*(str.Begin() + 4), 'o');
}

TEST(StringTest, TestEndIterator) {
    const dsa::String str{ "Hello" };

    EXPECT_EQ(*str.End(), '\0');
    EXPECT_EQ(*(str.End() - 1), 'o');
    EXPECT_EQ(*(str.End() - 2), 'l');
    EXPECT_EQ(*(str.End() - 3), 'l');
    EXPECT_EQ(*(str.End() - 4), 'e');
    EXPECT_EQ(*(str.End() - 5), 'H');
}

TEST(StringTest, TestAt) {
    const dsa::String str{ "Hello" };

    EXPECT_EQ(str.At(0), 'H');
    EXPECT_EQ(str.At(1), 'e');
    EXPECT_EQ(str.At(2), 'l');
    EXPECT_EQ(str.At(3), 'l');
    EXPECT_EQ(str.At(4), 'o');
}

TEST(StringTest, TestBack) {
    const dsa::String str{ "Hello" };

    EXPECT_EQ(str.Back(), 'o');
}

TEST(StringTest, TestFront) {
    const dsa::String str{ "Hello" };

    EXPECT_EQ(str.Front(), 'H');
}

TEST(StringTest, TestReplace) {
    dsa::String str{ "Hello world!" };
    const dsa::String str2{ "world" };
    const dsa::String str3{ "other world!" };

    str.Replace(str2, str3);

    EXPECT_EQ(str, "Hello other world!!");

    str = "Hello world!";
    str.Replace("world", "other world!");

    EXPECT_EQ(str, "Hello other world!!");

    str = "Hello world!";
    str.Replace(str2, "other world!");

    EXPECT_EQ(str, "Hello other world!!");

    str = "Hello world!";
    str.Replace("world", str3);

    EXPECT_EQ(str, "Hello other world!!");
}

TEST(StringTest, TestFind) {
    const dsa::String str{ "Hello world!" };
    const dsa::String str2{ "world" };
    const char* c_string = "world";

    EXPECT_EQ(str.Find(str2), "world");
    EXPECT_EQ(str.Find(c_string), "world");
}

TEST(StringTest, TestSubstr) {
    const dsa::String str{ "Hello world!" };

    EXPECT_EQ(str.Substr(0, 5), "Hello");
    EXPECT_EQ(str.Substr(5, 5), " worl");
    EXPECT_EQ(str.Substr(10, 5), "d!");
}

TEST(StringTest, TestSize) {
    const dsa::String str{ "Hello world!" };

    EXPECT_EQ(str.Size(), dsa::String::Strlen("Hello world!"));
}

TEST(StringTest, TestCapacity) {
    const dsa::String str{ "Hello world!" };

    EXPECT_EQ(str.Capacity(), dsa::String::Strlen("Hello world!") + DEFAULT_CAPACITY);
}

TEST(StringTest, TestIsEmpty) {
    const dsa::String str{};

    EXPECT_EQ(str.IsEmpty(), true);

    const dsa::String str2{ "Hello world!" };

    EXPECT_EQ(str2.IsEmpty(), false);
}

TEST(StringTest, TestIsEqual) {
    const dsa::String str{ "Hello world!" };
    const dsa::String str2{ "Hello world!" };

    EXPECT_EQ(str.IsEqual(str2), true);
    EXPECT_EQ(str.IsEqual("Hello"), false);
}

TEST(StringTest, TestStrlen) {
    constexpr std::size_t length = 12;

    EXPECT_EQ(dsa::String::Strlen("Hello world!"), length);
}
