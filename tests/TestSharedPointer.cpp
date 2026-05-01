//
// Created by Dorza on 5/1/2026.
//

#include <gtest/gtest.h>
#include "../include/SharedPointer.h"

TEST(SharedPointerTest, TestDefaultConstructor) {
    const dsa::SharedPointer<int> shared_pointer{};

    EXPECT_EQ(shared_pointer.Get(), nullptr);
    EXPECT_EQ(shared_pointer.ShareCount(), 0);
}

TEST(SharedPointerTest, TestValueConstructor) {
    const dsa::SharedPointer shared_pointer{new int(10)};

    EXPECT_EQ(*shared_pointer.Get(), 10);
    EXPECT_EQ(shared_pointer.ShareCount(), 1);
}

TEST(SharedPointerTest, TestCopyConstructor) {
    const dsa::SharedPointer shared_pointer_1{new int(10)};
    const dsa::SharedPointer shared_pointer_2{shared_pointer_1};

    EXPECT_EQ(*shared_pointer_2.Get(), 10);
    EXPECT_EQ(shared_pointer_2.ShareCount(), 2);
}

TEST(SharedPointerTest, TestCopyAssignmentOperator) {
    const dsa::SharedPointer shared_pointer_1{new int(10)};

    {
        dsa::SharedPointer<int> shared_pointer_2{};
        shared_pointer_2 = shared_pointer_1;

        EXPECT_EQ(*shared_pointer_2.Get(), 10);
        EXPECT_EQ(shared_pointer_2.ShareCount(), 2);
    }

    EXPECT_EQ(shared_pointer_1.ShareCount(), 1);
}

TEST(SharedPointerTest, TestReset) {
    dsa::SharedPointer shared_pointer_1{new int(10)};

    shared_pointer_1.Reset();

    EXPECT_EQ(shared_pointer_1.Get(), nullptr);
    EXPECT_EQ(shared_pointer_1.ShareCount(), 0);

    dsa::SharedPointer shared_pointer_2{new int(10)};
    const auto raw_pointer = new int(20);
    shared_pointer_2.Reset(raw_pointer);

    EXPECT_EQ(*shared_pointer_2.Get(), 20);
    EXPECT_EQ(shared_pointer_2.ShareCount(), 1);

    bool deleted{false};
    auto deleter = [&deleted](const int* pointer) {
        delete pointer;
        deleted = true;
    };

    dsa::SharedPointer shared_pointer_3(new int(10));
    shared_pointer_3.Reset(nullptr, deleter);

    EXPECT_TRUE(deleted);
}

TEST(SharedPointerTest, TestSwap) {
    dsa::SharedPointer shared_pointer_1{new int(10)};
    dsa::SharedPointer shared_pointer_1_cp{shared_pointer_1};
    dsa::SharedPointer shared_pointer_2{new int(30)};

    shared_pointer_1.Swap(shared_pointer_2);

    EXPECT_EQ(*shared_pointer_1.Get(), 30);
    EXPECT_EQ(shared_pointer_1.ShareCount(), 1);
    EXPECT_EQ(*shared_pointer_2.Get(), 10);
    EXPECT_EQ(shared_pointer_2.ShareCount(), 2);
}

TEST(SharedPointerTest, TestGet) {
    const dsa::SharedPointer shared_pointer{new int(10)};

    EXPECT_EQ(*shared_pointer.Get(), 10);
}

TEST(SharedPointerTest, TestIsUnique) {
    const dsa::SharedPointer shared_pointer_1{new int(10)};

    EXPECT_EQ(shared_pointer_1.ShareCount(), 1);

    dsa::SharedPointer shared_pointer_2{shared_pointer_1};

    EXPECT_EQ(shared_pointer_1.ShareCount(), 2);
}

TEST(SharedPointerTest, TestShareCount) {
    const dsa::SharedPointer shared_pointer_1{new int(10)};

    dsa::SharedPointer shared_pointer_2{shared_pointer_1};
    dsa::SharedPointer shared_pointer_3{shared_pointer_1};

    EXPECT_EQ(shared_pointer_1.ShareCount(), 3);

    shared_pointer_3.Reset();

    EXPECT_EQ(shared_pointer_1.ShareCount(), 2);

    shared_pointer_2.Reset();

    EXPECT_EQ(shared_pointer_1.ShareCount(), 1);
}
