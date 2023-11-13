// Copyright (c) 2023, Piotr Staniszewski

#include <iostream>
#include <type_traits>
#include <typeindex>
#include <array>
#include <vector>
#include <map>
#include <span>

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

    void foo();

    friend std::ostream &operator<<(std::ostream &os, const Small &s);
};

// To check is_equality_comparable OK
bool operator==(const Small &, const Small &)
{
    return true;
}

struct Big
{
    uint32_t m_x;
    uint32_t m_y;

    // friend std::ostream& operator<<(std::ostream& os, const Big& dt);
};

// To check is_equality_comparable NOK
std::string operator==(const Big &, const Big &)
{
    return "abc";
}

template <typename T>
using streamable = typename std::enable_if_t<sizeof(std::declval<std::ostream &>() << std::declval<T &>()) != 0>;

std::ostream &operator<<(std::ostream &os, const Small &s)
{
    os << "{" << static_cast<int>(s.m_x) << ", " << static_cast<int>(s.m_y) << "}";

    return os;
}

struct No_default_ctor
{
    No_default_ctor(int) {}
};

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
                                  // decltype(std::declval<T &>().cbegin()),
                                  // decltype(std::declval<T &>().cend()),
                                  decltype(std::declval<T &>().size())>> : std::true_type
{
};

// Helper for checking iterable type
template <typename T>
constexpr bool is_iterable_v = is_iterable<T>::value;

// Function template limited to iterable types only
template <typename T, typename std::enable_if_t<is_iterable_v<T>, bool> = true>
void print_iterable(const T &iterable)
{
    for (const auto &item : iterable)
    {
        std::cout << item << ", ";
    }
    std::cout << "\n";
}

// Class template limited to iterable types only using static_assert
template <typename T>
class Only_iterable_a
{
public:
    static_assert(is_iterable_v<T> == true);

    Only_iterable_a(const T &) { std::cout << "Only_iterable_a with type: " << typeid(T).name() << "\n"; }
};

// Class template limited to iterable types only using enable_if
template <typename T, typename std::enable_if_t<is_iterable_v<T>, bool> = true>
class Only_iterable_b
{
public:
    Only_iterable_b(const T &) { std::cout << "Only_iterable_b with type: " << typeid(T).name() << "\n"; }
};

// convertible_to implemented with concepts
template <class FromT, class ToT>
concept convertible_to =
    std::is_convertible_v<FromT, ToT> &&
    requires {
        static_cast<ToT>(std::declval<FromT>());
    };

// convertible_to implemented with SFINAE
template <class FromT, class ToT, typename = void>
struct is_convertible_to : std::false_type
{
};

template <class FromT, class ToT>
struct is_convertible_to<FromT, ToT, std::void_t<decltype(std::is_convertible_v<FromT, ToT>), decltype(static_cast<ToT>(std::declval<FromT>()))>> : std::true_type
{
};

template <class FromT, class ToT>
constexpr bool is_convertible_to_v = is_convertible_to<FromT, ToT>::value;

template <typename T>
concept equality_comparable = requires(const std::remove_reference_t<T> &a, const std::remove_reference_t<T> b) {
    {
        a == b
    } -> convertible_to<bool>;
};

// equality_comparable implemented with SFINAE
template <class T, typename = void>
struct is_equality_comparable : std::false_type
{
};

template <class T>
struct is_equality_comparable<T, std::enable_if_t<
                                     is_convertible_to_v<decltype(std::declval<const std::remove_reference_t<T> &>() == std::declval<const std::remove_reference_t<T>>()), bool>>> : std::true_type
{
};

template <class T>
constexpr bool is_equality_comparable_v = is_equality_comparable<T>::value;

// Destructible concept
template <class T>
concept destructible = std::is_nothrow_destructible_v<T>;

// is_destructible implemented with SFINAE
template <class T>
constexpr bool is_destructible_v = std::is_nothrow_destructible<T>::value;

template <class T, class... Args>
concept constructible_from =
    destructible<T> && std::is_constructible_v<T, Args...>;

// is_constructible_from implemented with SFINAE
template <class T, class... Args>
constexpr bool is_constructible_from_v = is_destructible_v<T> && std::is_constructible_v<T, Args...>;

template <class T>
concept default_initializable =
    constructible_from<T> &&
    requires { T{}; } &&
    requires { ::new (static_cast<void *>(nullptr)) T; };

// is_default_initializable implemented with SFINAE
template <class T, typename = void>
struct is_default_initializable : std::false_type
{
};

template <class T>
struct is_default_initializable<T, std::void_t<std::enable_if_t<is_constructible_from_v<T>>, decltype(T{}), decltype(::new (static_cast<void *>(nullptr)) T)>> : std::true_type
{
};

template <class T>
constexpr bool is_default_initializable_v = is_default_initializable<T>::value;

// concepts
template <typename IterT, typename ContainerT>
concept container_iterator =
    std::same_as<IterT, typename ContainerT::iterator> ||
    std::same_as<IterT, typename ContainerT::const_iterator>;

// old-school
template <typename T, typename U, typename = void>
struct is_same_as : std::false_type
{
};

template <typename T, typename U>
struct is_same_as<T, U, std::enable_if_t<std::is_same_v<T, U> && std::is_same_v<U, T>>> : std::true_type
{
};

template <typename T, typename U>
constexpr bool is_same_as_v = is_same_as<T, U>::value;

template <typename IterT, typename ContainerT, typename = void>
struct is_container_iterator : std::false_type
{
};

template <typename IterT, typename ContainerT>
struct is_container_iterator<IterT, ContainerT, std::enable_if_t<is_same_as_v<IterT, typename ContainerT::iterator> || is_same_as_v<IterT, typename ContainerT::const_iterator>>> : std::true_type
{
};

template <typename IterT, typename ContainerT>
constexpr bool is_container_iterator_v = is_container_iterator<IterT, ContainerT>::value;

template <typename PtrT, typename ContainerT>
concept container_pointer =
    std::same_as<PtrT, typename ContainerT::pointer> ||
    std::same_as<PtrT, typename ContainerT::const_pointer>;

template <typename PtrT, typename ContainerT, typename = void>
struct is_container_pointer : std::false_type
{
};

template <typename PtrT, typename ContainerT>
struct is_container_pointer<PtrT, ContainerT, std::enable_if_t<is_same_as_v<PtrT, typename ContainerT::pointer> || is_same_as_v<PtrT, typename ContainerT::const_pointer>>> : std::true_type
{
};

template <typename PtrT, typename ContainerT>
constexpr bool is_container_pointer_v = is_container_pointer<PtrT, ContainerT>::value;

template <typename T>
class Dummy_container
{
public:
    using value_type = T;
    using iterator = T *;
    using const_iterator = const T *;
    using pointer = T *;
    using const_pointer = const T *;
    using size_type = size_t;

    iterator begin() { return &m_value; }
    iterator end() { return &m_value; }

    const_iterator cbegin() const { return &m_value; }
    const_iterator cend() const { return &m_value; }

    size_type size() const { return 1UL; }
    bool empty() const { return false; }

private:
    T m_value;
};

template <typename T>
class Continuous_container : public Dummy_container<T>
{
public:
    Continuous_container(Dummy_container<T>::size_type) {}

    Dummy_container<T>::const_pointer data() { return nullptr; }

    void resize(Dummy_container<T>::size_type) {}
};

template <typename T>
concept container = requires(T a) {
    {
        a.begin()
    } -> container_iterator<T>;
    {
        a.end()
    } -> container_iterator<T>;
    {
        a.cbegin()
    } -> container_iterator<T>;
    {
        a.cend()
    } -> container_iterator<T>;
    {
        a.size()
    } -> std::same_as<typename T::size_type>;
    typename T::value_type;
};

template <typename T, typename = void>
struct is_container : std::false_type
{
};

template <typename T>
struct is_container<T, std::enable_if_t<is_container_iterator_v<decltype(std::declval<T &>().begin()), T> &&
                                        is_container_iterator_v<decltype(std::declval<T &>().end()), T> &&
                                        is_container_iterator_v<decltype(std::declval<T &>().cbegin()), T> &&
                                        is_container_iterator_v<decltype(std::declval<T &>().cend()), T> &&
                                        is_same_as_v<decltype(std::declval<T &>().size()), typename T::size_type>>> : std::true_type
{
    using value_type = typename T::value_type;
};

// Helper for checking iterable type
template <typename T>
constexpr bool is_container_v = is_container<T>::value;

template <typename T>
concept contiguous_container =
    container<T> &&
    requires(T a) {
        {
            a.data()
        } -> container_pointer<T>;
    };

template <typename T, typename = void>
struct is_contiguous_container : std::false_type
{
};

template <typename T>
struct is_contiguous_container<T, std::enable_if_t<is_container_v<T> &&
                                                   is_container_pointer_v<decltype(std::declval<T &>().data()), T>>> : std::true_type
{
};

// Helper for checking iterable type
template <typename T>
constexpr bool is_contiguous_container_v = is_contiguous_container<T>::value;

template <typename T>
concept has_empty = requires(T a) {
    {
        a.empty()
    } -> std::same_as<bool>;
};

template <typename T, typename = void>
struct has_empty_method : std::false_type
{
};

template <typename T>
struct has_empty_method<T, std::enable_if_t<is_same_as_v<decltype(std::declval<T &>().empty()), bool>>> : std::true_type
{
};

// Helper for checking has_empty_method
template <typename T>
constexpr bool has_empty_method_v = has_empty_method<T>::value;

template <typename T>
concept resizable_container =
    container<T> &&
    requires(T &c, typename T::size_type s) {
        T(s);
        c.resize(s);
    };

template <typename T, typename = void>
struct is_resizable_container : std::false_type
{
};

template <typename T>
struct is_resizable_container<T, std::void_t<std::enable_if_t<is_container_v<T>>, decltype(T(typename T::size_type{})), decltype(std::declval<T &>().resize(typename T::size_type{}))>> : std::true_type
{
};

// Helper for checking resizable_container type
template <typename T>
constexpr bool is_resizable_container_v = is_resizable_container<T>::value;

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
    int carr[] = {8, 9};
    std::span<int> sp(std::begin(carr), std::end(carr));
    // print_iterable(1);
    print_iterable(arr);
    print_iterable(vec);
    // print_iterable(carr);
    print_iterable(sp);

    // Only_iterable_a a1(1);
    Only_iterable_a a2(arr);
    Only_iterable_a a3(vec);
    // Only_iterable_a a4(carr);
    Only_iterable_a a5(sp);

    // Only_iterable_b b1(1);
    Only_iterable_b b2(arr);
    Only_iterable_b b3(vec);
    // Only_iterable_b b4(carr);
    Only_iterable_b b5(sp);

    // With concepts
    std::cout << "convertible_to with concepts\n";
    std::cout << convertible_to<int, int>;
    std::cout << convertible_to<int, double>;
    std::cout << convertible_to<std::string, std::string_view>;
    std::cout << convertible_to<int, std::string_view>;
    std::cout << convertible_to<std::string, bool>;
    std::cout << convertible_to<decltype(small == small), bool>;
    std::cout << convertible_to<decltype(big == big), bool> << "\n";

    // Old-school
    std::cout << "is_convertible_to_v old-school\n";
    std::cout << is_convertible_to_v<int, int>;
    std::cout << is_convertible_to_v<int, double>;
    std::cout << is_convertible_to_v<std::string, std::string_view>;
    std::cout << is_convertible_to_v<int, std::string_view>;
    std::cout << is_convertible_to_v<std::string, bool>;
    std::cout << is_convertible_to_v<decltype(small == small), bool>;
    std::cout << is_convertible_to_v<decltype(big == big), bool> << "\n";

    // Test
    static_assert(convertible_to<int, int> == is_convertible_to_v<int, int>, "NOK");
    static_assert(convertible_to<int, double> == is_convertible_to_v<int, double>, "NOK");
    static_assert(convertible_to<std::string, std::string_view> == is_convertible_to_v<std::string, std::string_view>, "NOK");
    static_assert(convertible_to<int, std::string_view> == is_convertible_to_v<int, std::string_view>, "NOK");
    static_assert(convertible_to<std::string, bool> == is_convertible_to_v<std::string, bool>, "NOK");
    static_assert(convertible_to<decltype(small == small), bool> == is_convertible_to_v<decltype(small == small), bool>, "NOK");
    static_assert(convertible_to<decltype(big == big), bool> == is_convertible_to_v<decltype(big == big), bool>, "NOK");

    // With concepts
    std::cout << "equality_comparable with concepts\n";
    std::cout << equality_comparable<int>;
    std::cout << equality_comparable<float>;
    std::cout << equality_comparable<std::string>;
    std::cout << equality_comparable<Small>;
    std::cout << equality_comparable<Big> << "\n";

    // Old-school
    std::cout << "is_equality_comparable_v old-school\n";
    std::cout << is_equality_comparable_v<int>;
    std::cout << is_equality_comparable_v<float>;
    std::cout << is_equality_comparable_v<std::string>;
    std::cout << is_equality_comparable_v<Small>;
    std::cout << is_equality_comparable_v<Big> << "\n";

    // Test
    static_assert(equality_comparable<int> == is_equality_comparable_v<int>, "NOK");
    static_assert(equality_comparable<float> == is_equality_comparable_v<double>, "NOK");
    static_assert(equality_comparable<std::string> == is_equality_comparable_v<std::string>, "NOK");
    static_assert(equality_comparable<Small> == is_equality_comparable_v<Small>, "NOK");
    static_assert(equality_comparable<Big> == is_equality_comparable_v<Big>, "NOK");

    // With concepts
    std::cout << "destructible with concepts\n";
    std::cout << destructible<int>;
    std::cout << destructible<float>;
    std::cout << destructible<std::string>;
    std::cout << destructible<Small>;
    std::cout << destructible<Big> << "\n";

    // Old-school
    std::cout << "is_nothrow_destructible_v old-school\n";
    std::cout << is_destructible_v<int>;
    std::cout << is_destructible_v<float>;
    std::cout << is_destructible_v<std::string>;
    std::cout << is_destructible_v<Small>;
    std::cout << is_destructible_v<Big> << "\n";

    // Test
    static_assert(destructible<int> == is_destructible_v<int>, "NOK");
    static_assert(destructible<float> == is_destructible_v<double>, "NOK");
    static_assert(destructible<std::string> == is_destructible_v<std::string>, "NOK");
    static_assert(destructible<Small> == is_destructible_v<Small>, "NOK");
    static_assert(destructible<Big> == is_destructible_v<Big>, "NOK");

    // With concepts
    std::cout << "constructible_from with concepts\n";
    std::cout << constructible_from<int, int>;
    std::cout << constructible_from<float, int>;
    std::cout << constructible_from<std::string, std::string_view>;
    std::cout << constructible_from<Small, Small>;
    std::cout << constructible_from<Big, Small> << "\n";

    // Old-school
    std::cout << "is_constructible_from_v old-school\n";
    std::cout << is_constructible_from_v<int, int>;
    std::cout << is_constructible_from_v<float, int>;
    std::cout << is_constructible_from_v<std::string, std::string_view>;
    std::cout << is_constructible_from_v<Small, Small>;
    std::cout << is_constructible_from_v<Big, Small> << "\n";

    // Test
    static_assert(constructible_from<int, int> == is_constructible_from_v<int, int>, "NOK");
    static_assert(constructible_from<float, int> == is_constructible_from_v<float, int>, "NOK");
    static_assert(constructible_from<std::string, std::string_view> == is_constructible_from_v<std::string, std::string_view>, "NOK");
    static_assert(constructible_from<Small, Small> == is_constructible_from_v<Small, Small>, "NOK");
    static_assert(constructible_from<Big, Small> == is_constructible_from_v<Big, Small>, "NOK");

    // With concepts
    std::cout << "default_initializable with concepts\n";
    std::cout << default_initializable<int>;
    std::cout << default_initializable<float>;
    std::cout << default_initializable<std::string>;
    std::cout << default_initializable<Small>;
    std::cout << default_initializable<Big>;
    std::cout << default_initializable<No_default_ctor> << "\n";

    // Old-school
    std::cout << "is_default_initializable_v old-school\n";
    std::cout << is_default_initializable_v<int>;
    std::cout << is_default_initializable_v<float>;
    std::cout << is_default_initializable_v<std::string>;
    std::cout << is_default_initializable_v<Small>;
    std::cout << is_default_initializable_v<Big>;
    std::cout << is_default_initializable_v<No_default_ctor> << "\n";

    // Test
    static_assert(default_initializable<int> == is_default_initializable_v<int>, "NOK");
    static_assert(default_initializable<float> == is_default_initializable_v<float>, "NOK");
    static_assert(default_initializable<std::string> == is_default_initializable_v<std::string>, "NOK");
    static_assert(default_initializable<Small> == is_default_initializable_v<Small>, "NOK");
    static_assert(default_initializable<Big> == is_default_initializable_v<Big>, "NOK");
    static_assert(default_initializable<No_default_ctor> == is_default_initializable_v<No_default_ctor>, "NOK");

    // Old-school
    std::cout << "is_same_as_v check\n";
    std::cout << is_same_as_v<int, int>;
    std::cout << is_same_as_v<float, int>;
    std::cout << is_same_as_v<long, int>;
    std::cout << is_same_as_v<Small, Big>;
    std::cout << is_same_as_v<Big, Big>;
    std::cout << is_same_as_v<std::vector<int>::iterator, std::vector<int>::iterator> << "\n";

    // Test
    static_assert(std::same_as<int, int> == is_same_as_v<int, int>, "NOK");
    static_assert(std::same_as<float, int> == is_same_as_v<float, int>, "NOK");
    static_assert(std::same_as<long, int> == is_same_as_v<long, int>, "NOK");
    static_assert(std::same_as<Small, Big> == is_same_as_v<Small, Big>, "NOK");
    static_assert(std::same_as<Big, Big> == is_same_as_v<Big, Big>, "NOK");
    static_assert(std::same_as<std::span<float>::iterator, std::span<float>::iterator> == is_same_as_v<std::span<float>::iterator, std::span<float>::iterator>, "NOK");

    // With concepts
    std::cout << "container_iterator with concepts\n";
    std::cout << container_iterator<int, std::vector<int>>;
    std::cout << container_iterator<std::vector<int>::iterator, std::vector<int>>;
    std::cout << container_iterator<std::vector<int>::const_iterator, std::array<int, 3>>;
    std::cout << container_iterator<std::vector<int>::const_iterator, std::vector<int>>;
    std::cout << container_iterator<std::map<int, std::string>::iterator, std::vector<int>>;
    std::cout << container_iterator<std::map<int, std::string>::iterator, std::map<int, std::string>>;
    std::cout << container_iterator<std::span<int>::iterator, std::span<float>>;
    std::cout << container_iterator<std::span<float>::iterator, std::span<float>> << "\n";

    // Old-school
    std::cout << "is_container_iterator_v old-school\n";
    std::cout << is_container_iterator_v<int, std::vector<int>>;
    std::cout << is_container_iterator_v<std::vector<int>::iterator, std::vector<int>>;
    std::cout << is_container_iterator_v<std::vector<int>::const_iterator, std::array<int, 3>>;
    std::cout << is_container_iterator_v<std::vector<int>::const_iterator, std::vector<int>>;
    std::cout << is_container_iterator_v<std::map<int, std::string>::iterator, std::vector<int>>;
    std::cout << is_container_iterator_v<std::map<int, std::string>::iterator, std::map<int, std::string>>;
    std::cout << is_container_iterator_v<std::span<int>::iterator, std::span<float>>;
    std::cout << is_container_iterator_v<std::span<float>::iterator, std::span<float>> << "\n"; // TODO: why false?

    // Test
    static_assert(container_iterator<int, std::vector<int>> == is_container_iterator_v<int, std::vector<int>>, "NOK");
    static_assert(container_iterator<std::vector<int>::iterator, std::vector<int>> == is_container_iterator_v<std::vector<int>::iterator, std::vector<int>>, "NOK");
    static_assert(container_iterator<std::vector<int>::const_iterator, std::array<int, 3>> == is_container_iterator_v<std::vector<int>::const_iterator, std::array<int, 3>>, "NOK");
    static_assert(container_iterator<std::vector<int>::const_iterator, std::vector<int>> == is_container_iterator_v<std::vector<int>::const_iterator, std::vector<int>>, "NOK");
    static_assert(container_iterator<std::map<int, std::string>::iterator, std::vector<int>> == is_container_iterator_v<std::map<int, std::string>::iterator, std::vector<int>>, "NOK");
    static_assert(container_iterator<std::map<int, std::string>::iterator, std::map<int, std::string>> == is_container_iterator_v<std::map<int, std::string>::iterator, std::map<int, std::string>>, "NOK");
    static_assert(container_iterator<std::span<int>::iterator, std::span<float>> == is_container_iterator_v<std::span<int>::iterator, std::span<float>>, "NOK");
    // static_assert(container_iterator<std::span<float>::iterator, std::span<float>> == is_container_iterator_v<std::span<float>::iterator, std::span<float>>, "NOK");

    // With concepts
    std::cout << "container_pointer with concepts\n";
    std::cout << container_pointer<int *, float>;
    std::cout << container_pointer<int *, std::vector<int>>;
    std::cout << container_pointer<const int *, std::vector<int>>;
    std::cout << container_pointer<const std::pair<int, std::string> *, std::map<int, std::string>>;
    std::cout << container_pointer<const std::pair<const int, std::string> *, std::map<int, std::string>> << "\n";

    // Old-school
    std::cout << "is_container_pointer_v old-school\n";
    std::cout << is_container_pointer_v<int *, float>;
    std::cout << is_container_pointer_v<int *, std::vector<int>>;
    std::cout << is_container_pointer_v<const int *, std::vector<int>>;
    std::cout << is_container_pointer_v<const std::pair<int, std::string> *, std::map<int, std::string>>;
    std::cout << is_container_pointer_v<const std::pair<const int, std::string> *, std::map<int, std::string>> << "\n";

    // Test
    static_assert(container_pointer<int *, float> == is_container_pointer_v<int *, float>, "NOK");
    static_assert(container_pointer<int *, std::vector<int>> == is_container_pointer_v<int *, std::vector<int>>, "NOK");
    static_assert(container_pointer<const int *, std::vector<int>> == is_container_pointer_v<const int *, std::vector<int>>, "NOK");
    static_assert(container_pointer<const std::pair<int, std::string> *, std::map<int, std::string>> == is_container_pointer_v<const std::pair<int, std::string> *, std::map<int, std::string>>, "NOK");
    static_assert(container_pointer<const std::pair<const int, std::string> *, std::map<int, std::string>> == is_container_pointer_v<const std::pair<const int, std::string> *, std::map<int, std::string>>, "NOK");

    // With concepts
    std::cout << "container with concepts\n";
    std::cout << container<int>;
    std::cout << container<int[4]>;
    std::cout << container<std::string>;
    std::cout << container<std::array<int, 3>>;
    std::cout << container<std::vector<float>>;
    std::cout << container<std::map<int, std::string>>;
    std::cout << container<Dummy_container<int>> << "\n";

    // With old-school
    std::cout << "is_container_v old-school\n";
    std::cout << is_container_v<int>;
    std::cout << is_container_v<int[4]>;
    std::cout << is_container_v<std::string>;
    std::cout << is_container_v<std::array<int, 3>>;
    std::cout << is_container_v<std::vector<float>>;
    std::cout << is_container_v<std::map<int, std::string>>;
    std::cout << is_container_v<Dummy_container<int>> << "\n";

    // Test
    static_assert(container<int> == is_container_v<int>, "NOK");
    static_assert(container<int[4]> == is_container_v<int[4]>, "NOK");
    static_assert(container<std::string> == is_container_v<std::string>, "NOK");
    static_assert(container<std::array<int, 3>> == is_container_v<std::array<int, 3>>, "NOK");
    static_assert(container<std::vector<float>> == is_container_v<std::vector<float>>, "NOK");
    static_assert(container<std::map<int, std::string>> == is_container_v<std::map<int, std::string>>, "NOK");
    static_assert(container<Dummy_container<int>> == is_container_v<Dummy_container<int>>, "NOK");

    // With concepts
    std::cout << "contiguous_container with concepts\n";
    std::cout << contiguous_container<int>;
    std::cout << contiguous_container<int[4]>;
    std::cout << contiguous_container<std::string>;
    std::cout << contiguous_container<std::array<int, 3>>;
    std::cout << contiguous_container<std::vector<float>>;
    std::cout << contiguous_container<std::map<int, std::string>>;
    std::cout << contiguous_container<Dummy_container<int>>;
    std::cout << contiguous_container<Continuous_container<float>> << "\n";

    // Old-school
    std::cout << "is_contiguous_container_v old-school\n";
    std::cout << is_contiguous_container_v<int>;
    std::cout << is_contiguous_container_v<int[4]>;
    std::cout << is_contiguous_container_v<std::string>;
    std::cout << is_contiguous_container_v<std::array<int, 3>>;
    std::cout << is_contiguous_container_v<std::vector<float>>;
    std::cout << is_contiguous_container_v<std::map<int, std::string>>;
    std::cout << is_contiguous_container_v<Dummy_container<int>>;
    std::cout << is_contiguous_container_v<Continuous_container<float>> << "\n";

    // Test
    static_assert(contiguous_container<int> == is_contiguous_container_v<int>, "NOK");
    static_assert(contiguous_container<int[4]> == is_contiguous_container_v<int[4]>, "NOK");
    static_assert(contiguous_container<std::string> == is_contiguous_container_v<std::string>, "NOK");
    static_assert(contiguous_container<std::array<int, 3>> == is_contiguous_container_v<std::array<int, 3>>, "NOK");
    static_assert(contiguous_container<std::vector<float>> == is_contiguous_container_v<std::vector<float>>, "NOK");
    static_assert(contiguous_container<std::map<int, std::string>> == is_contiguous_container_v<std::map<int, std::string>>, "NOK");
    static_assert(contiguous_container<Dummy_container<int>> == is_contiguous_container_v<Dummy_container<int>>, "NOK");
    static_assert(contiguous_container<Continuous_container<float>> == is_contiguous_container_v<Continuous_container<float>>, "NOK");

    // With concepts
    std::cout << "has_empty with concepts\n";
    std::cout << has_empty<int>;
    std::cout << has_empty<std::string>;
    std::cout << has_empty<std::vector<int>>;
    std::cout << has_empty<Big>;
    std::cout << has_empty<Dummy_container<double>> << "\n";

    // Old-school
    std::cout << "has_empty old-school\n";
    std::cout << has_empty_method_v<int>;
    std::cout << has_empty_method_v<std::string>;
    std::cout << has_empty_method_v<std::vector<int>>;
    std::cout << has_empty_method_v<Big>;
    std::cout << has_empty_method_v<Dummy_container<double>> << "\n";

    // Test
    static_assert(has_empty<int> == has_empty_method_v<int>, "NOK");
    static_assert(has_empty<std::string> == has_empty_method_v<std::string>, "NOK");
    static_assert(has_empty<std::vector<int>> == has_empty_method_v<std::vector<int>>, "NOK");
    static_assert(has_empty<Big> == has_empty_method_v<Big>, "NOK");
    static_assert(has_empty<Dummy_container<double>> == has_empty_method_v<Dummy_container<double>>, "NOK");

    // With concepts
    std::cout << "resizable_container with concepts\n";
    std::cout << resizable_container<int>;
    std::cout << resizable_container<std::string>;
    std::cout << resizable_container<std::vector<int>>;
    std::cout << resizable_container<Big>;
    std::cout << resizable_container<Continuous_container<int>> << "\n";

    // Old-school
    std::cout << "is_resizable_container_v old-school\n";
    std::cout << is_resizable_container_v<int>;
    std::cout << is_resizable_container_v<std::string>;
    std::cout << is_resizable_container_v<std::vector<int>>;
    std::cout << is_resizable_container_v<Big>;
    std::cout << is_resizable_container_v<Continuous_container<int>> << "\n";

    // Test
    static_assert(resizable_container<int> == is_resizable_container_v<int>, "NOK");
    static_assert(resizable_container<std::string> == is_resizable_container_v<std::string>, "NOK");
    static_assert(resizable_container<std::vector<int>> == is_resizable_container_v<std::vector<int>>, "NOK");
    static_assert(resizable_container<Big> == is_resizable_container_v<Big>, "NOK");
    static_assert(resizable_container<Continuous_container<int>> == is_resizable_container_v<Continuous_container<int>>, "NOK");

    return 0;
}
