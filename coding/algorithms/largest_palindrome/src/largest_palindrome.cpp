// Copyright (c) 2023, Piotr Staniszewski

#include "palindrome_number.hpp"

#include <benchmark/benchmark.h>

#include <iostream>
#include <tuple>

constexpr uint64_t MAX_N_DIGIT_NUMBER = 99U;
constexpr uint64_t MAX_PRODUCT_OF_N_DIGIT_NUMBER = MAX_N_DIGIT_NUMBER * MAX_N_DIGIT_NUMBER;

// Calculating all possibilities
auto brute_force()
{
    std::tuple<uint64_t, uint64_t, uint64_t> result = std::make_tuple(0U, 0U, 0U);
    uint64_t number1 = MAX_N_DIGIT_NUMBER;

    while (number1 > 0)
    {
        uint64_t number2 = MAX_N_DIGIT_NUMBER;
        while (number2 > 0)
        {
            auto product = number1 * number2;
            if (is_palindrome(product))
            {
                if (std::get<0>(result) < product)
                {
                    std::get<0>(result) = product;
                    std::get<1>(result) = number1;
                    std::get<2>(result) = number2;
                }
            }
            --number2;
        }
        --number1;
    }

    return result;
}

// Calculating all possibilities but skipping decrementing number2 after finding first palindrome
auto brute_force_better()
{
    std::tuple<uint64_t, uint64_t, uint64_t> result = std::make_tuple(0U, 0U, 0U);
    uint64_t number1 = MAX_N_DIGIT_NUMBER;

    while (number1 > 0)
    {
        uint64_t number2 = MAX_N_DIGIT_NUMBER;
        while (number2 > 0)
        {
            auto product = number1 * number2;
            if (is_palindrome(product))
            {
                if (std::get<0>(result) < product)
                {
                    std::get<0>(result) = product;
                    std::get<1>(result) = number1;
                    std::get<2>(result) = number2;

                    // It won't get bigger for lower number2
                    break;
                }
            }
            --number2;
        }
        --number1;
    }

    return result;
}

// First idea of solving the problem
auto first_idea()
{
    bool found {false};
    std::tuple<uint64_t, uint64_t, uint64_t> result = std::make_tuple(0U, 0U, 0U);

    uint64_t n = MAX_PRODUCT_OF_N_DIGIT_NUMBER;
    while (n > 0)
    {
        if (is_palindrome(n))
        {
            uint64_t number1 = MAX_N_DIGIT_NUMBER;
            while (number1--)
            {
                uint64_t number2 = n / number1;

                if (number2 > MAX_N_DIGIT_NUMBER)
                {
                    break;
                }

                if (n % number1 == 0)
                {
                    std::get<0>(result) = n;
                    std::get<1>(result) = number1;
                    std::get<2>(result) = number2;
                    found = true;
                    break;
                }
            }
        }
        --n;
        if (found) break;
    }

    return result;
}

static void benchmark_brute_force(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::tuple<uint64_t, uint64_t, uint64_t> result = brute_force();
        // std::cout << std::get<0>(result) << " = " << std::get<1>(result) << " * " << std::get<2>(result) << std::endl;
    }
}
BENCHMARK(benchmark_brute_force);

static void benchmark_brute_force_better(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::tuple<uint64_t, uint64_t, uint64_t> result = brute_force_better();
        // std::cout << std::get<0>(result) << " = " << std::get<1>(result) << " * " << std::get<2>(result) << std::endl;
    }
}
BENCHMARK(benchmark_brute_force_better);

static void benchmark_first_idea(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::tuple<uint64_t, uint64_t, uint64_t> result = first_idea();
        //std::cout << std::get<0>(result) << " = " << std::get<1>(result) << " * " << std::get<2>(result) << std::endl;
    }
}
BENCHMARK(benchmark_first_idea);

BENCHMARK_MAIN();
