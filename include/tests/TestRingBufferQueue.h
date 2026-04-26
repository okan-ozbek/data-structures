//
// Created by Dorza on 4/19/2026.
//

#ifndef TESTQUEUE_H
#define TESTQUEUE_H

#include "../RingBufferQueue.h"
#include "../utils/TempAssert.h"

class TestRingBufferQueue {
public:
    TestRingBufferQueue() {
        run();
    }

    ~TestRingBufferQueue() = default;

private:
    void TestDefaultConstructor() {
        dsa::RingBufferQueue<int> q{};

        assert_true(q.IsEmpty() == true, "Default Constructor: Default constructed queue is not empty");

        try {
            q.Pop();
            assert_true(false, "Default Constructor: Popping from empty queue should throw an exception");
        } catch (const std::out_of_range& e) {
            assert_true(true, "Default Constructor: Popping from empty queue does not throw correct error message");
        }
    }

    void TestCapacityConstructor() {
        dsa::RingBufferQueue<int> q{20};

        assert_true(q.IsEmpty(), "Capacity Constructor: Capacity constructor queue is not empty");
        assert_true(q.Capacity() == 20, "Capacity Constructor: Capacity constructor queue capacity is not equal to capacity");
    }

    void TestCopyConstructor() {
        dsa::RingBufferQueue<int> q{};
        q.Push(5);
        q.Push(10);
        
        dsa::RingBufferQueue copy{q};

        assert_true(q.IsEmpty() == false, "Copy Constructor: Copy constructed queue is empty");
        assert_true(copy.IsEmpty() == false, "Copy Constructor: Copy constructed queue is empty");

        const int value{ q.Pop() };
        const int copyValue{ copy.Pop() };

        assert_true(value == 5, "Copy Constructor: Pop value is not equal to 5");
        assert_true(copyValue == 5, "Copy Constructor: Pop copyValue is not equal to 5");
    }

    void TestMoveConstructor() {
        dsa::RingBufferQueue<int> q{};
        q.Push(5);
        q.Push(10);

        dsa::RingBufferQueue move{std::move(q)};

        const int moveValue = move.Pop();

        assert_true(q.IsEmpty(), "Move Constructor: Move constructed queue is not empty");
        assert_true(move.IsEmpty() == false, "Move Constructor: Move constructed queue is not empty");
        assert_true(moveValue == 5, "Move Constructor: Pop value is not equal to 5");
    }

    void TestCopyAssignmentOpertor() {
        dsa::RingBufferQueue<int> q{};
        q.Push(5);
        q.Push(10);

        dsa::RingBufferQueue<int> copy{};

        assert_true(copy.IsEmpty(), "Copy Assignment Operator: Copy assigned queue is not empty");

        copy = q;
        assert_true(copy.IsEmpty() == false, "Copy Assignment Operator: Copy assigned queue is empty");
        assert_true(copy.Pop() == 5, "Copy Assignment Operator: Pop value is not equal to 5");
    }

    void TestMoveAssignmentOperator() {
        dsa::RingBufferQueue<int> q{};
        q.Push(5);
        q.Push(10);

        dsa::RingBufferQueue<int> move{};

        assert_true(move.IsEmpty(), "Move Assignment Operator: Move assigned queue is not empty");
        assert_true(q.IsEmpty() == false, "Move Assignment Operator: Default constructed queue is empty");

        move = std::move(q);

        assert_true(move.IsEmpty() == false, "Move Assignment Operator: Move assigned queue is empty");
        assert_true(move.Pop() == 5, "Move Assignment Operator: Pop value is not equal to 5");
        assert_true(q.IsEmpty(), "Move Assignment Operator: Move assigned queue is not empty");
    }

    void TestPush() {
        dsa::RingBufferQueue<int> q{};
        q.Push(5);
        q.Push(10);
        q.Push(15);

        assert_true(q.Pop() == 5, "Push: Pop value is not equal to 5");
        assert_true(q.Pop() == 10, "Push: Pop value is not equal to 10");
        assert_true(q.Pop() == 15, "Push: Pop value is not equal to 15");

        assert_true(q.IsEmpty(), "Push: Queue is not empty");
    }

    void TestPop() {
        dsa::RingBufferQueue<int> q{};
        q.Push(5);
        q.Push(10);
        q.Push(15);

        assert_true(q.Pop() == 5, "Pop: Pop value is not equal to 5");
        assert_true(q.Pop() == 10, "Pop: Pop value is not equal to 10");
        assert_true(q.Pop() == 15, "Pop: Pop value is not equal to 15");

        assert_true(q.IsEmpty(), "Pop: Queue is not empty");
    }

    void TestIsEmpty() {
        dsa::RingBufferQueue<int> q{};

        assert_true(q.IsEmpty(), "IsEmpty: Default constructed queue is not empty");

        q.Push(5);
        assert_true(q.IsEmpty() == false, "IsEmpty: Queue is empty after push");

        q.Pop();
        assert_true(q.IsEmpty(), "IsEmpty: Queue is not empty after pop");
    }

    void run() {
        // Constructors
        TestDefaultConstructor();
        TestCapacityConstructor();
        TestCopyConstructor();
        TestMoveConstructor();

        // Opertators
        TestCopyAssignmentOpertor();
        TestMoveAssignmentOperator();

        // Functions
        TestPush();
        TestPop();
        TestIsEmpty();
    }
};

#endif // TESTQUEUE_H
