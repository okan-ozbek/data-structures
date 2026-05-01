//
// Created by Dorza on 5/1/2026.
//


#include <gtest/gtest.h>
#include "../include/LinkedList.h"


TEST(LinkedListTest, TestDefaultConstructor) {
    dsa::LinkedList<int> ll{};

    EXPECT_TRUE(ll.IsEmpty());
}

TEST(LinkedListTest, TestCopyConstructor) {
    dsa::LinkedList<int> ll{};

    ll.PushBack(1);
    ll.PushBack(2);
    ll.PushBack(3);

    dsa::LinkedList ll2{ ll };

    EXPECT_EQ(ll2.Front(), 1);
    EXPECT_EQ(ll2[1], 2);
    EXPECT_EQ(ll2[2], 3);
    EXPECT_EQ(ll2.Back(), 3);
}

TEST(LinkedListTest, TestMoveConstructor) {
    dsa::LinkedList<int> ll{};

    ll.PushBack(1);
    ll.PushBack(2);
    ll.PushBack(3);

    dsa::LinkedList ll2{ std::move(ll) };

    EXPECT_EQ(ll2.Front(), 1);
    EXPECT_EQ(ll2[1], 2);
    EXPECT_EQ(ll2[2], 3);
    EXPECT_EQ(ll2.Back(), 3);
    EXPECT_EQ(ll2.Size(), 3);

    EXPECT_TRUE(ll.IsEmpty());
    EXPECT_EQ(ll.Size(), 0);
}

TEST(LinkedListTest, TestCopyAssignmentOperator) {
    dsa::LinkedList<int> ll{};

    ll.PushBack(1);
    ll.PushBack(2);
    ll.PushBack(3);

    dsa::LinkedList<int> ll2{};

    ll2 = ll;

    EXPECT_EQ(ll2.Front(), 1);
    EXPECT_EQ(ll2[1], 2);
    EXPECT_EQ(ll2[2], 3);
    EXPECT_EQ(ll2.Back(), 3);
}

TEST(LinkedListTest, TestMoveAssignmentOperator) {
    dsa::LinkedList<int> ll{};

    ll.PushBack(1);
    ll.PushBack(2);
    ll.PushBack(3);

    dsa::LinkedList<int> ll2{};

    ll2 = std::move(ll);

    EXPECT_EQ(ll2.Front(), 1);
    EXPECT_EQ(ll2[1], 2);
    EXPECT_EQ(ll2[2], 3);
    EXPECT_EQ(ll2.Back(), 3);
    EXPECT_EQ(ll2.Size(), 3);

    EXPECT_TRUE(ll.IsEmpty());
    EXPECT_EQ(ll.Size(), 0);
}

TEST(LinkedListTest, TestIndexOfOperator) {
    dsa::LinkedList<int> ll{};

    ll.PushBack(1);
    ll.PushBack(2);
    ll.PushBack(3);

    EXPECT_EQ(ll[0], 1);
    EXPECT_EQ(ll[1], 2);
    EXPECT_EQ(ll[2], 3);
}

TEST(LinkedListTest, TestIterators) {
    dsa::LinkedList<int> ll{};

    ll.PushBack(1);
    ll.PushBack(2);
    ll.PushBack(3);

    int expect = ll[0];
    for (auto& val : ll) {
        EXPECT_EQ(val, expect);
        ++expect;
    }
}

TEST(LinkedListTest, TestAt) {
    dsa::LinkedList<int> ll{};

    ll.PushBack(1);
    ll.PushBack(2);
    ll.PushBack(3);

    EXPECT_EQ(ll.At(0), 1);
    EXPECT_EQ(ll.At(1), 2);
    EXPECT_EQ(ll.At(2), 3);
}

TEST(LinkedListTest, TestPushFront) {
    dsa::LinkedList<int> ll{};

    ll.PushFront(1);
    ll.PushFront(2);
    ll.PushFront(3);

    EXPECT_EQ(ll[0], 3);
    EXPECT_EQ(ll[1], 2);
    EXPECT_EQ(ll[2], 1);
}

TEST(LinkedListTest, TestPushBack) {
    dsa::LinkedList<int> ll{};

    ll.PushBack(1);
    ll.PushBack(2);
    ll.PushBack(3);

    EXPECT_EQ(ll[0], 1);
    EXPECT_EQ(ll[1], 2);
    EXPECT_EQ(ll[2], 3);
}

TEST(LinkedListTest, TestPushAfter) {
    dsa::LinkedList<int> ll{};

    ll.PushBack(1);
    ll.PushBack(3);
    ll.PushBack(5);

    ll.PushAfter(1, 4);
    ll.PushAfter(ll.Head(), 2);

    int expect = ll[0];
    for (auto& val : ll) {
        EXPECT_EQ(val, expect);
        ++expect;
    }
}

TEST(LinkedListTest, TestPopFront) {
    dsa::LinkedList<int> ll{};

    ll.PushFront(1);
    ll.PushFront(2);
    ll.PushFront(3);

    EXPECT_EQ(ll.PopFront(), 3);
    EXPECT_EQ(ll.PopFront(), 2);
    EXPECT_EQ(ll.PopFront(), 1);
}

TEST(LinkedListTest, TestPopBack) {
    dsa::LinkedList<int> ll{};

    ll.PushBack(1);
    ll.PushBack(2);
    ll.PushBack(3);

    EXPECT_EQ(ll.PopBack(), 3);
    EXPECT_EQ(ll.PopBack(), 2);
    EXPECT_EQ(ll.PopBack(), 1);
}

TEST(LinkedListTest, TestErase) {
    dsa::LinkedList<int> ll{};

    ll.PushBack(1);
    ll.PushBack(2);
    ll.PushBack(3);

    ll.Erase(ll.begin() + 1);

    EXPECT_EQ(ll[0], 1);
    EXPECT_EQ(ll[1], 3);
}

TEST(LinkedListTest, TestSize) {
    dsa::LinkedList<int> ll{};

    EXPECT_EQ(ll.Size(), 0);

    ll.PushBack(1);
    ll.PushBack(2);
    ll.PushBack(3);

    EXPECT_EQ(ll.Size(), 3);

    ll.Erase(ll.begin() + 1);

    EXPECT_EQ(ll.Size(), 2);
}

TEST(LinkedListTest, TestIsEmpty) {
    dsa::LinkedList<int> ll{};

    EXPECT_TRUE(ll.IsEmpty());

    ll.PushBack(1);

    EXPECT_FALSE(ll.IsEmpty());

    ll.Erase(ll.begin());

    EXPECT_TRUE(ll.IsEmpty());
}