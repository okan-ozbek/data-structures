//
// Created by Dorza on 4/29/2026.
//

#ifndef DATA_STRUCTURES_TESTQUEUE_H
#define DATA_STRUCTURES_TESTQUEUE_H


#include "../Queue.h"
#include "../utils/TempAssert.h"

class TestQueue {
public:
    TestQueue() {
        run();
    }

    ~TestQueue() = default;

private:
    void TestDefaultConstructor() {
        dsa::Queue<int> q{};

        assert_true(q.IsEmpty(), "Default Constructor: Default constructed queue is not empty");
        assert_true(q.Size() == 0, "Default Constructor: Default constructed queue size is not zero");
    }

    void TestCopyConstructor() {
        dsa::Queue<int> q{};

        q.Push(5);
        q.Push(10);

        const dsa::Queue q2{q};

        assert_true(q.IsEmpty() == false, "Copy Constructor: Copy constructed queue is empty");
        assert_true(q2.IsEmpty() == false, "Copy Constructor: Copy constructed queue is empty");
        assert_true(q.Front() == 5, "Copy Constructor: Copy constructed queue front is not equal to 5");
        assert_true(q.Back() == 10, "Copy Constructor: Copy constructed queue back is not equal to 10");
        assert_true(q2.Front() == 5, "Copy Constructor: Copy constructed queue front is not equal to 5");
        assert_true(q2.Back() == 10, "Copy Constructor: Copy constructed queue back is not equal to 10");
    }

    void TestMoveConstructor() {
        dsa::Queue<int> q{};

        q.Push(5);
        q.Push(10);

        const dsa::Queue q2{std::move(q)};

        assert_true(q.IsEmpty(), "Move Constructor: Move constructed queue is not empty");
        assert_true(q.Size() == 0, "Move Constructor: Move constructed queue size is not zero");
        assert_true(q2.IsEmpty() == false, "Move Constructor: Move constructed queue is empty");
        assert_true(q2.Size() == 2, "Move Constructor: Move constructed queue size is not 2");
        assert_true(q2.Front() == 5, "Move Constructor: Move constructed queue front is not equal to 5");
        assert_true(q2.Back() == 10, "Move Constructor: Move constructed queue back is not equal to 10");
    }

    void TestCopyAssignmentOperator() {
        dsa::Queue<int> q{};

        q.Push(5);
        q.Push(10);

        dsa::Queue<int> q2{};

        q2 = q;

        assert_true(q.IsEmpty() == false, "Copy Assignment: Copy constructed queue is empty");
        assert_true(q2.IsEmpty() == false, "Copy Assignment: Copy constructed queue is empty");
        assert_true(q.Front() == 5, "Copy Assignment: Copy constructed queue front is not equal to 5");
        assert_true(q.Back() == 10, "Copy Assignment: Copy constructed queue back is not equal to 10");
        assert_true(q2.Front() == 5, "Copy Assignment: Copy constructed queue front is not equal to 5");
        assert_true(q2.Back() == 10, "Copy Assignment: Copy constructed queue back is not equal to 10");
    }

    void TestMoveAssignmentOperator() {
        dsa::Queue<int> q{};

        q.Push(5);
        q.Push(10);

        dsa::Queue<int> q2{};

        q2 = std::move(q);

        assert_true(q.IsEmpty(), "Move Assignment: Move constructed queue is not empty");
        assert_true(q.Size() == 0, "Move Assignment: Move constructed queue size is not zero");
        assert_true(q2.IsEmpty() == false, "Move Assignment: Copy constructed queue is empty");
        assert_true(q2.Size() == 2, "Move Assignment: Move constructed queue size is not 2");
        assert_true(q2.Front() == 5, "Move Assignment: Move constructed queue front is not equal to 5");
        assert_true(q2.Back() == 10, "Move Assignment: Move constructed queue back is not equal to 10");
    }

    void TestIsEmpty() {
        dsa::Queue<int> q{};

        assert_true(q.IsEmpty(), "IsEmpty: Queue is empty");

        q.Push(5);

        assert_true(q.IsEmpty() == false, "IsEmpty: Queue is empty");
    }

    void TestSize() {
        dsa::Queue<int> q{};

        assert_true(q.Size() == 0, "Size: Default constructed queue size is not zero");

        q.Push(5);
        q.Push(10);

        assert_true(q.Size() == 2, "Size: Queue size is not equal to 2 after pushing 2 elements");

        q.Pop();

        assert_true(q.Size() == 1, "Size: Queue size is not equal to 1 after popping 1 element");
    }

    void TestFront() {
        dsa::Queue<int> q{};

        q.Push(5);
        q.Push(10);

        assert_true(q.Front() == 5, "Front: Queue front is not equal to 5");
        q.Pop();
        assert_true(q.Front() == 10, "Front: Queue front is not equal to 10 after popping front");
    }

    void TestBack() {
        dsa::Queue<int> q{};

        q.Push(5);

        assert_true(q.Back() == 5, "Back: Queue back is not equal to 5");
        q.Push(10);
        assert_true(q.Back() == 10, "Back: Queue back is not equal to) 10 after pushing 10");
    }

    void TestPush() {
        dsa::Queue<int> q{};

        q.Push(5);
        q.Push(10);

        assert_true(q.Front() == 5, "Push: Queue front is not equal to 5");
        assert_true(q.Back() == 10, "Push: Queue back is not equal to 10");
        assert_true(q.Size() == 2, "Push: Queue size is not 2");

        q.Push(15);

        assert_true(q.Size() == 3, "Push: Queue size is not 3");

        q.Pop();

        assert_true(q.Size() == 2, "Push: Queue size is not 2");
    }

    void TestEmplace() {
        struct Vector2D {
            int x;
            int y;

            Vector2D() : x{0}, y{0} {}
            explicit Vector2D(const int scalar) : x{scalar}, y{scalar} {}
            explicit Vector2D(const int x, const int y) : x{x}, y{y} {}
        };

        dsa::Queue<Vector2D> q{};

        q.Emplace(5, 10);
        q.Emplace(5);
        q.Emplace();

        assert_true(q.Front().x, 5, "Emplace operator value is not equal to value 1");
        assert_true(q.Front().y, 10, "Emplace operator value is not equal to value 2");
        assert_true(q.Back().x, 0, "Emplace operator value is not equal to value 3");
        assert_true(q.Back().y, 0, "Emplace operator value is not equal to value 4");
    }

    void TestPop() {
        dsa::Queue<int> q{};

        q.Push(5);
        q.Push(10);
        q.Push(15);

        assert_true(q.Pop() == 5, "Pop: Pop value is not equal to 5");
        assert_true(q.Pop() == 10, "Pop: Pop value is not equal to 10");
        assert_true(q.Pop() == 15, "Pop: Pop value is not equal to 15");
        assert_true(q.IsEmpty(), "Pop: Queue is not empty");
    }

    void TestSwap() {
        dsa::Queue<int> q1{};
        dsa::Queue<int> q2{};

        q1.Push(5);
        q1.Push(10);

        q2.Push(15);
        q2.Push(20);
        q2.Push(25);

        assert_true(q1.Front() == 5, "Swap: Queue 1 front is not equal to 5 before swap");
        assert_true(q1.Back() == 10, "Swap: Queue 1 back is not equal to 10 before swap");
        assert_true(q2.Front() == 15, "Swap: Queue 2 front is not equal to 15 before swap");
        assert_true(q2.Back() == 25, "Swap: Queue 2 back is not equal to 25 before swap");

        q1.Swap(q2);

        assert_true(q1.Front() == 15, "Swap: Queue 1 front is not equal to 15 after swap");
        assert_true(q1.Back() == 25, "Swap: Queue 1 back is not equal to 25 after swap");
        assert_true(q2.Front() == 5, "Swap: Queue 2 front is not equal to 5 after swap");
        assert_true(q2.Back() == 10, "Swap: Queue 2 back is not equal to 10 after swap");
    }

    void run() {
        // Constructors
        TestDefaultConstructor();
        TestCopyConstructor();
        TestMoveConstructor();

        // Operators
        TestCopyAssignmentOperator();
        TestMoveAssignmentOperator();

        // Indexing
        TestFront();
        TestBack();

        // Helpers
        TestIsEmpty();
        TestSize();

        // Business logic
        TestPush();
        TestEmplace();
        TestPop();
        TestSwap();
    }
};


#endif //DATA_STRUCTURES_TESTQUEUE_H