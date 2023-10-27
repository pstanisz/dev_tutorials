// Copyright (c) 2023, Piotr Staniszewski

#include <iostream>
#include <type_traits>

namespace playground {

template<bool Condition, typename T = void>
struct enable_if {
    using type = T;
};

template<typename T>
struct enable_if<false, T> {
};

template <bool Condition, typename T = void>
using enable_if_t = typename enable_if<Condition, T>::type;

template<typename T>
using Integral = enable_if_t<std::is_integral<T>::value>;

template<typename T, typename = Integral<T>>
class Printer
{
public:
Printer(T val) : m_val{val} {}
void print() {
    std::cout << m_val << std::endl;
}
    

private:
    T m_val;
};

}

int main()
{
    std::cout << "tempate fun\n";

    using namespace playground;
    auto pr1 = Printer(10);
    pr1.print();

    auto pr2 = Printer(0.5);
    pr2.print();


    return EXIT_SUCCESS;
}
