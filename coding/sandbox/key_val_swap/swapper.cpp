// Copyright (c) 2022, Piotr Staniszewski

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cassert>
#include <typeinfo>

template <typename C, typename T = typename C::value_type>
void print(const C &input)
{
    std::cout << "{\n";
    for (const auto &item : input)
    {
        std::cout << "  {" << item.first << " : " << item.second << "},\n";
    }
    std::cout << "}\n";
}

template <typename K, typename V>
bool is_swappable_to_map(const std::map<K, V> &input)
{
    std::map<V, unsigned int> values;
    for (const auto &item : input)
    {
        // No need to iterate further if duplicate found
        if (++values[item.second] > 1U)
        {
            return false;
        }
    }

    return true;
}

template <typename K, typename V>
std::multimap<V, K> swap_keys_with_multivalues(const std::map<K, V> &input)
{
    std::multimap<V, K> output;
    std::for_each(std::begin(input), std::end(input), [&output](const auto &item)
                  { output.insert(std::make_pair(item.second, item.first)); });

    return output;
}

template <typename K, typename V>
std::map<V, K> swap_keys_with_values(const std::map<K, V> &input)
{
    std::map<V, K> output;
    std::for_each(std::begin(input), std::end(input), [&output](const auto &item)
                  { output[item.second] = item.first; });

    return output;
}

template <typename K, typename V>
void test_swap(const std::map<K, V> &input)
try
{
    std::cout << "Input:\n";
    print(input);

    std::cout << "Output:\n";
    if (is_swappable_to_map(input))
    {
        auto output = swap_keys_with_values(input);

        // verify
        print(output);
        assert(input.size() == output.size());
        assert(typeid(output) == typeid(std::map<V, K>));
    }
    else
    {
        auto output = swap_keys_with_multivalues(input);

        // verify
        print(output);
        assert(input.size() == output.size());
        assert(typeid(output) == typeid(std::multimap<V, K>));
    }
}
catch (const std::exception &ex)
{
    std::cerr << "Test failed: " << ex.what() << std::endl;
}

int main()
{
    std::cout << "Swapping keys and values within the map\n";

    std::map<int, int> map_one = {
        {1, 10},
        {2, 20},
        {3, 30}};

    std::map<char, int> map_two = {
        {'a', 40},
        {'b', 30},
        {'c', 20},
        {'d', 10}};

    std::map<int, char> map_three = {
        {1, 'q'},
        {2, 'w'},
        {3, 'e'},
        {4, 'r'},
        {5, 't'},
        {6, 'y'}};

    std::map<char, int> map_four = {
        {'a', 1},
        {'b', 2},
        {'c', 1},
        {'d', 2}};

    std::map<std::string, int> map_five = {
        {"three", 3},
        {"one", 1},
        {"nine", 9},
        {"zero", 0},
        {"six", 6},
        {"one_half", 1}};

    test_swap(map_one);
    test_swap(map_two);
    test_swap(map_three);
    test_swap(map_four);
    test_swap(map_five);

    return EXIT_SUCCESS;
}
