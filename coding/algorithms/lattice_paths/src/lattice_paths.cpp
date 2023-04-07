// Copyright (c) 2023, Piotr Staniszewski

#include <benchmark/benchmark.h>

#include <iostream>
#include <array>
#include <optional>
#include <cstdint>

namespace Common
{

    template <unsigned WIDTH, unsigned HEIGHT>
    struct Node
    {
        Node() = default;

        Node(unsigned x, unsigned y) : m_x{x}, m_y{y}, m_paths(0U)
        {
            // auto idx = calculate_idx(m_x, m_y);
            // std::cout << "at(" << idx << ") = Node(" << x << "," << y << ")\n";
            if (x + 1U < WIDTH)
            {
                m_left_neighbour_idx = calculate_idx(x + 1U, y);
                // std::cout << "has neighbout left with idx = " << m_left_neighbour_idx.value() << "\n";
            }
            if (y + 1U < HEIGHT)
            {
                m_down_neighbour_idx = calculate_idx(x, y + 1U);
                // std::cout << "has neighbout down with idx = " << m_down_neighbour_idx.value() << "\n";
            }
        }

        static unsigned long calculate_idx(unsigned x, unsigned y) noexcept { return x + y * WIDTH; }

        unsigned m_x{0U};
        unsigned m_y{0U};
        unsigned long m_paths{0U};
        std::optional<unsigned> m_left_neighbour_idx;
        std::optional<unsigned> m_down_neighbour_idx;
    };

    template <unsigned WIDTH, unsigned HEIGHT>
    auto build_nodes() -> std::array<Node<WIDTH, HEIGHT>, WIDTH * HEIGHT>
    {
        std::array<Node<WIDTH, HEIGHT>, WIDTH * HEIGHT> grid{};
        for (unsigned i = 0U; i < WIDTH; ++i)
        {
            for (unsigned j = 0U; j < HEIGHT; ++j)
            {
                grid[Node<WIDTH, HEIGHT>::calculate_idx(i, j)] = Node<WIDTH, HEIGHT>(i, j);
            }
        }

        return grid;
    }
}

namespace Brute
{
    using Common::Node;

    // Recursive, all possibilities
    template <unsigned WIDTH, unsigned HEIGHT>
    void calculate_paths(const Node<WIDTH, HEIGHT> &node, const std::array<Node<WIDTH, HEIGHT>, WIDTH * HEIGHT> &nodes, unsigned long &paths)
    {
        if (node.m_left_neighbour_idx.has_value())
        {
            calculate_paths(nodes[node.m_left_neighbour_idx.value()], nodes, paths);
        }

        if (node.m_down_neighbour_idx.has_value())
        {
            calculate_paths(nodes[node.m_down_neighbour_idx.value()], nodes, paths);
        }

        if (!node.m_down_neighbour_idx.has_value() && !node.m_left_neighbour_idx.has_value())
        {
            ++paths;
        }
    }
}

namespace Better
{
    using Common::Node;

    // Iterates once
    template <unsigned WIDTH, unsigned HEIGHT>
    void calculate_paths(std::array<Node<WIDTH, HEIGHT>, WIDTH * HEIGHT> &nodes, unsigned long &paths)
    {
        nodes[0].m_paths = 1; // first path

        // Each node has as many possible paths as its parents
        for (auto &node : nodes)
        {
            // std::cout << "at(" << node.m_x << ", " << node.m_y << ") = " << node.m_weight << "\n";
            if (node.m_left_neighbour_idx.has_value())
            {
                nodes[node.m_left_neighbour_idx.value()].m_paths += node.m_paths;
            }

            if (node.m_down_neighbour_idx.has_value())
            {
                nodes[node.m_down_neighbour_idx.value()].m_paths += node.m_paths;
            }
        }

        paths = nodes.back().m_paths;
    }

}

namespace Math
{
    template <size_t NUM>
    struct power2
    {
        static constexpr size_t m_value = NUM * NUM;
    };

    template <unsigned long NUM>
    struct factorial
    {
        static constexpr size_t m_value = NUM * factorial<NUM - 1U>::m_value;
    };

    template <>
    struct factorial<0>
    {
        static constexpr size_t m_value = 1U;
    };

    template <unsigned WIDTH, unsigned HEIGHT>
    void calculate_paths(unsigned long &paths)
    {
        constexpr unsigned long grid_width = WIDTH - 1U;

        // with central binomial coefficient
        paths = (factorial<2U * grid_width>::m_value) / (power2<factorial<grid_width>::m_value>::m_value);
    }
}

static void benchmark_calculate_paths_brute(benchmark::State &state)
{
    for (auto _ : state)
    {
        unsigned long paths{0UL};

        auto nodes = Common::build_nodes<13U, 13U>();
        Brute::calculate_paths(nodes[0], nodes, paths);

        //std::cout << "paths: " << paths << "\n";
    }
}
BENCHMARK(benchmark_calculate_paths_brute);

static void benchmark_calculate_paths_better(benchmark::State &state)
{
    for (auto _ : state)
    {
        unsigned long paths{0UL};

        auto nodes = Common::build_nodes<13U, 13U>();
        Better::calculate_paths(nodes, paths);

        //std::cout << "paths: " << paths << "\n";
    }
}
BENCHMARK(benchmark_calculate_paths_better);

static void benchmark_calculate_paths_central_binomial_coefficient(benchmark::State &state)
{
    for (auto _ : state)
    {
        unsigned long paths{0UL};

        //TODO: wrong calculation, type too short
        Math::calculate_paths<13U, 13U>(paths);

        //std::cout << "paths: " << paths << "\n";
    }
}
BENCHMARK(benchmark_calculate_paths_central_binomial_coefficient);

BENCHMARK_MAIN();

// int main()
// {
//     {
//         unsigned long paths{0UL};

//         auto nodes = Common::build_nodes<10U, 10U>();
//         Brute::calculate_paths(nodes[0], nodes, paths);

//         std::cout << "(brute force) paths: " << paths << "\n";
//     }

//     {
//         unsigned long paths{0UL};

//         auto nodes = Common::build_nodes<10U, 10U>();
//         Better::calculate_paths(nodes, paths);

//         std::cout << "0: " << Math::factorial<0>::m_value << "\n";
//         std::cout << "1: " << Math::factorial<1>::m_value << "\n";
//         std::cout << "2: " << Math::factorial<2>::m_value << "\n";
//         std::cout << "3: " << Math::factorial<3>::m_value << "\n";
//         std::cout << "4: " << Math::factorial<4>::m_value << "\n";

//         std::cout << "(better) paths: " << paths << "\n";
//     }

//     {
//         unsigned long paths{0UL};

//         Math::calculate_paths<10U, 10U>(paths);

//         std::cout << "(math) paths: " << paths << "\n";
//     }

//     return 0;
// }
