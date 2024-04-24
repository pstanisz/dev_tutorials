// Copyright (c) 2024, Piotr Staniszewski

#include <iostream>
#include <limits>
#include <vector>

namespace simple
{

    template <typename T>
    class Counting_allocator
    {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        Counting_allocator() noexcept : m_allocated_objs{0}
        {
            std::cout << "Counting_allocator ctor\n";
        }

        template <typename U>
        Counting_allocator(const Counting_allocator<U> &other) noexcept : m_allocated_objs{other.m_allocated_objs}
        {
            std::cout << "Counting_allocator copy ctor\n";
        }

        ~Counting_allocator() noexcept
        {
            std::cout << "Counting_allocator dtor\n";
        }

        [[nodiscard]] constexpr T *allocate(size_type n)
        {
            std::cout << "Counting_allocator::allocate memory for " << n << " element(s)\n";
            if (n > std::numeric_limits<size_type>::max() / sizeof(T))
            {
                throw std::bad_alloc();
            }
            T *ptr = static_cast<T *>(::operator new(n * sizeof(T)));
            if (!ptr)
            {
                throw std::bad_alloc();
            }

            m_allocated_objs += n;
            std::cout << "Memory allocated: " << sizeof(T) * m_allocated_objs << "\n";

            return ptr;
        }

        constexpr void deallocate(T *ptr, size_type) noexcept
        {
            std::cout << "Counting_allocator::deallocate memory\n";
            ::operator delete(ptr);

            m_allocated_objs = 0;
            std::cout << "Memory allocated: " << sizeof(T) * m_allocated_objs << "\n";
        }

    private:
        size_type m_allocated_objs;
    };

}

int main()
{
    std::cout << "allocators\n";

    std::vector<int, simple::Counting_allocator<int>> vec;
    vec.push_back(1); 
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    vec.push_back(6);

    return EXIT_SUCCESS;
}
