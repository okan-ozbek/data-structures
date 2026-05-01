//
// Created by Dorza on 5/1/2026.
//

#include <gtest/gtest.h>
#include "../include/Vector.h"

constexpr std::size_t ZERO{ 0 };
constexpr std::size_t DEFAULT_CAPACITY{ 10 };

constexpr std::size_t TEST_CAPACITY{ 3 };
constexpr int TEST_VALUE{ 3 };

struct Vector2D {
    int x;
    int y;

    Vector2D() : x{0}, y{0} {}
    explicit Vector2D(const int scalar) : x{scalar}, y{scalar} {}
    explicit Vector2D(const int x, const int y) : x{x}, y{y} {}
};

TEST(VectorTest, TestDefaultConstructor) {
    const dsa::Vector<int> v{};

    EXPECT_EQ(v.Size(), ZERO);
    EXPECT_EQ(v.Capacity(), DEFAULT_CAPACITY);
}

TEST(VectorTest, TestCapacityConstructor) {
    const dsa::Vector<int> v{ TEST_CAPACITY };

    EXPECT_EQ(v.Capacity(), TEST_CAPACITY);
}

TEST(VectorTest, TestCapacityValueConstructor) {
    const dsa::Vector v{ TEST_CAPACITY, TEST_VALUE };

    EXPECT_EQ(v.Capacity(), TEST_CAPACITY);
    EXPECT_EQ(v.Size(), TEST_CAPACITY);
    EXPECT_EQ(v[0], TEST_VALUE);
    EXPECT_EQ(v[TEST_CAPACITY - 1], TEST_VALUE);
}

TEST(VectorTest, TestCopyConstructor) {
    const dsa::Vector v{ TEST_CAPACITY, TEST_VALUE };
    const dsa::Vector vCopy{ v };

    EXPECT_EQ(*v.begin(), *vCopy.begin());
    EXPECT_EQ(v.Size(), vCopy.Size());
    EXPECT_EQ(v.Capacity(), vCopy.Capacity());
}

TEST(VectorTest, TestMoveConstructor) {
    dsa::Vector v{ TEST_CAPACITY, TEST_VALUE };
    const dsa::Vector vMove{ std::move(v) };

    EXPECT_EQ(v.Size(), ZERO);
    EXPECT_EQ(vMove.Size(), TEST_CAPACITY);
    EXPECT_EQ(vMove.Capacity(), TEST_CAPACITY);
}

TEST(VectorTest, TestCopyAssignmentOperator) {
    const dsa::Vector v{ TEST_CAPACITY, TEST_VALUE };
    dsa::Vector<int> vCopy{};

    vCopy = v;

    EXPECT_EQ(*v.begin(), *vCopy.begin());
    EXPECT_EQ(v.Size(), vCopy.Size());
    EXPECT_EQ(v.Capacity(), vCopy.Capacity());
}

TEST(VectorTest, TestMoveAssignmentOperator) {
    dsa::Vector v{ TEST_CAPACITY, TEST_VALUE };
    dsa::Vector<int> vMove{};

    vMove = std::move(v);

    EXPECT_EQ(v.Size(), ZERO);
    EXPECT_EQ(v.Capacity(), ZERO);
    EXPECT_EQ(vMove.Size(), TEST_CAPACITY);
    EXPECT_EQ(vMove.Capacity(), TEST_CAPACITY);
}

TEST(VectorTest, TestIndexOfOperator) {
    dsa::Vector<int> v{};

    v.PushBack(5);

    EXPECT_EQ(v[0], 5);

    v[0] = 10;

    EXPECT_EQ(v[0], 10);
}

TEST(VectorTest, TestIndexOperatorOutOfBounds) {
    dsa::Vector<int> v{};

    EXPECT_THROW(v[100] = 10, std::out_of_range);
}

TEST(VectorTest, TestIterators) {
    dsa::Vector v{ TEST_CAPACITY, TEST_VALUE };

    for (int& x : v) {
        x = 10;
    }

    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 10);
    EXPECT_EQ(v[2], 10);
}

TEST(VectorTest, TestAt) {
    dsa::Vector v{ TEST_CAPACITY, TEST_VALUE };

    EXPECT_EQ(v.At(0), TEST_VALUE);
    EXPECT_EQ(v.At(1), TEST_VALUE);
    EXPECT_EQ(v.At(2), TEST_VALUE);
}

TEST(VectorTest, TestAtOutOfBounds) {
    dsa::Vector<int> v{};

    EXPECT_THROW(v.At(100), std::out_of_range);
}

TEST(VectorTest, TestFront) {
    dsa::Vector v{ TEST_CAPACITY, TEST_VALUE };

    EXPECT_EQ(v.Front(), TEST_VALUE);
}

TEST(VectorTest, TestBack) {
    dsa::Vector v{ TEST_CAPACITY, TEST_VALUE };

    EXPECT_EQ(v.Back(), TEST_VALUE);
}

TEST(VectorTest, TestPushBack) {
    dsa::Vector<int> v{};

    v.PushBack(5);
    v.PushBack(10);

    EXPECT_EQ(v[0], 5);
    EXPECT_EQ(v[1], 10);
}

TEST(VectorTest, TestEmplaceBack) {
    dsa::Vector<Vector2D> v{};

    v.EmplaceBack(5, 10);
    v.EmplaceBack(5);
    v.EmplaceBack();

    EXPECT_EQ(v[0].x, 5);
    EXPECT_EQ(v[0].y, 10);
    EXPECT_EQ(v[1].x, 5);
    EXPECT_EQ(v[1].y, 5);
    EXPECT_EQ(v[2].x, 0);
    EXPECT_EQ(v[2].y, 0);

}

TEST(VectorTest, TestPopBack) {
    dsa::Vector v{ TEST_CAPACITY, TEST_VALUE };

    EXPECT_EQ(v.Size(), TEST_CAPACITY);

    v.PopBack();

    EXPECT_EQ(v.Size(), TEST_CAPACITY - 1);

    v.PopBack();

    EXPECT_EQ(v.Size(), TEST_CAPACITY - 2);
}

TEST(VectorTest, TestEraseSingleElement) {
    dsa::Vector v{ TEST_CAPACITY, TEST_VALUE };

    v.Erase(v.begin() + 1);

    EXPECT_EQ(v.Size(), TEST_CAPACITY - 1);
}

TEST(VectorTest, TestEraseRange) {
    dsa::Vector v{ TEST_CAPACITY, TEST_VALUE };

    v.Erase(v.begin(), v.end() - 1);

    EXPECT_EQ(v.Size(), 0);
}

TEST(VectorTest, TestClear) {
    dsa::Vector v{ TEST_CAPACITY, TEST_VALUE };

    v.Clear();

    EXPECT_EQ(v.Size(), 0);
}

TEST(VectorTest, TestResize) {
    dsa::Vector<int> v{};

    EXPECT_EQ(v.Size(), ZERO);
    EXPECT_EQ(v.Capacity(), DEFAULT_CAPACITY);

    for (int i = 0; i < 15; ++i) {
        v.PushBack(i);
    }

    EXPECT_EQ(v.Size(), 15);
    EXPECT_EQ(v.Capacity(), DEFAULT_CAPACITY * 2);
}

TEST(VectorTest, TestReserve) {
    constexpr std::size_t RESERVED_CAPACITY{ 50 };

    dsa::Vector v{ TEST_CAPACITY, TEST_VALUE };

    EXPECT_EQ(v.Size(), TEST_CAPACITY);
    EXPECT_EQ(v.Capacity(), TEST_CAPACITY);

    v.Reserve(RESERVED_CAPACITY);

    EXPECT_EQ(v.Size(), TEST_CAPACITY);
    EXPECT_EQ(v.Capacity(), RESERVED_CAPACITY);
}

TEST(VectorTest, TestShrinkToFit) {
    dsa::Vector<int> v{};

    v.PushBack(5);
    v.PushBack(10);
    v.PushBack(15);

    EXPECT_EQ(v.Size(), 3);
    EXPECT_EQ(v.Capacity(), DEFAULT_CAPACITY);

    v.ShrinkToFit();

    EXPECT_EQ(v.Size(), 3);
    EXPECT_EQ(v.Capacity(), 3);
}