// Copyright (c) 2024, Piotr Staniszewski

export module my_module;

import <iostream>;
import <concepts>;

export void info() { std::cout << "my_module info() called\n"; }

export
{
    int add(int a, int b) { return a + b; }
    int sub(int a, int b) { return a - b; }
}

export namespace helper
{

    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <Integral T>
    void print(T val)
    {
        std::cout << val << "\n";
    }
}
