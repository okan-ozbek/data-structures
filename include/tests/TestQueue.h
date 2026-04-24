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
    void test_happy_flow() {
        Queue<int> q{};

        q.push(5);
        q.push(10);
        q.push(15);

        int expect{5};

        while (q.size() > 0) {
            const int value = q.pop();

            //std::cout << "Pop value: " << value << std::endl;

            assert_true(value, expect, "Pop value is not equal to a divisible by 5");

            expect += 5;
        }

        //std::cout << "Queue size: " << q.size() << std::endl;
    }

    void run() {
        test_happy_flow();
    }
};

#endif // TESTQUEUE_H

