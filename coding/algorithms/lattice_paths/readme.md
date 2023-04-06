# Lattice paths

Starting in the top left corner of a 2×2 grid, and only being able to move to the right and down, there are exactly 6 routes to the bottom right corner.
How many such routes are there through a 20×20 grid?

Source: https://projecteuler.net/

## Benchmarked with grid of 12x12

Grid size selected to make at least a few iterations with brute-force.

```shell
---------------------------------------------------------------------------
Benchmark                                 Time             CPU   Iterations
---------------------------------------------------------------------------
benchmark_calculate_paths_brute   148255160 ns    147876191 ns            4
benchmark_calculate_paths_better      10254 ns        10243 ns        64192
```
