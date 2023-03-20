// Copyright (c) 2022, Piotr Staniszewski

#include <iostream>
#include <type_traits>

namespace Helper
{
    template <typename B>
    std::true_type test_convertible(const volatile B *);
    template <typename>
    std::false_type test_convertible(const volatile void *);

    template <typename, typename>
    auto test_is_base_of(...) -> std::true_type;
    
    template <typename B, typename D>
    auto test_is_base_of(int) -> decltype(test_convertible<B>(static_cast<D *>(nullptr)));
}

template <typename Base, typename Derived>
struct is_base_of : std::integral_constant<
                        bool,
                        std::is_class<Base>::value && std::is_class<Derived>::value &&decltype(Helper::test_is_base_of<Base, Derived>(0))::value>
{
};

template <class D, class B>
constexpr bool is_derived_from(const D &, const B &)
{
    return std::is_base_of_v<B, D>;
}

struct Base_One
{
    virtual ~Base_One() = default;
};

class Base_Two
{
public:
    virtual ~Base_Two() = default;
};

class Derived_One : public Base_One
{
};

class Derived_Two : public Base_Two
{
};

class Derived_Three : public Derived_One, public Derived_Two
{
};

template <class D, class B>
void test_is_derived(const D &derived, const B &base)
{
    std::cout << "Is " << typeid(derived).name() << " derived from " << typeid(base).name()
              << " : " << std::boolalpha << is_derived_from(derived, base) << "\n";
}

int main()
{
    std::cout << "Is class D derived from class B?\n";

    Base_One base_one;
    Base_Two base_two;
    Derived_One derived_one;
    Derived_Two derived_two;
    Derived_Three derived_three;

    test_is_derived(derived_one, base_one);
    test_is_derived(derived_one, base_two);
    test_is_derived(derived_two, base_one);
    test_is_derived(derived_two, base_two);
    test_is_derived(derived_three, base_one);
    test_is_derived(derived_three, base_two);

    return EXIT_SUCCESS;
}
