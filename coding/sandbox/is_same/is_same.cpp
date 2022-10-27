// Copyright (c) 2022, Piotr Staniszewski

#include <iostream>
#include <cassert>

template <class T, class X>
struct is_same
{
    static const bool value = false;
};

template <class T>
struct is_same<T, T>
{
    static const bool value = true;
};

template<class T, class X>
constexpr const bool is_same_v = is_same<T, X>::value;

template <class T, class X>
void test_is_same()
{
    std::cout << "Is " << typeid(T).name() << " same as " << typeid(X).name()
              << " : " << std::boolalpha << is_same_v<T, X> << "\n";

    static_assert(is_same_v<T, X> == std::is_same_v<T, X>);
}

struct A {};
class B {};
class C : public B {};

int main()
{
    std::cout << "Is type T same as X?\n";

    test_is_same<int, double>();
    test_is_same<int, int>();
    test_is_same<size_t, uint32_t>();
    test_is_same<char, char*>();
    test_is_same<char, uint8_t>();
    test_is_same<uint8_t, uint8_t>();
    test_is_same<A, B>();
    test_is_same<A, C>();
    test_is_same<B, C>();
    test_is_same<C, C>();

    return EXIT_SUCCESS;
}
