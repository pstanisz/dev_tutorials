// Copyright (c) 2024, Piotr Staniszewski

#include <iostream>
#include <limits>
#include <vector>

namespace simple
{

    /**
     * @brief A custom allocator that counts the number of allocated objects.
     *
     * This allocator provides the basic functionality to allocate and deallocate memory
     * for objects of type T. It also keeps track of the total number of allocated objects.
     *
     * @tparam T The type of the elements to allocate memory for.
     */
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
            std::cout << "Allocate memory for " << n << " element(s)\n";
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
            std::cout << "Deallocate memory\n";
            ::operator delete(ptr);

            m_allocated_objs = 0;
            std::cout << "Memory allocated: " << sizeof(T) * m_allocated_objs << "\n";
        }

    private:
        size_type m_allocated_objs;
    };

}

namespace medium
{

    /**
     * @brief Custom allocator that allocates memory from a fixed-size area.
     *
     * This allocator provides a fixed-size memory area from which memory is allocated and deallocated.
     * It ensures that the allocated memory does not exceed a predefined maximum size.
     *
     * @tparam T The type of elements to allocate memory for.
     */
    template <typename T>
    class Overwritting_allocator
    {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        static constexpr const size_type MAX = 1024;

        Overwritting_allocator() noexcept
        {
            std::cout << "Overwritting_allocator ctor\n";
        }

        template <typename U>
        Overwritting_allocator(const Overwritting_allocator<U> &other) noexcept
        {
            std::cout << "Overwritting_allocator copy ctor\n";
        }

        ~Overwritting_allocator() noexcept
        {
            std::cout << "Overwritting_allocator dtor\n";
        }

        [[nodiscard]] constexpr T *allocate(size_type n)
        {
            std::cout << "Allocate memory for " << n << " element(s)\n";
            if (n > MAX)
            {
                throw std::bad_alloc();
            }
            T *ptr = &m_fixed_memory_area[0];

            std::cout << "Memory allocated at: " << std::hex << &m_fixed_memory_area[0] << std::dec << "\n";

            return ptr;
        }

        constexpr void deallocate(T *, size_type) noexcept
        {
            std::cout << "Deallocate memory\n";

            std::cout << "Memory deallocated at: " << std::hex << &m_fixed_memory_area[0] << std::dec << "\n";
        }

    private:
        T m_fixed_memory_area[MAX];
    };

}

int main()
{
    std::cout << "allocators\n";

    std::vector<int, simple::Counting_allocator<int>> vec1;
    for (int i = 0; i < 10; ++i)
    {
        vec1.push_back(i);
    }

    std::vector<int, medium::Overwritting_allocator<int>> vec2;
    for (int i = 10; i < 20; ++i)
    {
        vec2.push_back(i);
    }

    for (auto item : vec2)
    {
        std::cout << item << ", ";
    }

    std::cout << "\n";

    return EXIT_SUCCESS;
}
