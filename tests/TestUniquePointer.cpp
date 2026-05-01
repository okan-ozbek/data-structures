//
// Created by Dorza on 5/1/2026.
//

#include <gtest/gtest.h>
#include "../include/UniquePointer.h"

struct Vector2D {
    int x;
    int y;

    Vector2D() : x{0}, y{0} {}
    explicit Vector2D(const int scalar) : x{scalar}, y{scalar} {}
    explicit Vector2D(const int x, const int y) : x{x}, y{y} {}
};

TEST(UniquePointerTest, TestDefaultConstructor) {
    dsa::UniquePointer<int> pointer{};

    EXPECT_EQ(pointer.Get(), nullptr);
}

TEST(UniquePointerTest, TestExplicitValueConstructor) {
    const dsa::UniquePointer pointer(new Vector2D{10, 10});

    EXPECT_EQ(pointer->x, 10);
    EXPECT_EQ(pointer->y, 10);
}

TEST(UniquePointerTest, TestFactoryConstructor) {
    const dsa::UniquePointer<Vector2D> pointer = dsa::make_unique<Vector2D>(10, 10);

    EXPECT_EQ(pointer->x, 10);
    EXPECT_EQ(pointer->y, 10);
}

TEST(UniquePointerTest, TestMoveConstructor) {
    dsa::UniquePointer<Vector2D> pointer_1 = dsa::make_unique<Vector2D>(10, 10);
    const Vector2D* temp_pointer = pointer_1.Get();

    const dsa::UniquePointer pointer_2{std::move(pointer_1)};

    EXPECT_EQ(pointer_2.Get(), temp_pointer);
    EXPECT_EQ(pointer_1.Get(), nullptr);
}

TEST(UniquePointerTest, TestDestructor) {
    bool deleted{false};

    {
        auto deleter = [&deleted](const Vector2D* pointer) {
            delete pointer;
            deleted = true;
        };

        dsa::UniquePointer pointer(new Vector2D(10, 10), deleter);

        EXPECT_FALSE(deleted);
    }

    EXPECT_TRUE(deleted);
}

TEST(UniquePointerTest, TestMoveAssignmentOperator) {
    dsa::UniquePointer<Vector2D> pointer_1 = dsa::make_unique<Vector2D>(10, 10);
    const Vector2D* temp_pointer = pointer_1.Get();
    dsa::UniquePointer<Vector2D> pointer_2{};

    pointer_2 = std::move(pointer_1);

    EXPECT_EQ(pointer_2.Get(), temp_pointer);
    EXPECT_EQ(pointer_1.Get(), nullptr);
}

TEST(UniquePointerTest, TestArrowOperator) {
    constexpr int x{10};
    constexpr int y{15};
    const dsa::UniquePointer<Vector2D> pointer = dsa::make_unique<Vector2D>(x, y);

    EXPECT_EQ(pointer->x, x);
    EXPECT_EQ(pointer->y, y);
}

TEST(UniquePointerTest, TestDereferenceOperator) {
    constexpr int x{10};
    constexpr int y{15};
    const dsa::UniquePointer<Vector2D> pointer = dsa::make_unique<Vector2D>(x, y);

    EXPECT_EQ((*pointer).x, x);
    EXPECT_EQ((*pointer).y, y);
}

TEST(UniquePointerTest, TestBooleanOperator) {
    const dsa::UniquePointer<Vector2D> pointer = dsa::make_unique<Vector2D>(10, 10);

    EXPECT_TRUE(static_cast<bool>(pointer));

    const dsa::UniquePointer<Vector2D> nullPointer{};

    EXPECT_FALSE(static_cast<bool>(nullPointer));
}

TEST(UniquePointerTest, TestRelease) {
    constexpr int n{10};
    dsa::UniquePointer<Vector2D> pointer = dsa::make_unique<Vector2D>(n);

    const Vector2D* stale = pointer.Release();

    EXPECT_EQ(stale->x, n);
    EXPECT_EQ(stale->y, n);
    EXPECT_FALSE(static_cast<bool>(pointer));

    delete stale;
}

TEST(UniquePointerTest, TestReset) {
    constexpr int n{10};
    bool deleted{false};

    auto deleter = [&deleted](const Vector2D* pointer) {
        delete pointer;
        deleted = true;
    };

    dsa::UniquePointer pointer_1(new Vector2D(n), deleter);

    pointer_1.Reset(new Vector2D(n * 2));

    EXPECT_TRUE(deleted);
    EXPECT_EQ(pointer_1->x, n * 2);
    EXPECT_EQ(pointer_1->y, n * 2);

    deleted = false;
    pointer_1.Reset(pointer_1.Get());

    EXPECT_FALSE(deleted);
    EXPECT_EQ(pointer_1->x, n * 2);
    EXPECT_EQ(pointer_1->y, n * 2);
}

TEST(UniquePointerTest, TestGet) {
    constexpr int n{10};
    const dsa::UniquePointer<Vector2D> pointer_1 = dsa::make_unique<Vector2D>(n);

    EXPECT_NE(pointer_1.Get(), nullptr);
    EXPECT_EQ(pointer_1.Get()->x, n);
    EXPECT_EQ(pointer_1.Get()->y, n);
}

TEST(UniquePointerTest, TestSwap) {
    constexpr int n{10};
    dsa::UniquePointer<Vector2D> pointer_1 = dsa::make_unique<Vector2D>(n);
    dsa::UniquePointer<Vector2D> pointer_2 = dsa::make_unique<Vector2D>(n * 2);

    pointer_1.Swap(pointer_2);

    EXPECT_EQ(pointer_1->x, n * 2);
    EXPECT_EQ(pointer_1->y, n * 2);
    EXPECT_EQ(pointer_2->x, n);
    EXPECT_EQ(pointer_2->y, n);
}
