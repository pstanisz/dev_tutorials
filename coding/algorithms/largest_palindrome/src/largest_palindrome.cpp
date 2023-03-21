// Copyright (c) 2023, Piotr Staniszewski

#include <iostream>

#include "palindrome_number.hpp"

constexpr uint64_t MAX_N_DIGIT_NUMBER = 99U;
constexpr uint64_t MAX_PRODUCT_OF_N_DIGIT_NUMBER = MAX_N_DIGIT_NUMBER * MAX_N_DIGIT_NUMBER;

uint64_t brute_force()
{
    uint64_t number1 = MAX_N_DIGIT_NUMBER;
    
    uint64_t largest_palindrome = 0U;
    while (number1 > 0) {
        uint64_t number2 = MAX_N_DIGIT_NUMBER;
        while (number2 > 0) {
            auto product = number1 * number2;
            if (is_palindrome(product)) {
                if (largest_palindrome < product) {
                    largest_palindrome = product;
                    break;
                }
            }
            --number2;
        } 
        --number1;
    }

    return largest_palindrome;
}

int main()
{
    std::cout << "largest palindrome\n";

    std::cout << "Calculated with brute force: " << brute_force() << std::endl;

    return EXIT_SUCCESS;
}
