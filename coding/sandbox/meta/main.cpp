// Copyright (c) 2023, Piotr Staniszewski

#include <iostream>
#include <type_traits>
#include <typeindex>

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
using streamable = typename std::enable_if_t<sizeof(std::declval<std::ostream &>() << std::declval<Small &>()) != 0>;

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

int main(int /*argc*/, char * /*argv*/[])
{
    std::cout << "templates" << std::endl;

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
    //print_if_convertible(small, big);

    std::cout << small << "\n";
    // std::cout << big << "\n";

    // sizeof(std::declval<std::ostream&>() << std::declval<Small&>()) != 0;
    print_if_streamable(i);
    print_if_streamable(j);
    print_if_streamable(small);
    // print_if_streamable(big);
    print_if_streamable(s);

    return 0;
}