// Copyright (c) 2022, Piotr Staniszewski

#include <iostream>

class A
{
    int m_data[5];

public:
    A() : m_data{1, 2, 3, 4, 5}
    {
    }
};

class B
{
    int m_data[5];

public:
    B() : m_data{1, 2, 3, 4, 5}
    {
    }

    virtual void foo() { std::cout << "foo called\n"; }
    virtual void bar() { std::cout << "bar called\n"; }
};

int main()
{
    std::cout << "vtable ptr\n";

    A obj_a;

    void* obj_a_ptr = &obj_a;
    int *a_data = reinterpret_cast<int *>(&obj_a);
    std::cout << "obj_a ptr = " << obj_a_ptr << "\n";
    std::cout << "a_data[0] ptr = " << &a_data[0] << "\n";
    std::cout << "a_data[0] = " << a_data[0] << "\n";

    B obj_b;

    void* obj_b_ptr = &obj_b;
    int *b_data = reinterpret_cast<int *>(&obj_b);
    std::cout << "obj_b ptr = " << obj_b_ptr << "\n";
    std::cout << "b_data[2] ptr = " << &b_data[2] << "\n";
    std::cout << "b_data[2] = " << b_data[2] << "\n";

    // Valid for 64-bit architecture
    static_assert(sizeof(void*) == 8U);

    unsigned long* vtable_address = (unsigned long*)&obj_b;
    std::cout << "obj_b vtable address = " << vtable_address << "\n";

    typedef void(*Fun_from_vtable)(void*);
    Fun_from_vtable foo = (Fun_from_vtable)(*(unsigned long*)(*vtable_address));
    Fun_from_vtable bar = (Fun_from_vtable)(*(unsigned long*)(*vtable_address + 8U));
    
    foo(&obj_b);
    bar(&obj_b);

    return EXIT_SUCCESS;
}
