// Copyright (c) 2022, Piotr Staniszewski

#include <iostream>
#include <iomanip>

namespace char_literals
{
    template <typename T>
    void dump(const T c)
    {
        const uint8_t *data{reinterpret_cast<const uint8_t *>(&c)};

        std::cout << std::hex << std::setfill('0');
        for (auto idx{0U}; idx < sizeof(T); ++idx)
        {
            std::cout << std::setw(2) << static_cast<unsigned>(data[idx]) << " ";
        }

        std::cout << std::dec << std::endl;
    }

    template <typename T>
    void print(std::string_view desc, const T c)
    {
        std::cout << desc << ":\t" << c << "\t";
        dump(c);
    }

    void experiment();
}

namespace noexcept_changed
{
    int foo() { return 1; }
    int bar() noexcept { return 10; }

    void experiment();
}

namespace lambda_capture
{
    struct A
    {
        void print() const { std::cout << "print\n"; }
        void method()
        {
            auto lambda = [=, *this]
            { this->print(); };
            lambda();
        };
    };

    void experiment();
}

namespace constexpr_features
{
    template <typename T>
    constexpr T increment_by_one(const T input)
    {
        // constexpr lambda example
        auto is_incrementable_lambda = []() constexpr->bool
        {
            return std::is_arithmetic_v<T>;
        };

        // constexpr if example
        if constexpr (is_incrementable_lambda())
        {
            return input + static_cast<T>(1);
        };

        return input;
    }

    constexpr int MY_INT = 10;
    constexpr int MY_OUTPUT_INT = increment_by_one(MY_INT); // evaluated to 11

    constexpr double MY_DOUBLE = 3.14;
    constexpr double MY_OUTPUT_DOUBLE = increment_by_one(MY_DOUBLE); // evaluated to 4.14

    constexpr const double *MY_PTR = &MY_DOUBLE;
    constexpr const double *MY_OUTPUT_PTR = increment_by_one(MY_PTR); // unchanged and compiles
}

int main()
{
    std::cout << "C++17 features\n";

    // Experiments with char literals
    char_literals::experiment();

    // noexcept as a part of type system
    noexcept_changed::experiment();

    // Lambda capture of *this
    lambda_capture::experiment();

    return EXIT_SUCCESS;
}

void char_literals::experiment()
{
    // 'a' in UTF-8
    const char c1 = u8'a';
    print("UTF-8", c1);

    // 'a' in UTF-16
    const char16_t c2 = u'a';
    print("UTF-16", c2);

    // 'a' in UTF-32
    const char32_t c3 = U'a';
    print("UTF-32", c3);

    // 'a' in wide character literal
    const wchar_t c4 = L'a';
    print("Wide", c4);
}

void noexcept_changed::experiment()
{
    // error: invalid conversion from ‘int (*)()’ to ‘int (*)() noexcept’
    // int (*foo_ptr)() noexcept = foo;
    // std::cout << "foo via functor: " << foo_ptr() << "\n";

    int (*bar_ptr)() noexcept = bar;
    std::cout << "bar via functor: " << bar_ptr() << "\n";
}

void lambda_capture::experiment()
{
    A obj;
    obj.method();
}
