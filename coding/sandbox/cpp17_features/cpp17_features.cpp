// Copyright (c) 2022, Piotr Staniszewski

#include <iostream>
#include <iomanip>

template <typename T>
void dump(const T c)
{
    const uint8_t *data{reinterpret_cast<const uint8_t *>(&c)};

    std::cout << std::hex << std::setfill('0');
    for (auto idx{0U}; idx < sizeof(T); ++idx)
    {
        std::cout << std::setw(2) << static_cast<unsigned>(data[idx]) << " ";
    }

    std::cout << std::endl;
}

template <typename T>
void print(std::string_view desc, const T c)
{
    std::cout << desc << ":\t" << c << "\t";
    dump(c);
}

void char_literals()
{
    // 'a' in UTF-8
    const char c1 = u8'a';
    print("UTF-8", c1);

    // 'a' in UTF-16
    const char16_t c2 = u'a';
    print("UTF-16", c2);

    // 'a' in UTF-32
    const char32_t c3 = U'a';
    print("UTF-32", c3);

    // 'a' in wide character literal
    const wchar_t c4 = L'a';
    print("Wide", c4);
}

int main()
{
    std::cout << "C++17 features\n";

    char_literals();

    return EXIT_SUCCESS;
}
