#pragma once

#include <type_traits>

template <typename T>
constexpr bool is_palindrome(T number)
{
    static_assert(std::is_integral<T>::value, "Integral type required.");
    const T orig_number = number;

    T rev_number = 0;
    while (number > 0)
    {
        rev_number = rev_number * 10 + number % 10;
        number = number / 10;
    }
    return rev_number == orig_number;
}
