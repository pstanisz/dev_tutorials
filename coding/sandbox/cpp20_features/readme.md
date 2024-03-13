# C++20 features

Examples of **C++20** features.

## Modules

For my environment it was necessary to build the iostream and concepts modules first using:

```cpp
    g++ -std=c++20 -fmodules-ts -xc++-system-header iostream
    g++ -std=c++20 -fmodules-ts -xc++-system-header concepts
```
