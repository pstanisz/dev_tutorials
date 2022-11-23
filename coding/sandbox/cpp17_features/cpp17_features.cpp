// Copyright (c) 2022, Piotr Staniszewski

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

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

namespace structured_bindings
{
    struct A
    {
        int m_x{5};
        std::string m_text{"five"};
    };

    void experiment();
}

namespace if_switch_initializer
{
    void experiment();
}

namespace templates
{
    template <typename... Args>
    struct check_condition : std::true_type
    {
    };

    template <typename Arg, typename... Args>
    struct check_condition<Arg, Args...> : std::conditional_t<Arg::value, check_condition<Args...>, std::false_type>
    {
    };

    // Fold expressions
    template <typename T, typename... Args>
    auto mean(const T &value, const Args &...values)
    {
        static_assert(check_condition<std::is_arithmetic<T>>::value);
        static_assert(check_condition<std::is_arithmetic<Args>...>::value);

        constexpr auto size = 1 + sizeof...(values);
        return (value + ... + values) / size;
    }

    template <typename T>
    struct Fruit
    {
        Fruit(T) {}

        T m_value;
    };

    // Non-type template parameters declared from auto
    template <auto N>
    struct Vegetable
    {
        Vegetable()
        {
            std::cout << "Vege with " << N << std::endl;
        }
    };

    void experiment();
}

namespace namespaces
{
    namespace simple::nested
    {
        void experiment() {}
    }
}

namespace attributes
{
    void experiment(int i);

    [[nodiscard]] int foo() { return 5; };
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

    // Structured bindings
    structured_bindings::experiment();

    // If and switch initializers
    if_switch_initializer::experiment();

    // New template features
    templates::experiment();

    // Namespaces
    namespaces::simple::nested::experiment();

    // Attributes
    attributes::experiment(5);

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

void structured_bindings::experiment()
{
    // Binding to tuple values
    std::tuple<int, char, double> tup(1, 'z', 3.14);
    const auto &[x, y, z] = tup;
    std::cout << x << ", " << y << ", " << z << "\n";

    // Binding to data members
    A obj;
    const auto [a, b] = obj;
    std::cout << a << ", " << b << "\n";

    // Binding to array items
    float floats[3] = {1.1, 2.2, 3.3};
    auto [f1, f2, f3] = floats;
    std::cout << f1 << ", " << f2 << ", " << f3 << "\n";
}

void if_switch_initializer::experiment()
{
    std::vector<int> ints = {1, 5, 3, 9, 7, 6, 8, 2, 1, 0};
    if (auto it = std::find(std::begin(ints), std::end(ints), 9); it != std::end(ints))
    {
        std::cout << "9 found in position: " << std::distance(std::begin(ints), it) << "\n";
    }

    std::srand(std::time(nullptr));
    switch (int i = std::rand() % 2; i)
    {
    case 0:
        std::cout << "0\n";
        break;
    default:
        std::cout << "1\n";
    }
}

void templates::experiment()
{
    std::cout << "mean of floats: " << mean(1.1f, 3.3f, 5.5f) << "\n";
    std::cout << "mean of ints: " << mean(3, 9, 6) << "\n";
    std::cout << "mean of mix: " << mean(1.1f, 4U, 7.2) << "\n";

    // Won't compile: error: static assertion failed: static_assert(check_condition<std::is_arithmetic<Args>...>::value);
    // int x = 5;
    // const int* ptr = &x;
    // std::cout << "mean of non-arithmetic : " << mean(1, ptr, 3.14) << "\n";

    // Won't compile: candidate expects at least 1 argument, 0 provided
    // mean();

    // Class template deduction
    std::pair p = {2.5, 1U}; // types deduced from the type of initializer
    auto [p1, p2] = p;
    static_assert(std::is_same_v<decltype(p1), double>);
    static_assert(std::is_same_v<decltype(p2), unsigned>);

    std::tuple t = {'c', 10, "foo"};
    auto [t1, t2, t3] = t;
    static_assert(std::is_same_v<decltype(t1), char>);
    static_assert(std::is_same_v<decltype(t2), int>);
    static_assert(std::is_same_v<decltype(t3), const char *>);

    // Deduction from new expression
    auto s = new Fruit{5};
    static_assert(std::is_same_v<decltype(s->m_value), int>);
    delete s;

    // Non-type template parameters declared with auto
    static constexpr int value{10};
    Vegetable<5> v1;
    Vegetable<'x'> v2;
    Vegetable<&value> v3;
}

void attributes::experiment([[maybe_unused]] int i)
{
    int z = 2;
    switch (z)
    {
    case 1:
        std::cout << "bla\n";
        [[fallthrough]];
    case 2:
        std::cout << "one or two\n";
    default:
        break;
    }

    // Won't compile: ignoring return value of ‘int attributes::foo()’, declared with attribute ‘nodiscard’
    //foo();
}
