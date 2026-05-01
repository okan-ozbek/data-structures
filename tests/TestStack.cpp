//
// Created by Dorza on 5/1/2026.
//

#include <gtest/gtest.h>
#include "../include/Stack.h"

constexpr std::size_t ZERO{ 0 };

struct Vector2D {
    int x;
    int y;

    Vector2D() : x{0}, y{0} {}
    explicit Vector2D(const int scalar) : x{scalar}, y{scalar} {}
    explicit Vector2D(const int x, const int y) : x{x}, y{y} {}
};

TEST(StackTest, TestDefaultConstructor) {
    const dsa::Stack<int> q{};

    EXPECT_TRUE(q.IsEmpty());
    EXPECT_EQ(q.Size(), ZERO);
}

TEST(StackTest, TestCopyConstructor) {
    dsa::Stack<int> q{};

    q.Push(5);
    q.Push(10);

    const dsa::Stack q2{q};

    EXPECT_FALSE(q.IsEmpty());
    EXPECT_FALSE(q2.IsEmpty());
    EXPECT_EQ(q.Front(), 5);
    EXPECT_EQ(q.Back(), 10);
    EXPECT_EQ(q2.Front(), 5);
    EXPECT_EQ(q2.Back(), 10);
}

TEST(StackTest, TestMoveConstructor) {
    dsa::Stack<int> q{};

    q.Push(5);
    q.Push(10);

    const dsa::Stack q2{std::move(q)};

    EXPECT_TRUE(q.IsEmpty());
    EXPECT_EQ(q.Size(), ZERO);
    EXPECT_FALSE(q2.IsEmpty());
    EXPECT_EQ(q2.Size(), 2);
    EXPECT_EQ(q2.Front(), 5);
    EXPECT_EQ(q2.Back(), 10);
}

TEST(StackTest, TestCopyAssignmentOperator) {
    dsa::Stack<int> q{};

    q.Push(5);
    q.Push(10);

    dsa::Stack<int> q2{};
    q2 = q;

    EXPECT_FALSE(q.IsEmpty());
    EXPECT_FALSE(q2.IsEmpty());
    EXPECT_EQ(q.Front(), 5);
    EXPECT_EQ(q.Back(), 10);
    EXPECT_EQ(q2.Front(), 5);
    EXPECT_EQ(q2.Back(), 10);
}

TEST(StackTest, TestMoveAssignmentOperator) {
    dsa::Stack<int> q{};

    q.Push(5);
    q.Push(10);

    dsa::Stack<int> q2{};
    q2 = std::move(q);

    EXPECT_TRUE(q.IsEmpty());
    EXPECT_EQ(q.Size(), ZERO);
    EXPECT_FALSE(q2.IsEmpty());
    EXPECT_EQ(q2.Size(), 2);
    EXPECT_EQ(q2.Front(), 5);
    EXPECT_EQ(q2.Back(), 10);
}

TEST(StackTest, TestIsEmpty) {
    dsa::Stack<int> q{};

    EXPECT_TRUE(q.IsEmpty());

    q.Push(5);

    EXPECT_FALSE(q.IsEmpty());
}

TEST(StackTest, TestSize) {
    dsa::Stack<int> q{};

    EXPECT_EQ(q.Size(), ZERO);

    q.Push(5);
    q.Push(10);

    EXPECT_EQ(q.Size(), 2);

    q.Pop();

    EXPECT_EQ(q.Size(), 1);
}

TEST(StackTest, TestFront) {
    dsa::Stack<int> q{};

    q.Push(5);
    q.Push(10);

    EXPECT_EQ(q.Front(), 5);

    q.Pop();

    EXPECT_EQ(q.Front(), 5);
}

TEST(StackTest, TestBack) {
    dsa::Stack<int> q{};

    q.Push(5);

    EXPECT_EQ(q.Back(), 5);

    q.Push(10);

    EXPECT_EQ(q.Back(), 10);
}

TEST(StackTest, TestPush) {
    dsa::Stack<int> q{};

    q.Push(5);
    q.Push(10);

    EXPECT_EQ(q.Front(), 5);
    EXPECT_EQ(q.Back(), 10);
    EXPECT_EQ(q.Size(), 2);

    q.Push(15);

    EXPECT_EQ(q.Size(), 3);

    q.Pop();

    EXPECT_EQ(q.Size(), 2);
}

TEST(StackTest, TestEmplace) {
    dsa::Stack<Vector2D> q{};

    q.Emplace(5, 10);
    q.Emplace(5);
    q.Emplace();

    EXPECT_EQ(q.Front().x, 5);
    EXPECT_EQ(q.Front().y, 10);
    EXPECT_EQ(q.Back().x, 0);
    EXPECT_EQ(q.Back().y, 0);
}

TEST(StackTest, TestPop) {
    dsa::Stack<int> q{};

    q.Push(5);
    q.Push(10);
    q.Push(15);

    EXPECT_EQ(q.Pop(), 15);
    EXPECT_EQ(q.Pop(), 10);
    EXPECT_EQ(q.Pop(), 5);
    EXPECT_TRUE(q.IsEmpty());
}

TEST(StackTest, TestSwap) {
    dsa::Stack<int> q1{};
    dsa::Stack<int> q2{};

    q1.Push(5);
    q1.Push(10);

    q2.Push(15);
    q2.Push(20);
    q2.Push(25);

    EXPECT_EQ(q1.Front(), 5);
    EXPECT_EQ(q1.Back(), 10);
    EXPECT_EQ(q2.Front(), 15);
    EXPECT_EQ(q2.Back(), 25);

    q1.Swap(q2);

    EXPECT_EQ(q1.Front(), 15);
    EXPECT_EQ(q1.Back(), 25);
    EXPECT_EQ(q2.Front(), 5);
    EXPECT_EQ(q2.Back(), 10);
}
