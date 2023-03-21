// Copyright (c) 2023, Piotr Staniszewski

#include "palindrome_number.hpp"

#include <benchmark/benchmark.h>

#include <iostream>
#include <tuple>

constexpr uint64_t MAX_N_DIGIT_NUMBER = 99U;
constexpr uint64_t MAX_PRODUCT_OF_N_DIGIT_NUMBER = MAX_N_DIGIT_NUMBER * MAX_N_DIGIT_NUMBER;

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

static void benchmark_brute_force(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::tuple<uint64_t, uint64_t, uint64_t> result = brute_force();
        std::cout << std::get<0>(result) << " = " << std::get<1>(result) << " * " << std::get<2>(result) << std::endl;
    }
}
BENCHMARK(benchmark_brute_force);

BENCHMARK_MAIN();
