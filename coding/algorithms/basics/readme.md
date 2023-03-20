# Algorithms basics

Here I'll describe what the algorithms are starting from the very beginning.

## What is an algorithm

An algorithm is a procedure for solving a well-defined problem or performing a computation in a finite number of steps.

An algorithm (f) takes an arbitary number of inputs (I) to produce desired outputs (O).

```
f : I -> O
```
### Correctness

We can say that algorithm is correct if for every input it produces an output that belongs to the output domain and is correct.

Induction hypothesis TBD

### Finiteness

An algorithm must be finite, in particular: it should have a limited number of instructions.

### Efficiency

How fast the algorithm runs?

Do not measure the time

Count the fundamental operations
* integer arithmetics
* logical ops
* bitwise ops
* read from address in memory
* write to address in memory

Limitation to operate on constant amount of memory at once (i.e.: 2 words)

Performance to depend on side of input (n)

Asymptotic / Worst case / Average case / Amortized

O() upper bounds

$\Omega$() lower bounds

$\Theta$() both

Functions that relates an algorithm input size to the running time

O(1) - constant time (does not depend on input size at all)

O(logn) - logarithmic time

O(n) - linear time

O(nlogn) - log linear time

O(n<sup>2</sup>) - quadratic time

O(n<sup>3</sup>) - cubic time

O(n<sup>k</sup>) - polynomial time for some k

O(c<sup>c</sup>) - exponential time for some c > 1

## Algorithm example

Check,  whether there are the same bricks in the box

Inductive hypothesis:

   if first k bricks contain a match
      algorithm returns a match
   bricks k++

Base case: k = 0 -> no match
Assume inductive hypothesis for k = k'


