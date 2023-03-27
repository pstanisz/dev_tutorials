# Largest palindrome produce

A palindromic number reads the same both ways. The largest palindrome made from the product of two 2-digit numbers is 9009 = 91 x 99.
Find the largest palindrome made from the product of two 3-digit numbers.

Source: https://projecteuler.net/

The purpose here is to try out multiple approaches and find out which one is the most efficient.

```
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
benchmark_brute_force          17402340 ns     17393505 ns           35
benchmark_brute_force_better   17419466 ns     17410809 ns           41
benchmark_first_idea            1579648 ns      1578833 ns          443
benchmark_is_palindrome            26.4 ns         26.3 ns     27409280
```
