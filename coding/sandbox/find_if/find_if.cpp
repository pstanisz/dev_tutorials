// Copyright (c) 2022, Piotr Staniszewski

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <limits>

template <typename Container, typename F>
void test_find_if(const std::string &desc, const Container &items, F &&lambda)
{
    std::cout << desc;
    if (std::find_if(std::begin(items), std::end(items), lambda) != std::end(items))
    {
        std::cout << "Found\n";
    }
    else
    {
        std::cout << "Not found\n";
    }
}

int main()
{
    std::cout << "find_if with containers\n";

    std::vector<int> ints{1, 6, 2, 99, 76, 5, 32, 89};
    test_find_if("Find value > 50: ", ints, [](const auto item)
                 { return item > 50; });
    test_find_if("Find value > 100: ", ints, [](const auto item)
                 { return item > 100; });
    test_find_if("Find value %3 = 0: ", ints, [](const auto item)
                 { return ((item % 3) == 0); });

    std::map<std::string, size_t> sizes{
        {"uint8_t", std::numeric_limits<uint8_t>::max()},
        {"uint16_t", std::numeric_limits<uint16_t>::max()},
        {"uint32_t", std::numeric_limits<uint32_t>::max()}};

    test_find_if("Find type that's max is less that 1024: ", sizes, [](const auto &item)
                 { return item.second < 1024; });
    test_find_if("Find type that's max is above std::numeric_limits<uint64_t>::max(): ", sizes, [](const auto &item)
                 { return item.second > std::numeric_limits<uint64_t>::max(); });
    test_find_if("Find key that contains '8': ", sizes, [](const auto &item)
                 { return (item.first.find('8') != std::string::npos); });
    test_find_if("Find key that contains 'x': ", sizes, [](const auto &item)
                 { return (item.first.find('x') != std::string::npos); });

    return EXIT_SUCCESS;
}
