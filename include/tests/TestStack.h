//
// Created by Dorza on 4/19/2026.
//

#ifndef TESTSTACK_H
#define TESTSTACK_H

#include "../Stack.h"
#include "../utils/TempAssert.h"

class TestStack {
public:
    TestStack() {
        run();
    }

    ~TestStack() = default;

private:
    void test_happy_flow() {
        Stack<int> q{};

        std::cout << "Queue size: " << q.size() << std::endl;

        q.push(5);
        q.push(10);
        q.push(15);

        int expect{15};

        while (q.size() > 0) {
            const int value = q.pop();

            std::cout << "Pop value: " << value << std::endl;

            assert_true(value, expect, "Pop value is not equal to a divisible by 5");

            expect -= 5;
        }

        std::cout << "Stack size: " << q.size() << std::endl;
    }

    void run() {
        test_happy_flow();
    }
};

#endif // TESTSTACK_H

