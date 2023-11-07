// Copyright (c) 2023, Piotr Staniszewski

#include <iostream>
#include <type_traits>
#include <typeindex>
#include <array>
#include <vector>

template <typename T>
using small_type = typename std::enable_if_t<(sizeof(T) < 8U)>;

template <typename T>
using large_type = typename std::enable_if_t<(sizeof(T) >= 8U)>;

template <typename T, typename = small_type<T>>
void process(T)
{
    std::cout << "small type processed\n";
}

template <typename T, typename = large_type<T>>
void process(const T &)
{
    std::cout << "large type processed\n";
}

template <typename From, typename To>
using convertible = typename std::enable_if_t<std::is_convertible<From, To>::value>;

template <typename T, typename U, typename = convertible<T, U>>
void print_if_convertible(T t, U u)
{
    std::cout << "type " << typeid(t).name() << " is convertible to " << typeid(u).name() << "\n";
}

struct Small
{
    uint8_t m_x;
    uint8_t m_y;

    friend std::ostream &operator<<(std::ostream &os, const Small &s);
};

struct Big
{
    uint32_t m_x;
    uint32_t m_y;

    // friend std::ostream& operator<<(std::ostream& os, const Big& dt);
};

template <typename T>
using streamable = typename std::enable_if_t<sizeof(std::declval<std::ostream &>() << std::declval<T &>()) != 0>;

std::ostream &operator<<(std::ostream &os, const Small &s)
{
    os << "{" << static_cast<int>(s.m_x) << ", " << static_cast<int>(s.m_y) << "}";

    return os;
}

template <typename T, typename = streamable<T>>
void print_if_streamable(T t)
{
    std::cout << t << "\n";
}

// Function template - no limitation
template <typename T>
void print_number_a(T number)
{
    std::cout << number << "\n";
}

// Function template - limited to integral types by enable_if
template <typename T, std::enable_if_t<std::is_integral<T>::value, bool> = true>
void print_number_b(T number)
{
    std::cout << number << "\n";
}

// Function template - limited to floating types by static_assert
template <typename T>
void print_number_c(T number)
{
    static_assert(std::is_floating_point<T>::value == true);
    std::cout << number << "\n";
}

// Function template - limited to integral types by requires
template <typename T>
    requires std::is_integral_v<T>
void print_number_d(T number)
{
    std::cout << number << "\n";
}

// Function template - limited to floting types by concept + requires
template <typename T>
concept Floating_point = std::is_floating_point_v<T>;

template <Floating_point T>
void print_number_e(T number)
{
    std::cout << number << "\n";
}

// Function template - no limitation
template <typename T>
T doubled_a(T input)
{
    return input + input;
}

// Function template - limited to arithmetic types by enable_of
template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
T doubled_b(T input)
{
    return input + input;
}

// Function template - limited to arithmetic types by enable_if in return type
template <typename T>
auto doubled_c(T input) -> typename std::enable_if_t<std::is_arithmetic_v<T>, T>
{
    return input + input;
}

template <typename T, typename = void>
struct is_iterable : std::false_type
{
};

// Checks whether type is iterable
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::declval<T &>().begin()),
                                  decltype(std::declval<T &>().end()),
                                  decltype(std::declval<T &>().cbegin()),
                                  decltype(std::declval<T &>().cend()),
                                  decltype(std::declval<T &>().size())>> : std::true_type
{
};

// Helper for checking iterable type
template <typename T>
constexpr bool is_iterable_v = is_iterable<T>::value;

template <typename T, typename std::enable_if_t<is_iterable_v<T>, bool> = true>
void print_iterable(const T &iterable)
{
    for (const auto &item : iterable)
    {
        std::cout << item << ", ";
    }
    std::cout << "\n";
}

int main(int /*argc*/, char * /*argv*/[])
{
    std::cout << "meta" << std::endl;

    int i = 0;
    float j = 5.0;
    Small small{16, 32};
    Big big{1024, 4096};
    std::string s{"text"};

    process(i);
    process(j);
    process(small);
    process(big);
    process(s);

    // print_if_convertible(i, s);
    print_if_convertible(i, j);
    // print_if_convertible(small, big);

    std::cout << small << "\n";
    // std::cout << big << "\n";

    // sizeof(std::declval<std::ostream&>() << std::declval<Small&>()) != 0;
    print_if_streamable(i);
    print_if_streamable(j);
    print_if_streamable(small);
    // print_if_streamable(big);
    print_if_streamable(s);

    print_number_a(1);
    print_number_a(2UL);
    print_number_a(3.0);
    print_number_a(4.5f);

    print_number_b(1);
    print_number_b(2UL);
    // print_number_b(3.0);
    // print_number_b(4.5f);

    // print_number_c(1);
    // print_number_c(2UL);
    print_number_c(3.0);
    print_number_c(4.5f);

    print_number_d(1);
    print_number_d(2UL);
    // print_number_d(3.0);
    // print_number_d(4.5f);

    // print_number_e(1);
    // print_number_e(2UL);
    print_number_e(3.0);
    print_number_e(4.5f);

    std::cout << doubled_a(1) << "\n";
    std::cout << doubled_a(2.0f) << "\n";
    // std::cout << doubled_a("text") << "\n";

    std::cout << doubled_b(1) << "\n";
    std::cout << doubled_b(2.0f) << "\n";
    // std::cout << doubled_b("text") << "\n";

    std::cout << doubled_c(1) << "\n";
    std::cout << doubled_c(2.0f) << "\n";
    // std::cout << doubled_c("text") << "\n";

    std::array<int, 3> arr = {1, 2, 3};
    std::vector<int> vec = {4, 5, 6, 7};
    // int carr[] = {8, 9};
    // print_iterable(1);
    print_iterable(arr);
    print_iterable(vec);
    // print_iterable(carr);

    return 0;
}