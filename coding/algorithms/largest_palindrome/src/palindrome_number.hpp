#pragma once

#include <type_traits>

template <typename T>
constexpr bool is_palindrome(T number)
{
    static_assert(std::is_integral<T>::value, "Integral type required.");

    return false;
}
