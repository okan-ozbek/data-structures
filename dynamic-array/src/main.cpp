//
// Created by Dorza on 4/7/2026.
//

#include <iostream>

#include "../include/DynamicArray.h"

int main() {
    /**
     * This is mainly for testing use cases until I find a way to create proper tests. I am currently still
     * reading up on C++ and how it works, so not everything is as optimal as it could be but I am happy
     * with the result so far. I will revisit this sometime in the future.
     */

    std::cout << "[PART 1]" << std::endl;
    DynamicArray dynamicArray(15);

    std::cout << dynamicArray.size() << std::endl;
    if (dynamicArray.empty() == false) {
        for (int i{}; i < dynamicArray.size(); ++i) {
            std::cout << i + 1 << ". " << dynamicArray[i] << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << "[PART 2]" << std::endl;

    dynamicArray.push_back(1);
    dynamicArray.push_back(2);

    std::cout << "Pushed 2 elements with values 1 and 2" << std::endl;
    std::cout << "Size: " << dynamicArray.size() << ", capacity: " << dynamicArray.capacity() << std::endl;

    const auto value = dynamicArray.pop_back();

    std::cout << "Popped element with value: " << value << std::endl;
    std::cout << "Size: " << dynamicArray.size() << std::endl;

    std::cout << std::endl;
    std::cout << "[PART 3]" << std::endl;

    for (int i{}; i < 10; i++) {
        dynamicArray.push_back(i);
    }

    std::cout << "Pushed 10 elements with values 0 to 9" << std::endl;
    std::cout << "Size: " << dynamicArray.size() << ", capacity: " << dynamicArray.capacity() << std::endl;

    dynamicArray[3] = 10;
    std::cout << "Value at index 3: " << dynamicArray[3] << std::endl;

    std::cout << std::endl;
    std::cout << "[PART 4]" << std::endl;

    dynamicArray.clear();
    std::cout << "Cleared the dynamic array" << std::endl;
    std::cout << "Size: " << dynamicArray.size() << ", capacity: " << dynamicArray.capacity() << std::endl;
    std::cout << "Dynamic array is empty: " << (dynamicArray.empty() ? "true" : "false") << std::endl;

    std::cout << std::endl;
    std::cout << "[PART 5]" << std::endl;

    DynamicArray dynamicArray2(10, 10);

    std::cout << "Created a dynamic array with 10 elements initialized with value 10" << std::endl;
    std::cout << "Size: " << dynamicArray2.size() << ", capacity: " << dynamicArray2.capacity() << std::endl;

    for (int i{}; i < dynamicArray2.size(); ++i) {
        std::cout << i + 1 << ". " << dynamicArray2[i] << std::endl;
    }

    dynamicArray2.push_back(1);

    std::cout << "Pushed 1 element with value 1" << std::endl;
    std::cout << "Size: " << dynamicArray2.size() << ", capacity: " << dynamicArray2.capacity() << std::endl;

    for (int i{}; i < dynamicArray2.size(); ++i) {
        std::cout << i + 1 << ". " << dynamicArray2[i] << std::endl;
    }

    std::cout << std::endl;

    return 0;
}
