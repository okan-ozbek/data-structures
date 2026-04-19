//
// Created by Dorza on 4/19/2026.
//

#ifndef TEMPASSERT_H
#define TEMPASSERT_H


#include <iostream>

static int TEST_ERRORS = 0;
static int TEST_RAN = 0;

template<typename T>
void assert_true(T a, T b, const std::string& error_message) {
    if (a != b) {
        std::cout << "Assertion failed: " << error_message << std::endl;
        ++TEST_ERRORS;
    }

    ++TEST_RAN;
}

inline void assert_true(const bool a, const std::string& error_message) {
    if (!a) {
        std::cout << "Assertion failed: " << error_message << std::endl;
        ++TEST_ERRORS;
    }

    ++TEST_RAN;
}


#endif // TEMPASSERT_H