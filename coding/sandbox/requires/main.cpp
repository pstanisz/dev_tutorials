// Copyright (c) 2023, Piotr Staniszewski

#include <iostream>
#include <type_traits>

// No restrictions
template <typename T>
class A
{
public:
    A(T val) : m_val{val} {}

private:
    T m_val;
};

// Only compilable for integral types
template <typename T, std::enable_if_t<std::is_integral<T>::value, bool> = true>
class B
{
public:
    B(T val) : m_val{val} {}

private:
    T m_val;
};

// Only compilable for integral types
template <typename T>
class C
{
public:
    static_assert(std::is_integral<T>::value);
    C(T val) : m_val{val} {}

private:
    T m_val;
};

// Only compilable for integral types
template <typename T>
requires std::is_integral<T>::value
class D
{
public:
    D(T val) : m_val{val} {}

private:
    T m_val;
};

int main(int /*argc*/, char * /*argv*/[])
{
    std::cout << "requires" << std::endl;

    A a1{15};
    // A a2{std::string("hello")};
    // A a3{0.5};

    B b1{15};
    // B b2{std::string("hello")};
    // B b3{0.5};

    C c1{15};
    // C c2{std::string("hello")};
    // C c3{0.5};

    D d1{15};
    // D d2{std::string("hello")};
    // D d3{0.5};

    return 0;
}