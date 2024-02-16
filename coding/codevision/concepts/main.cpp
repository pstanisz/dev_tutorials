// Copyright (c) 2024, Piotr Staniszewski

#include <iostream>
#include <vector>

namespace sfinae
{

    // Integral type processed
    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    void process(T)
    {
        std::cout << "integral processed\n";
    }

    // Won't work with sfinae with same function
    // // Floating type processed
    // template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>, void>>
    // void processs(T &)
    // {
    //     std::cout << "floating processed\n";
    // }

}

namespace concepts
{

    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <typename T>
    concept Floating_point = std::is_floating_point_v<T>;

    template <typename T>
    void process(T)
    {
        std::cout << "any processed\n";
    }

    template <Integral T>
    void process(T)
    {
        std::cout << "integral processed\n";
    }

    template <Floating_point T>
    void process(T)
    {
        std::cout << "floating processed\n";
    }

}

namespace sfinae2
{

    // Old-school
    template <typename IterT, typename ContainerT>
    using Container_iterator = typename std::enable_if_t<std::same_as<IterT, typename ContainerT::iterator> || std::same_as<IterT, typename ContainerT::const_iterator>>;

    template <typename IterT, typename ContainerT, typename = void>
    struct is_container_iterator : std::false_type
    {
    };

    template <typename IterT, typename ContainerT>
    struct is_container_iterator<IterT, ContainerT, Container_iterator<IterT, ContainerT>> : std::true_type
    {
    };

    template <typename IterT, typename ContainerT>
    constexpr bool is_container_iterator_v = is_container_iterator<IterT, ContainerT>::value;

    template <typename T>
    using Container = typename std::enable_if_t<is_container_iterator_v<decltype(std::declval<T &>().begin()), T> &&
                                                is_container_iterator_v<decltype(std::declval<T &>().end()), T> &&
                                                is_container_iterator_v<decltype(std::declval<T &>().cbegin()), T> &&
                                                is_container_iterator_v<decltype(std::declval<T &>().cend()), T> &&
                                                std::same_as<decltype(std::declval<T &>().size()), typename T::size_type>>;

    template <typename T, typename = void>
    struct is_container : std::false_type
    {
    };

    template <typename T>
    struct is_container<T, Container<T>> : std::true_type
    {
        using value_type = typename T::value_type;
    };

    // Helper for checking iterable type
    template <typename T>
    constexpr bool is_container_v = is_container<T>::value;

    template <typename T, typename = std::enable_if_t<is_container_v<T>>>
    void process(T)
    {
        std::cout << "container processed\n";
    }

}

namespace concepts
{

    template <typename IterT, typename ContainerT>
    concept Container_iterator =
        std::same_as<IterT, typename ContainerT::iterator> ||
        std::same_as<IterT, typename ContainerT::const_iterator>;

    template <typename T>
    concept Container = requires(T cont) {
        {
            cont.begin()
        } -> Container_iterator<T>;
        {
            cont.end()
        } -> Container_iterator<T>;
        {
            cont.cbegin()
        } -> Container_iterator<T>;
        {
            cont.cend()
        } -> Container_iterator<T>;
        {
            cont.size()
        } -> std::same_as<typename T::size_type>;
    };

    template <Container C>
    void process(C)
    {
        std::cout << "container processed\n";
    }

}

int main(int /*argc*/, char * /*argv*/[])
{
    std::cout << "C++20 concepts\n\n";

    const int i = 10;
    long j = 5;
    float k = 2.0;
    std::string s{"hello"};
    std::vector<int> v{1, 2, 3};

    std::cout << "sfinae\n";

    sfinae::process(i);
    sfinae::process(j);
    // sfinae::process(k);
    // sfinae::process(s);
    // sfinae::process(v);
    sfinae2::process(v);

    std::cout << "conceprs\n";

    concepts::process(i);
    concepts::process(j);
    concepts::process(k);
    concepts::process(s);
    concepts::process(v);

    return 0;
}
