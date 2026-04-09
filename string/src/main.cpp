//
// Created by Dorza on 4/9/2026.
//

#include <iostream>

#include "../include/String.h"

int main() {
    // String str{"Hello world!"};
    // String str2{"Hello world!"};

    String str = "Hello world!";
    std::cout << str << std::endl;

    const String str2 = "Hello world!";

    if (str == str2) {
        std::cout << "str and str2 are equal" << std::endl;
    } else {
        std::cout << "str and str2 are not equal" << std::endl;
    }

    std::cout << "str capacity: " << str.capacity() << std::endl;
    // String str3 = " WHasdhashdahsdhashdhashdhashdhashdhashdhashdahshd ";
    // str += str3;
    std::cout << str.capacity() << std::endl;

    for (char c : str) {
        std::cout << c << c;
    }

    // std::cout << "Is str == str2: " << (str == str2) << std::endl;

    // std::cout << "Length of Hello world!: " << String::strlen("Hello world!") << std::endl;

    return 0;
}
