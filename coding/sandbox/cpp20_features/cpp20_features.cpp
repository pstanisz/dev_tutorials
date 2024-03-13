// Copyright (c) 2023, Piotr Staniszewski

#include <iostream>
#include <vector>
#include <span>

import my_module;

void print(std::span<int> numbers) {
    bool first{true};
    for (auto number : numbers) {
        if (first) {
            std::cout << number;
            first = false;
        } else {
            std::cout << ", " << number;
        }
    }

    std::cout << "\n";
}

int main()
{
    std::cout << "C++20 features\n";

    // Three-way comparison (spaceship operator)
    {
        int x = 10;
        int y = 20;

        auto res = x <=> y;

        if (res < 0) {
            std::cout << x << " is less than " << y << "\n";
        } else if (res > 0) {
            std::cout << x << " is greater than " << y << "\n";
        } else if (res == 0) {
            std::cout << x << " is equal to " << y << "\n";
        } else {
            std::cout << "unordered\n";
        }
    }

    {
        std::string_view x = "hello";
        std::string_view y = "Hellno";

        auto res = x <=> y;

        if (res < 0) {
            std::cout << x << " is less than " << y << "\n";
        } else if (res > 0) {
            std::cout << x << " is greater than " << y << "\n";
        } else if (res == 0) {
            std::cout << x << " is equal to " << y << "\n";
        } else {
            std::cout << "unordered\n";
        }
    }

    {
        std::vector<int> x = {1,2,3,4,5};
        std::vector<int> y = {1,2,3,5,5};

        auto res = x <=> y;

        if (res < 0) {
            std::cout << "less" << "\n";
        } else if (res > 0) {
            std::cout << "greater" << "\n";
        } else if (res == 0) {
            std::cout << "equal" << "\n";
        } else {
            std::cout << "unordered\n";
        }
    }

    // span
    int numbers_a[] = {-1,0,1};
    print(numbers_a);

    std::vector<int> numbers_b {1,2,3};
    print(numbers_b);

    std::array<int, 4> numbers_c {1,2,3,4};
    print(numbers_c);

    // Modules
    info(); // from my_module

    return EXIT_SUCCESS;
}
