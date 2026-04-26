//
// Created by Dorza on 4/19/2026.
//

#ifndef TESTQUEUE_H
#define TESTQUEUE_H

#include "../Queue.h"
#include "../utils/TempAssert.h"

class TestQueue {
public:
    TestQueue() {
        run();
    }

    ~TestQueue() = default;

private:
    void TestHappyFlow() {
        dsa::Queue<int> q{};

        q.Push(5);
        q.Push(10);
        q.Push(15);

        int expect{5};

        assert_true(q.IsEmpty(), false, "Queue is not empty");

        while (!q.IsEmpty()) {
            const int value = q.Pop();

            std::cout << "Pop value: " << value << std::endl;

            assert_true(value, expect, "Pop value is not equal to a divisible by 5");

            expect += 5;
        }

        assert_true(q.IsEmpty(), true, "Queue is not empty");
    }

    void run() {
        TestHappyFlow();
    }
};

#endif // TESTQUEUE_H
