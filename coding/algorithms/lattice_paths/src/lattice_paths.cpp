// Copyright (c) 2023, Piotr Staniszewski

#include <benchmark/benchmark.h>

#include <iostream>
#include <array>

constexpr unsigned long GRID_WIDTH = 1U;
constexpr unsigned long GRID_HEIGHT = 1U;
constexpr unsigned long NODE_COUNT = 4U * GRID_WIDTH * GRID_HEIGHT;

template <unsigned long WIDTH, unsigned long HEIGHT>
struct Node
{
    Node() = default;

    Node(unsigned long x, unsigned long y) : m_x{x}, m_y{y}
    {
        std::cout << "at(" << get_idx() << ") = Node(" << x << "," << y << ")\n";
    }

    unsigned long get_idx() const noexcept { return m_y + m_x * WIDTH; }

    bool is_root() const noexcept { return m_x == m_y == 0; }

    unsigned long m_x;
    unsigned long m_y;
};

template <unsigned long WIDTH, unsigned long HEIGHT>
auto build_nodes() -> std::array<Node<WIDTH, HEIGHT>, WIDTH * HEIGHT>
{
    std::array<Node<WIDTH, HEIGHT>, WIDTH * HEIGHT> grid{};
    for (unsigned long i = 0U; i < WIDTH; ++i)
    {
        for (unsigned long j = 0U; j < HEIGHT; ++j)
        {
            grid[j + i * (WIDTH)] = Node<WIDTH, HEIGHT>(i, j);
        }
    }

    return grid;
}

template <unsigned long WIDTH, unsigned long HEIGHT>
void calculate_paths(const Node<WIDTH, HEIGHT> &node, unsigned long &counter)
{
}

static void benchmark_calculate_paths(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto grid = build_nodes<2U, 2U>();
    }
}
BENCHMARK(benchmark_calculate_paths);

BENCHMARK_MAIN();
