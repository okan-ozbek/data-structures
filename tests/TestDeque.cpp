//
// Created by Dorza on 5/1/2026.
//

#include <gtest/gtest.h>
#include "../include/Deque.h"

constexpr std::size_t ZERO{ 0 };
constexpr std::size_t DEFAULT_CAPACITY{ 10 };

struct Vector2D {
    int x;
    int y;

    explicit Vector2D(const int scalar) : x{ scalar }, y{ scalar } {}
    explicit Vector2D(const int x, const int y) : x{ x }, y{ y } {}
};

TEST(DequeTest, TestDefaultConstructor) {
    const dsa::Deque<int> dq{};

    EXPECT_TRUE(dq.IsEmpty());
    EXPECT_EQ(dq.Capacity(), DEFAULT_CAPACITY);
    EXPECT_EQ(dq.Size(), ZERO);
}

TEST(DequeTest, TestCopyConstructor) {
    dsa::Deque<int> dq{};

    dq.PushBack(10);
    dq.PushBack(20);
    dq.PushBack(30);

    EXPECT_EQ(dq.Size(), 3);

    const dsa::Deque dq2(dq);

    EXPECT_EQ(dq2.Size(), 3);
    EXPECT_EQ(dq2.Front(), 10);
    EXPECT_EQ(dq2.Back(), 30);
}

TEST(DequeTest, TestMoveConstructor) {
    dsa::Deque<int> dq{};

    dq.PushBack(10);
    dq.PushBack(20);
    dq.PushBack(30);

    EXPECT_EQ(dq.Size(), 3);

    const dsa::Deque dq2(std::move(dq));

    EXPECT_TRUE(dq.IsEmpty());
    EXPECT_EQ(dq.Size(), ZERO);
    EXPECT_EQ(dq2.Size(), 3);
    EXPECT_EQ(dq2.Front(), 10);
    EXPECT_EQ(dq2.Back(), 30);
}

TEST(DequeTest, TestCopyAssignmentOperator) {
    dsa::Deque<int> dq{};

    dq.PushBack(10);
    dq.PushBack(20);
    dq.PushBack(30);

    EXPECT_EQ(dq.Size(), 3);

    dsa::Deque<int> dq2{};
    dq2 = dq;

    EXPECT_EQ(dq2.Size(), 3);
    EXPECT_EQ(dq2.Front(), 10);
    EXPECT_EQ(dq2.Back(), 30);
}

TEST(DequeTest, TestMoveAssignmentOperator) {
    dsa::Deque<int> dq{};

    dq.PushBack(10);
    dq.PushBack(20);
    dq.PushBack(30);

    EXPECT_EQ(dq.Size(), 3);

    dsa::Deque<int> dq2{};
    dq2 = std::move(dq);

    EXPECT_TRUE(dq.IsEmpty());
    EXPECT_EQ(dq.Size(), ZERO);
    EXPECT_EQ(dq2.Size(), 3);
    EXPECT_EQ(dq2.Front(), 10);
    EXPECT_EQ(dq2.Back(), 30);
}

TEST(DequeTest, TestIndexOfOperator) {
    dsa::Deque<int> dq{};

    dq.PushBack(10);
    dq.PushBack(20);
    dq.PushBack(30);

    EXPECT_EQ(dq[0], 10);
    EXPECT_EQ(dq[1], 20);
    EXPECT_EQ(dq[2], 30);

    dq.PopFront();

    EXPECT_EQ(dq[0], 20);
}

TEST(DequeTest, TestIterators) {
    dsa::Deque<int> dq{};

    dq.PushBack(10);
    dq.PushBack(20);
    dq.PushBack(30);

    EXPECT_EQ(dq.Size(), 3);

    dq.PopFront();

    for (int& x : dq) {
        x += 5;
    }

    EXPECT_EQ(dq.Size(), 2);
    EXPECT_EQ(dq[0], 25);
    EXPECT_EQ(dq[1], 35);
}

TEST(DequeTest, TestFront) {
    dsa::Deque<int> dq{};

    dq.PushBack(10);
    dq.PushBack(20);
    dq.PushBack(30);

    EXPECT_EQ(dq.Front(), 10);
}

TEST(DequeTest, TestBack) {
    dsa::Deque<int> dq{};

    dq.PushBack(10);
    dq.PushBack(20);
    dq.PushBack(30);

    EXPECT_EQ(dq.Back(), 30);
}

TEST(DequeTest, TestIsEmpty) {
    const dsa::Deque<int> dq{};

    EXPECT_TRUE(dq.IsEmpty());
}

TEST(DequeTest, TestResize) {
    dsa::Deque<int> dq{};

    for (int i{}; i < 21; ++i) {
        dq.PushBack(i);
    }

    EXPECT_EQ(dq.Size(), 21);
    EXPECT_EQ(dq.Capacity(), 40);
    EXPECT_EQ(dq[0], 0);
    EXPECT_EQ(dq[19], 19);
}

TEST(DequeTest, TestPushBack) {
    dsa::Deque<int> dq{};

    for (int i{}; i < 10; ++i) {
        dq.PushBack(i);
    }

    EXPECT_EQ(dq.Size(), 10);
    EXPECT_EQ(dq[0], 0);
    EXPECT_EQ(dq[9], 9);
}

TEST(DequeTest, TestPushFront) {
    dsa::Deque<int> dq{};

    for (int i{}; i < 10; ++i) {
        dq.PushFront(i);
    }

    EXPECT_EQ(dq.Size(), 10);
    EXPECT_EQ(dq[0], 9);
    EXPECT_EQ(dq[9], 0);
}

TEST(DequeTest, TestPopBack) {
    dsa::Deque<int> dq{};

    for (int i{}; i < 10; ++i) {
        dq.PushBack(i);
    }

    EXPECT_EQ(dq.Size(), 10);
    EXPECT_EQ(dq[0], 0);
    EXPECT_EQ(dq[9], 9);

    for (int i{9}; i >= 0; --i) {
        const int value = dq.PopBack();
        EXPECT_EQ(value, i);
    }

    EXPECT_TRUE(dq.IsEmpty());
}

TEST(DequeTest, TestPopFront) {
    dsa::Deque<int> dq{};

    for (int i{}; i < 10; ++i) {
        dq.PushBack(i);
    }

    EXPECT_EQ(dq.Size(), 10);
    EXPECT_EQ(dq[0], 0);
    EXPECT_EQ(dq[9], 9);

    for (int i{0}; i < 10; ++i) {
        const int value = dq.PopFront();
        EXPECT_EQ(value, i);
    }

    EXPECT_TRUE(dq.IsEmpty());
}

TEST(DequeTest, TestEmplaceFront) {
    dsa::Deque<Vector2D> dq{};

    for (int i{}; i < 5; ++i) {
        dq.EmplaceFront(i, i + 1);
        EXPECT_EQ(dq.Front().x, i);
        EXPECT_EQ(dq.Front().y, i + 1);
    }

    for (int i{}; i < 5; ++i) {
        dq.EmplaceFront(i);
        EXPECT_EQ(dq.Front().x, i);
        EXPECT_EQ(dq.Front().y, i);
    }

    EXPECT_EQ(dq.Size(), 10);
}

TEST(DequeTest, TestEmplaceBack) {
    dsa::Deque<Vector2D> dq{};

    for (int i{}; i < 5; ++i) {
        dq.EmplaceBack(i, i + 1);
        EXPECT_EQ(dq.Back().x, i);
        EXPECT_EQ(dq.Back().y, i + 1);
    }

    for (int i{}; i < 5; ++i) {
        dq.EmplaceBack(i);
        EXPECT_EQ(dq.Back().x, i);
        EXPECT_EQ(dq.Back().y, i);
    }

    EXPECT_EQ(dq.Size(), 10);
}

TEST(DequeTest, TestSwap) {
    dsa::Deque<int> dq1{};
    dsa::Deque<int> dq2{};

    for (int i{}; i < 5; ++i) {
        dq1.PushBack(i);
        if (i < 3) {
            dq2.PushBack(i + 5);
        }
    }

    EXPECT_EQ(dq1.Size(), 5);
    EXPECT_EQ(dq2.Size(), 3);
    EXPECT_EQ(dq1.Front(), 0);
    EXPECT_EQ(dq1.Back(), 4);
    EXPECT_EQ(dq2.Front(), 5);
    EXPECT_EQ(dq2.Back(), 7);

    dq1.Swap(dq2);

    EXPECT_EQ(dq1.Size(), 3);
    EXPECT_EQ(dq2.Size(), 5);
    EXPECT_EQ(dq1.Front(), 5);
    EXPECT_EQ(dq1.Back(), 7);
    EXPECT_EQ(dq2.Front(), 0);
    EXPECT_EQ(dq2.Back(), 4);
}

TEST(DequeTest, TestClear) {
    dsa::Deque<int> dq{};

    for (int i{}; i < 5; ++i) {
        dq.PushBack(i);
    }

    EXPECT_EQ(dq.Size(), 5);

    dq.Clear();

    EXPECT_TRUE(dq.IsEmpty());
    EXPECT_EQ(dq.Size(), ZERO);
}
