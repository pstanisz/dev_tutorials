// Copyright (c) 2023, Piotr Staniszewski

#include <benchmark/benchmark.h>

#include <iostream>
#include <array>
#include <optional>

// constexpr unsigned long GRID_WIDTH = 1U;
// constexpr unsigned long GRID_HEIGHT = 1U;

template <unsigned long WIDTH, unsigned long HEIGHT>
struct Node
{
    Node() = default;

    Node(unsigned long x, unsigned long y) : m_x{x}, m_y{y}
    {
        //auto idx = calculate_idx(m_x, m_y);
        //std::cout << "at(" << idx << ") = Node(" << x << "," << y << ")\n";
        if (x + 1U < WIDTH)
        {
            m_left_neighbour_idx = calculate_idx(x + 1U, y);
            //std::cout << "has neighbout left with idx = " << m_left_neighbour_idx.value() << "\n";
        }
        if (y + 1U < HEIGHT)
        {
            m_down_neighbour_idx = calculate_idx(x, y + 1U);
            //std::cout << "has neighbout down with idx = " << m_down_neighbour_idx.value() << "\n";
        }
    }

    static unsigned long calculate_idx(unsigned long x, unsigned long y) { return x + y * WIDTH; }

    bool is_root() const noexcept { return m_x == m_y == 0; }

    unsigned long m_x;
    unsigned long m_y;

    std::optional<unsigned long> m_left_neighbour_idx;
    std::optional<unsigned long> m_down_neighbour_idx;
};

template <unsigned long WIDTH, unsigned long HEIGHT>
auto build_nodes() -> std::array<Node<WIDTH, HEIGHT>, WIDTH * HEIGHT>
{
    std::array<Node<WIDTH, HEIGHT>, WIDTH * HEIGHT> grid{};
    for (unsigned long i = 0U; i < WIDTH; ++i)
    {
        for (unsigned long j = 0U; j < HEIGHT; ++j)
        {
            grid[Node<WIDTH, HEIGHT>::calculate_idx(i, j)] = Node<WIDTH, HEIGHT>(i, j);
        }
    }

    return grid;
}

template <unsigned long WIDTH, unsigned long HEIGHT>
void calculate_paths_brute(const Node<WIDTH, HEIGHT> &node, const std::array<Node<WIDTH, HEIGHT>, WIDTH * HEIGHT>& nodes, unsigned long &paths)
{
    if (node.m_left_neighbour_idx.has_value()) {
        calculate_paths_brute(nodes[node.m_left_neighbour_idx.value()], nodes, paths);
    }

    if (node.m_down_neighbour_idx.has_value()) {
        calculate_paths_brute(nodes[node.m_down_neighbour_idx.value()], nodes, paths);
    }

    if (!node.m_down_neighbour_idx.has_value() && !node.m_left_neighbour_idx.has_value()) {
        ++paths;
    }
}

static void benchmark_calculate_paths_brute(benchmark::State &state)
{
    for (auto _ : state)
    {
        unsigned long paths{0UL};

        auto nodes = build_nodes<2U, 2U>();
        auto root = nodes[0];
        calculate_paths_brute(root, nodes, paths);

        //std::cout << "paths: " << paths << "\n";
    }
}
BENCHMARK(benchmark_calculate_paths_brute);

BENCHMARK_MAIN();

// int main() {
//     unsigned long paths{0UL};

//     auto nodes = build_nodes<4U, 4U>();
//     auto root = nodes[0];
//     calculate_paths_brute(root, nodes, paths);

//     std::cout << "paths: " << paths << "\n";

//     return 0;
// }
