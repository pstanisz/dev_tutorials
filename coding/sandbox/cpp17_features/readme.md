# C++17 features

Examples of **C++17** features.

## Char literals

Available since **C++11**, except of `u8'` added by **C++17**.

```cpp
    // 'a' in UTF-8
    const char c1 = u8'a';      // UTF-8:  a       61

    // 'a' in UTF-16
    const char16_t c2 = u'a';   // UTF-16: 61      61 00

    // 'a' in UTF-32
    const char32_t c3 = U'a';   // UTF-32: 61      61 00 00 00

    // 'a' in wide character literal
    const wchar_t c4 = L'a';    // Wide:   61      61 00 00 00
```

## noexcept

`noexcept` is now a part of type system

```cpp
    // error: invalid conversion from ‘int (*)()’ to ‘int (*)() noexcept’
    // int (*foo_ptr)() noexcept = foo;
    // std::cout << "foo via functor: " << foo_ptr() << "\n";

    int (*bar_ptr)() noexcept = bar;
    std::cout << "bar via functor: " << bar_ptr() << "\n";
```

## Lambda capture of `*this`

Possible in **C++17** to capture `*this` by copy

```cpp
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
```

## constexpr if and lambda

`constexpr if` can be used, as well as `constexpr` lambdas

```cpp
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
```

## Structured bindings

Binds the specified names to subobjects or elements of array

```cpp
    struct A
    {
        int m_x{5};
        std::string m_text{"five"};
    };

    // Binding to tuple values
    std::tuple<int, char, double> tup(1, 'z', 3.14);
    const auto& [x, y, z] = tup;
    std::cout << x << ", " << y << ", " << z << "\n";       // 1, z, 3.14

    // Binding to data members
    A obj;
    const auto [a, b] = obj;
    std::cout << a << ", " << b << "\n";                    // 5, five

    // Binding to array items
    float floats[3] = {1.1, 2.2, 3.3};
    auto [f1, f2, f3] = floats;
    std::cout << f1 << ", " << f2 << ", " << f3 << "\n";    // 1.1, 2.2, 3.3
```

## If and switch initializers

`if` with initializer

```cpp
    std::vector<int> ints = {1, 5, 3, 9, 7, 6, 8, 2, 1, 0};
    if (auto it = std::find(std::begin(ints), std::end(ints), 9); it != std::end(ints))
    {
        std::cout << "9 found in position: " << std::distance(std::begin(ints), it) << "\n";    // 9 found in position: 3
    }
```

`switch` with initializer

```cpp
    std::srand(std::time(nullptr));
    switch (int i = std::rand() % 2; i)
    {
    case 0:
        std::cout << "0\n";
        break;
    default:
        std::cout << "1\n";
    }
```

## Mandatory elision of copy/move operations

Return value optimization is mandatory

## Fold expressions

Reduces (folds) a parameter pack using a binary operator

```cpp
    template<typename T, typename ...Args>
    auto mean(const T& value, const Args&... values)
    {
        constexpr auto size = 1 + sizeof...(values);
        return (value + ... + values) / size;
    }

    std::cout << "mean of floats: " << mean(1.1f, 3.3f, 5.5f) << "\n";  // 3.3
    std::cout << "mean of ints: " << mean(3, 9, 6) << "\n";             // 6
    std::cout << "mean of mix: " << mean(1.1f, 4U, 7.2) << "\n";        // 4.1
    
    // Won't compile: candidate expects at least 1 argument, 0 provided
    //mean();
```