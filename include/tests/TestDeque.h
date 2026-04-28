//
// Created by Dorza on 4/28/2026.
//

#ifndef DATA_STRUCTURES_TESTDEQUE_H
#define DATA_STRUCTURES_TESTDEQUE_H


#include "../Deque.h"
#include "../utils/TempAssert.h"

class TestDeque {
public:
    TestDeque() {
        run();
    }

    ~TestDeque() = default;

private:
    void TestDefaultConstructor() {
        const dsa::Deque<int> dq{};

        assert_true(dq.IsEmpty(), "Default Constructor: Default constructed deque is not empty");
        assert_true(dq.Capacity() == 10, "Default Constructor: Default constructed deque capacity is not equal to default capacity");
        assert_true(dq.Size() == 0, "Default Constructor: Default constructed deque size is not zero");
    }

    void TestCopyConstructor() {
        dsa::Deque<int> dq{};

        dq.PushBack(10);
        dq.PushBack(20);
        dq.PushBack(30);

        assert_true(dq.Size() == 3, "Copy Constructor: Original deque size is not equal to 3");

        const dsa::Deque dq2(dq);

        assert_true(dq2.Size() == 3, "Copy Constructor: Copy constructed deque size is not equal to 3");
        assert_true(dq2.Front() == 10, "Copy Constructor: Copy constructed) deque front is not equal to 10");
        assert_true(dq2.Back() == 30, "Copy Constructor: Copy constructed deque) back is not equal to 30");
    }

    void TestMoveConstructor() {
        dsa::Deque<int> dq{};

        dq.PushBack(10);
        dq.PushBack(20);
        dq.PushBack(30);

        assert_true(dq.Size() == 3, "Move Constructor: Original deque size is not equal to 3");

        const dsa::Deque dq2(std::move(dq));

        assert_true(dq.IsEmpty(), "Move Constructor: Moved from deque is not empty");
        assert_true(dq.Size() == 0, "Move Constructor: Moved from deque size is not zero");
        assert_true(dq2.Size() == 3, "Move Constructor: Move constructed deque size is not equal to 3");
        assert_true(dq2.Front() == 10, "Move Constructor: Move constructed deque front is not equal to 10");
        assert_true(dq2.Back() == 30, "Move Constructor: Move constructed deque back is not equal to 30");
    }

    void TestCopyAssignmentOperator() {
        dsa::Deque<int> dq{};

        dq.PushBack(10);
        dq.PushBack(20);
        dq.PushBack(30);

        assert_true(dq.Size() == 3, "Copy Assignment Operator: Original deque size is not equal to 3");

        dsa::Deque<int> dq2{};
        dq2 = dq;

        assert_true(dq2.Size() == 3, "Copy Assignment Operator: Copy assigned deque size is not equal to 3");
        assert_true(dq2.Front() == 10, "Copy Assignment Operator: Copy) assigned deque front is not equal to 10");
        assert_true(dq2.Back() == 30, "Copy Assignment Operator: Copy assigned deque back is not equal to 30");
    }

    void TestMoveAssignmentOperator() {
        dsa::Deque<int> dq{};

        dq.PushBack(10);
        dq.PushBack(20);
        dq.PushBack(30);

        assert_true(dq.Size() == 3, "Move Assignment Operator: Original deque size is not equal to 3");

        dsa::Deque<int> dq2{};
        dq2 = std::move(dq);

        assert_true(dq.IsEmpty(), "Move Assignment Operator: Moved from deque is not empty");
        assert_true(dq.Size() == 0, "Move Assignment Operator: Moved from deque size is not zero");
        assert_true(dq2.Size() == 3, "Move Assignment Operator: Move assigned deque size is not equal to 3");
        assert_true(dq2.Front() == 10, "Move Assignment Operator: Move assigned deque front is not equal to 10");
        assert_true(dq2.Back() == 30, "Move Assignment Operator: Move assigned deque back is not equal to 30");
    }

    void TestIndefOfOperator() {
        dsa::Deque<int> dq{};

        dq.PushBack(10);
        dq.PushBack(20);
        dq.PushBack(30);

        assert_true(dq[0] == 10, "IndefOf Operator: Deque at index 0 is not equal to 10");
        assert_true(dq[1] == 20, "IndefOf Operator: Deque at index 1 is not equal to 20");
        assert_true(dq[2] == 30, "IndefOf Operator: Deque at index 2 is not equal to 30");

        dq.PopFront();

        assert_true(dq[0] == 20, "IndefOf Operator: Deque at index 0 is not equal to 20 after popping front");
    }

    void TestIterators() {
        dsa::Deque<int> dq{};

        dq.PushBack(10);
        dq.PushBack(20);
        dq.PushBack(30);

        assert_true(dq.Size() == 3, "Iterators are not equal to 3");

        for (int& x : dq) {
            x += 5;
        }

        assert_true(dq.Size() == 3, "Iterators are not equal to 3");
        assert_true(dq[0] == 15, "Iterators value at index 0 is not equal to 15");
        assert_true(dq[1] == 25, "Iterators value at index 1 is not equal to 25");
        assert_true(dq[2] == 35, "Iterators value at index 2 is not equal to 35");
    }

    void TestFront() {
        dsa::Deque<int> dq{};

        dq.PushBack(10);
        dq.PushBack(20);
        dq.PushBack(30);

        assert_true(dq.Front() == 10, "Front: Deque front is not equal to 10");
    }

    void TestBack() {
        dsa::Deque<int> dq{};

        dq.PushBack(10);
        dq.PushBack(20);
        dq.PushBack(30);

        assert_true(dq.Back() == 30, "Back: Deque back is not equal to 30");
    }

    void TestIsEmpty() {
        dsa::Deque<int> dq{};

        assert_true(dq.IsEmpty(), "IsEmpty: Deque is NOT empty");
    }

    void TestResize() {
        dsa::Deque<int> dq{};

        for (int i{}; i < 21; ++i) {
            dq.PushBack(i);
        }

        assert_true(dq.Size() == 21, "Resize: Deque size is not equal to 21 after pushing 21 elements");
        assert_true(dq.Capacity() == 40, "Resize: Deque capacity is not equal to 40 after pushing 20 elements");
        assert_true(dq[0] == 0, "Resize: Deque at index 0 is not equal to 0 after pushing 20 elements");
        assert_true(dq[19] == 19, "Resize: Deque at index 19 is not equal to 19 after pushing 20 elements");
    }

    void TestPushBack() {
        dsa::Deque<int> dq{};

        for (int i{}; i < 10; ++i) {
            dq.PushBack(i);
        }

        assert_true(dq.Size() == 10, "PushBack: Deque size is not equal to 10 after pushing 10 elements");
        assert_true(dq[0] == 0, "PushBack: Deque at) index 0 is not equal to 0 after pushing 10 elements");
        assert_true(dq[9] == 9, "PushBack: Deque at) index 9 is not equal to 9 after pushing 10 elements");
    }

    void TestPushFront() {
        dsa::Deque<int> dq{};

        for (int i{}; i < 10; ++i) {
            dq.PushFront(i);
        }

        assert_true(dq.Size() == 10, "PushBack: Deque size is not equal to 10 after pushing 10 elements");
        assert_true(dq[0] == 9, "PushFront: Deque at index 0 is not equal to 9 after pushing 10 elements");
        assert_true(dq[9] == 0, "PushFront: Deque at) index 9 is not equal to 0 after pushing 10 elements");
    }

    void TestPopBack() {
        dsa::Deque<int> dq{};

        for (int i{}; i < 10; ++i) {
            dq.PushBack(i);
        }

        assert_true(dq.Size() == 10, "PopBack: Deque size is not equal to 10 after pushing 10 elements");
        assert_true(dq[0] == 0, "PopBack: Deque at index 0 is not equal to 0 after pushing 10 elements");
        assert_true(dq[9] == 9, "PopBack: Deque at index 9 is not equal to 9 after pushing 10 elements");

        for (int i{9}; i >= 0; --i) {
            const int value = dq.PopBack();

            assert_true(value == i, "PopBack: Pop value is not equal to expected value");
        }

        assert_true(dq.IsEmpty(), "PopBack: Deque is not empty after popping all elements");
    }

    void TestPopFront() {
        dsa::Deque<int> dq{};

        for (int i{}; i < 10; ++i) {
            dq.PushBack(i);
        }

        assert_true(dq.Size() == 10, "PopFront: Deque size is not equal to 10 after pushing 10 elements");
        assert_true(dq[0] == 0, "PopFront: Deque at) index 0 is not equal to 0 after pushing 10 elements");
        assert_true(dq[9] == 9, "PopFront: Deque at) index 9 is not equal to 9 after pushing 10 elements");

        for (int i{0}; i < 10; ++i) {
            const int value = dq.PopFront();

            assert_true(value == i, "PopFront: Pop value is not equal to expected value");
        }

        assert_true(dq.IsEmpty(), "PopFront: Deque is not empty after popping all elements");
    }

    void TestEmplaceFront() {
        struct Vector2D {
            int x;
            int y;

            explicit Vector2D(const int scalar)
                : x{ scalar }
                , y{ scalar }
            {}

            explicit Vector2D(const int x, const int y)
                : x{ x }
                , y{ y }
            {}
        };

        dsa::Deque<Vector2D> dq{};

        for (int i{}; i < 5; ++i) {
            dq.EmplaceFront(i, i + 1);

            assert_true(dq.Front().x == i, "EmplaceFront: Deque front x value is not equal to expected value");
            assert_true(dq.Front().y == i + 1, "EmplaceFront): Deque front y value is not equal to expected value");
        }

        for (int i{}; i < 5; ++i) {
            dq.EmplaceFront(i);

            assert_true(dq.Front().x == i, "EmplaceFront: Deque front x value is not equal to expected value");
            assert_true(dq.Front().y == i, "EmplaceFront: Deque) front y value is not equal to expected value");
        }

        assert_true(dq.Size() == 10, "EmplaceFront: Deque size is not equal to 10 after emplacing 10 elements");
    }

    void TestEmplaceBack() {
        struct Vector2D {
            int x;
            int y;

            explicit Vector2D(const int scalar)
                : x{ scalar }
            , y{ scalar }
            {}

            explicit Vector2D(const int x, const int y)
                : x{ x }
            , y{ y }
            {}
        };

        dsa::Deque<Vector2D> dq{};

        for (int i{}; i < 5; ++i) {
            dq.EmplaceBack(i, i + 1);

            assert_true(dq.Back().x == i, "EmplaceBack: Deque back x value is not equal to expected value");
            assert_true(dq.Back().y == i + 1, "EmplaceBack: Deque back y value is not equal to expected value");
        }

        for (int i{}; i < 5; ++i) {
            dq.EmplaceBack(i);

            assert_true(dq.Back().x == i, "EmplaceBack: Deque back x value is not equal to expected value");
            assert_true(dq.Back().y == i, "EmplaceBack: Deque back y value is not equal to expected value");
        }

        assert_true(dq.Size() == 10, "EmplaceBack: Deque size is not equal to 10 after emplacing 10 elements");
    }

    void TestSwap() {
        dsa::Deque<int> dq1{};
        dsa::Deque<int> dq2{};

        for (int i{}; i < 5; ++i) {
            dq1.PushBack(i);

            if (i < 3) {
                dq2.PushBack(i + 5);
            }
        }

        assert_true(dq1.Size() == 5, "Swap: Deque 1 size is not equal to 5 before swap");
        assert_true(dq2.Size() == 3, "Swap: Deque 2 size is not equal to 5 before swap");
        assert_true(dq1.Front() == 0, "Swap: Deque 1 front is not equal to 0 before swap");
        assert_true(dq1.Back() == 4, "Swap: Deque 1 back is not equal to 4 before swap");
        assert_true(dq2.Front() == 5, "Swap: Deque 2 front is not equal to 5 before swap");
        assert_true(dq2.Back() == 7, "Swap: Deque 2 back is not equal to 9 before swap");

        dq1.Swap(dq2);

        assert_true(dq1.Size() == 3, "Swap: Deque 1 size is not equal to 5 after swap");
        assert_true(dq2.Size() == 5, "Swap: Deque 2 size is not equal to 5 after swap");
        assert_true(dq1.Front() == 5, "Swap: Deque 1 front is not equal to 5 after swap");
        assert_true(dq1.Back() == 7, "Swap: Deque 1 back is not equal to 9 after swap");
        assert_true(dq2.Front() == 0, "Swap: Deque 2 front is not equal to 0 after swap");
        assert_true(dq2.Back() == 4, "Swap: Deque 2 back is not equal to 4 after swap");
    }

    void TestClear() {
        dsa::Deque<int> dq{};

        for (int i{}; i < 5; ++i) {
            dq.PushBack(i);
        }

        assert_true(dq.Size() == 5, "Clear: Deque size is not equal to 5 before clear");

        dq.Clear();

        assert_true(dq.IsEmpty(), "Clear: Deque is not empty after clear");
        assert_true(dq.Size() == 0, "Clear: Deque size is not zero after clear");
    }

    void run() {
        // Constructors
        TestDefaultConstructor();
        TestCopyConstructor();
        TestMoveConstructor();

        // Operators
        TestCopyAssignmentOperator();
        TestMoveAssignmentOperator();
        TestIndefOfOperator();

        // Iterators
        TestIterators();

        // Indexing
        TestFront();
        TestBack();

        // Helpers
        TestIsEmpty();
        TestResize();

        // Business logic
        TestPushBack();
        TestPushFront();
        TestPopBack();
        TestPopFront();
        TestEmplaceFront();
        TestEmplaceBack();
        TestSwap();
        TestClear();
    }
};

#endif //DATA_STRUCTURES_TESTDEQUE_H