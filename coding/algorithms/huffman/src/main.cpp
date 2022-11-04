// Copyright (c) 2022, Piotr Staniszewski

#include <iostream>
#include <cstdlib>
#include <map>
#include <queue>
#include <memory>

#include <version.h>

const std::string INPUT_STRING = R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod
    tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,
    quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.
    Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore
    eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident,
    sunt in culpa qui officia deserunt mollit anim id est laborum.)";

using Char_freq = std::pair<char, size_t>;

struct Node
{
    char m_data;
    size_t m_freq;

    Node *m_left;
    Node *m_right;

    Node(char data, size_t freq) : m_data{data},
                                   m_freq{freq},
                                   m_left{nullptr},
                                   m_right{nullptr} {}
};

template <typename T>
struct Greater
{
    bool operator()(const T lhs, const T rhs) { return lhs > rhs; }
};

template <>
struct Greater<Node *>
{
    bool operator()(const Node *lhs, const Node *rhs)
    {
        return lhs->m_freq > rhs->m_freq;
    }
};

auto build_huffman_tree(const std::string &input_text)
{
    std::map<char, size_t> frequency;
    for (auto character : input_text)
    {
        ++frequency[character];
    }

    // Create a leaf node for every character in the input, minimum heap of all leaf nodes
    std::priority_queue<Node *, std::vector<Node *>, Greater<Node *>> min_heap;
    for (const auto &item : frequency)
    {
        min_heap.push(new Node(item.first, item.second));
    }

    while (min_heap.size() > 1)
    {
        // Get the top two nodes with minimum frequency
        auto left = min_heap.top();
        min_heap.pop();

        auto right = min_heap.top();
        min_heap.pop();

        // Create internal node with sum of above two
        auto *internal = new Node('*', left->m_freq + right->m_freq);
        internal->m_left = left;
        internal->m_right = right;

        // Add internal node to the minumum heap
        min_heap.push(internal);
    }

    return min_heap;
}

void print_huffman_tree(const Node *root, std::string code)
{
    if (!root)
    {
        return;
    }

    if (root->m_data != '*')
    {
        std::cout << root->m_data << ": " << code << "\n";
    }
    print_huffman_tree(root->m_left, code + "0");
    print_huffman_tree(root->m_right, code + "1");
};

void release_huffman_tree(const Node *root)
{
    if (!root)
    {
        return;
    }

    release_huffman_tree(root->m_left);
    release_huffman_tree(root->m_right);

    delete root;
    root = nullptr;
}

int main()
{

    std::cout << "Huffman coding v" << Version_major << "." << Version_minor << "\n";

    const std::string &input_text = INPUT_STRING;

    std::cout << "Input:\n\""
              << input_text << "\"\n";
    std::cout << "Size: " << input_text.size() << "\n";

    auto min_heap = build_huffman_tree(input_text);

    if (!min_heap.empty())
    {
        auto root = min_heap.top();
        print_huffman_tree(root, "");

        min_heap.pop();
        release_huffman_tree(root);
    }

    return EXIT_SUCCESS;
}
