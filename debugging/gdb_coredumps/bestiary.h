// Copyright (c) 2022, Piotr Staniszewski

#include <cstddef>
#include <cstdint>
#include <string>

namespace Bestiary
{
    using Strength = uint8_t;
    using Constitution = uint8_t;
    using Intelligence = uint8_t;
    using Charisma = uint8_t;
    using Luck = uint8_t;

    /*
     * Represents the attributes of beast
     */
    enum class Attribute : uint8_t
    {
        Strenght = 0,
        Constitution,
        Intelligence,
        Charisma,
        Luck
    };

    /*
     * Represents the result of the fight
     */
    enum class Fight_result : uint8_t
    {
        Win = 0,
        Draw,
        Lost
    };

    std::string fight_result_to_string(Fight_result result);

    /*
     * Represents the beast in the bestiary
     */
    struct Beast
    {
        std::string m_name;
        Strength m_strength{0U};
        Constitution m_constitution{0U};
        Intelligence m_intelligence{0U};
        Charisma m_charisma{0U};
        Luck m_luck{0U};
    };

    /*
     * Adds the beast to the bestiary
     * @param beast to be added
     * @return true if added; false otherwise
     */
    void add_beast(const Beast &beast);

    /*
     * Prints all of the beasts
     */
    void print_beasts();

    /*
     * Aggressor initiates the fight with defender by choosing the attribute
     * @param aggressor
     * @param defender
     * @param attribute
     * @return result of the fight for the aggressor
     */
    Fight_result fight(const std::string &aggressor, const std::string &defender, Attribute attribute);

}
