// Copyright (c) 2022, Piotr Staniszewski

#include <iostream>
#include <vector>

#include "bestiary.h"

class Arena // Unused
{
public:
    Arena();

    std::vector<Bestiary::Beast> m_beasts;
};

int main()
{
    std::cout << "Bestiary game\n";

    using namespace Bestiary;

    add_beast({"goblin",
               Strength(2U),
               Constitution(2U),
               Intelligence(1U),
               Charisma(2U),
               Luck(0U)});

    add_beast({"troll",
               Strength(5U),
               Constitution(4U),
               Intelligence(1U),
               Charisma(1U),
               Luck(1U)});

    print_beasts();

    size_t fight_count{1000U};
    size_t troll_wins{0U};
    for (size_t idx = 0; idx < fight_count; ++idx)
    {
        Attribute attr = static_cast<Attribute>(idx % 4U);
        if (fight("troll", "goblin", attr) == Fight_result::Win)
        {
            ++troll_wins;
        }
        std::cout << idx << " ";
    }

    std::cout << "Troll vs goblin: Troll won " << troll_wins << " times in " << fight_count << " rounds\n";

    return EXIT_SUCCESS;
}
