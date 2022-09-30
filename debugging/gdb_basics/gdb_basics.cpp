// Copyright (c) 2022, Piotr Staniszewski

#include <iostream>
#include <cstring>

bool has_character(const char *sentence, char character)
{
    auto len = strlen(sentence);
    for (auto idx = 0U; idx < len; ++idx)
    {
        if (sentence[idx] == character)
        {
            return true;
        }
    }

    return false;
}

void replace_character(char *sentence, char old_char, char new_char)
{
    auto len = strlen(sentence);
    for (auto idx = 0U; idx < len; ++idx)
    {
        if (sentence[idx] == old_char)
        {
            sentence[idx] = new_char;
        }
    }
}

int main()
{
    std::cout << "Basics of gdb\n";

    char sentence[] = "Hello, World!";
    std::cout << "First sentence: " << sentence << "\n";
    if (has_character(sentence, 'H'))
    {
        replace_character(sentence, 'H', 'Y');
    }
    std::cout << "After replacing 'H' with 'Y': " << sentence << "\n";

    const char* sentence2 = "Good bye, World!";
    std::cout << "Second sentence: " << sentence2 << "\n";
    if (has_character(sentence2, 'o'))
    {
        replace_character((char*)sentence2, 'o', 'O');
    }
    std::cout << "After replacing 'o' with 'O': " << sentence2 << "\n";

    return EXIT_SUCCESS;
}
