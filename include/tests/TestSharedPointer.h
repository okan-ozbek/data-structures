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
    void test_default_constructor() {
        const dsa::SharedPointer<int> shared_pointer{};

        assert_true(shared_pointer.get() == nullptr, "Default constructed pointer should be nullptr");
        assert_true(shared_pointer.share_count() == 0, "Default constructed pointer should have share count of 0");
    }

    void test_value_constructor() {
        const dsa::SharedPointer shared_pointer{new int(10)};

        assert_true(*(shared_pointer.get()) == 10, "Value constructed pointer should dereference to 10");
        assert_true(shared_pointer.share_count() == 1, "Value constructed pointer should have share count of 1");
    }

    void test_copy_constructor() {
        const dsa::SharedPointer shared_pointer_1{new int(10)};
        const dsa::SharedPointer shared_pointer_2{shared_pointer_1};

        assert_true(*(shared_pointer_2.get()) == 10, "Copy constructed pointer should dereference to 10");
        assert_true(shared_pointer_2.share_count() == 2, "Copy constructed pointer should have share count of 2");
    }

    void test_copy_assignment_operator() {
        const dsa::SharedPointer shared_pointer_1{new int(10)};

        {
            dsa::SharedPointer<int> shared_pointer_2{};

            shared_pointer_2 = shared_pointer_1;

            assert_true(*(shared_pointer_2.get()) == 10, "Copy assigned pointer should dereference to 10");
            assert_true(shared_pointer_2.share_count() == 2, "Copy assigned pointer should have share count of 2");
        }

        assert_true(shared_pointer_1.share_count() == 1, "Share count should be 1 after copy goes out of scope");
    }

    void test_reset() {
        /**
         * Reset with default parameter value
         */
        dsa::SharedPointer shared_pointer_1{new int(10)};

        shared_pointer_1.reset();

        assert_true(shared_pointer_1.get() == nullptr, "Reset pointer should be nullptr");
        assert_true(shared_pointer_1.share_count() == 0, "Reset pointer should have share count of 0");

        /**
         * Reset with value
         */
        dsa::SharedPointer shared_pointer_2{new int(10)};

        const auto raw_pointer = new int(20);
        shared_pointer_2.reset(raw_pointer);

        assert_true(*(shared_pointer_2.get()) == 20, "Pointer reset with new value should dereference to 20");
        assert_true(shared_pointer_2.share_count() == 1, "Pointer reset with new value should have share count of 1");

        /**
         * Deleter
         */
        bool deleted{false};
        auto deleter = [&deleted](const int* pointer) {
            delete pointer;
            deleted = true;
        };

        dsa::SharedPointer shared_pointer_3(new int(10));

        shared_pointer_3.reset(nullptr, deleter);

        assert_true(deleted, "Destruction: should not be deleted");
    }

    void test_swap() {
        dsa::SharedPointer shared_pointer_1{new int(10)};
        dsa::SharedPointer shared_pointer_1_cp{shared_pointer_1};
        dsa::SharedPointer shared_pointer_2{new int(30)};

        shared_pointer_1.swap(shared_pointer_2);

        assert_true(*(shared_pointer_1.get()) == 30, "Swapped pointer 1 should dereference to 30");
        assert_true(shared_pointer_1.share_count() == 1, "Swapped pointer 1 should have share count of 1");
        assert_true(*(shared_pointer_2.get()) == 10, "Swapped pointer 2 should dereference to 10");
        assert_true(shared_pointer_2.share_count() == 2, "Swapped pointer 2 should have share count of 2");
    }

    void test_get() {
        const dsa::SharedPointer shared_pointer{new int(10)};

        assert_true(*(shared_pointer.get()) == 10, "get() should return pointer to value 10");
    }

    void test_is_unique() {
        const dsa::SharedPointer shared_pointer_1{new int(10)};

        assert_true(shared_pointer_1.share_count() == 1, "Single owner should have share count of 1");

        dsa::SharedPointer shared_pointer_2{shared_pointer_1};

        assert_true(shared_pointer_1.share_count() == 2, "Two owners should have share count of 2");
    }

    void test_share_count() {
        const dsa::SharedPointer shared_pointer_1{new int(10)};

        dsa::SharedPointer shared_pointer_2{shared_pointer_1};
        dsa::SharedPointer shared_pointer_3{shared_pointer_1};

        assert_true(shared_pointer_1.share_count() == 3, "Three owners should have share count of 3");

        shared_pointer_3.reset();

        assert_true(shared_pointer_1.share_count() == 2, "Share count should be 2 after one owner resets");

        shared_pointer_2.reset();

        assert_true(shared_pointer_1.share_count() == 1, "Share count should be 1 after two owners reset");
    }

    void run() {
        // Constructors
        test_default_constructor();
        test_value_constructor();
        test_copy_constructor();

        // Assignment operators
        test_copy_assignment_operator();

        // Functions
        test_reset();
        test_swap();
        test_get();
        test_is_unique();
        test_share_count();
    }
};

#endif // TESTSHAREDPOINTER_H

