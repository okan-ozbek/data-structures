//
// Created by Dorza on 4/19/2026.
//

#ifndef TESTSHAREDPOINTER_H
#define TESTSHAREDPOINTER_H

#include "../utils/TempAssert.h"
#include "../SharedPointer.h"

class TestSharedPointer {
public:
    TestSharedPointer() {
        run();
    }

    ~TestSharedPointer() = default;

private:
    void TestDefaultConstructor() {
        const dsa::SharedPointer<int> shared_pointer{};

        assert_true(shared_pointer.Get() == nullptr, "Default constructed pointer should be nullptr");
        assert_true(shared_pointer.ShareCount() == 0, "Default constructed pointer should have share count of 0");
    }

    void TestValueConstructor() {
        const dsa::SharedPointer shared_pointer{new int(10)};

        assert_true(*(shared_pointer.Get()) == 10, "Value constructed pointer should dereference to 10");
        assert_true(shared_pointer.ShareCount() == 1, "Value constructed pointer should have share count of 1");
    }

    void TestCopyConstructor() {
        const dsa::SharedPointer shared_pointer_1{new int(10)};
        const dsa::SharedPointer shared_pointer_2{shared_pointer_1};

        assert_true(*(shared_pointer_2.Get()) == 10, "Copy constructed pointer should dereference to 10");
        assert_true(shared_pointer_2.ShareCount() == 2, "Copy constructed pointer should have share count of 2");
    }

    void TestCopyAssignmentOperator() {
        const dsa::SharedPointer shared_pointer_1{new int(10)};

        {
            dsa::SharedPointer<int> shared_pointer_2{};

            shared_pointer_2 = shared_pointer_1;

            assert_true(*(shared_pointer_2.Get()) == 10, "Copy assigned pointer should dereference to 10");
            assert_true(shared_pointer_2.ShareCount() == 2, "Copy assigned pointer should have share count of 2");
        }

        assert_true(shared_pointer_1.ShareCount() == 1, "Share count should be 1 after copy goes out of scope");
    }

    void TestReset() {
        /**
         * Reset with default parameter value
         */
        dsa::SharedPointer shared_pointer_1{new int(10)};

        shared_pointer_1.Reset();

        assert_true(shared_pointer_1.Get() == nullptr, "Reset pointer should be nullptr");
        assert_true(shared_pointer_1.ShareCount() == 0, "Reset pointer should have share count of 0");

        /**
         * Reset with value
         */
        dsa::SharedPointer shared_pointer_2{new int(10)};

        const auto raw_pointer = new int(20);
        shared_pointer_2.Reset(raw_pointer);

        assert_true(*(shared_pointer_2.Get()) == 20, "Pointer reset with new value should dereference to 20");
        assert_true(shared_pointer_2.ShareCount() == 1, "Pointer reset with new value should have share count of 1");

        /**
         * Deleter
         */
        bool deleted{false};
        auto deleter = [&deleted](const int* pointer) {
            delete pointer;
            deleted = true;
        };

        dsa::SharedPointer shared_pointer_3(new int(10));

        shared_pointer_3.Reset(nullptr, deleter);

        assert_true(deleted, "Destruction: should not be deleted");
    }

    void TestSwap() {
        dsa::SharedPointer shared_pointer_1{new int(10)};
        dsa::SharedPointer shared_pointer_1_cp{shared_pointer_1};
        dsa::SharedPointer shared_pointer_2{new int(30)};

        shared_pointer_1.Swap(shared_pointer_2);

        assert_true(*(shared_pointer_1.Get()) == 30, "Swapped pointer 1 should dereference to 30");
        assert_true(shared_pointer_1.ShareCount() == 1, "Swapped pointer 1 should have share count of 1");
        assert_true(*(shared_pointer_2.Get()) == 10, "Swapped pointer 2 should dereference to 10");
        assert_true(shared_pointer_2.ShareCount() == 2, "Swapped pointer 2 should have share count of 2");
    }

    void TestGet() {
        const dsa::SharedPointer shared_pointer{new int(10)};

        assert_true(*(shared_pointer.Get()) == 10, "get() should return pointer to value 10");
    }

    void TestIsUnique() {
        const dsa::SharedPointer shared_pointer_1{new int(10)};

        assert_true(shared_pointer_1.ShareCount() == 1, "Single owner should have share count of 1");

        dsa::SharedPointer shared_pointer_2{shared_pointer_1};

        assert_true(shared_pointer_1.ShareCount() == 2, "Two owners should have share count of 2");
    }

    void TestShareCount() {
        const dsa::SharedPointer shared_pointer_1{new int(10)};

        dsa::SharedPointer shared_pointer_2{shared_pointer_1};
        dsa::SharedPointer shared_pointer_3{shared_pointer_1};

        assert_true(shared_pointer_1.ShareCount() == 3, "Three owners should have share count of 3");

        shared_pointer_3.Reset();

        assert_true(shared_pointer_1.ShareCount() == 2, "Share count should be 2 after one owner resets");

        shared_pointer_2.Reset();

        assert_true(shared_pointer_1.ShareCount() == 1, "Share count should be 1 after two owners reset");
    }

    void run() {
        // Constructors
        TestDefaultConstructor();
        TestValueConstructor();
        TestCopyConstructor();

        // Assignment operators
        TestCopyAssignmentOperator();

        // Functions
        TestReset();
        TestSwap();
        TestGet();
        TestIsUnique();
        TestShareCount();
    }
};

#endif // TESTSHAREDPOINTER_H
