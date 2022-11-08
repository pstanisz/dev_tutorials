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
