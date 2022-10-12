// Copyright (c) 2022, Piotr Staniszewski

#include "bestiary.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <random>

namespace Bestiary
{
    /*
     * Holds all of the beasts
     */
    static std::vector<Beast> g_beasts;

    const Beast &find_beast(const std::string &name)
    {

        const auto it = std::find_if(std::begin(g_beasts), std::end(g_beasts), [&name](const Beast &beast)
                                     { return beast.m_name == name; });
        if (it != std::end(g_beasts))
        {
            return *it;
        }

        throw std::runtime_error(std::string(name).append(": beast does not exist"));
    }

    template <Attribute A>
    Fight_result calculate(const Beast &aggressor, const Beast &defender)
    {
        return Fight_result::Draw;
    }

    template <>
    Fight_result calculate<Attribute::Strenght>(const Beast &aggressor, const Beast &defender)
    {
        return (aggressor.m_strength > defender.m_strength ? Fight_result::Win : (aggressor.m_strength == defender.m_strength ? Fight_result::Draw : Fight_result::Lost));
    }

    template <>
    Fight_result calculate<Attribute::Constitution>(const Beast &aggressor, const Beast &defender)
    {
        return (aggressor.m_constitution > defender.m_constitution ? Fight_result::Win : (aggressor.m_constitution == defender.m_constitution ? Fight_result::Draw : Fight_result::Lost));
    }

    template <>
    Fight_result calculate<Attribute::Intelligence>(const Beast &aggressor, const Beast &defender)
    {
        return (aggressor.m_intelligence > defender.m_intelligence ? Fight_result::Win : (aggressor.m_intelligence == defender.m_intelligence ? Fight_result::Draw : Fight_result::Lost));
    }

    template <>
    Fight_result calculate<Attribute::Charisma>(const Beast &aggressor, const Beast &defender)
    {
        return (aggressor.m_charisma > defender.m_charisma ? Fight_result::Win : (aggressor.m_charisma == defender.m_charisma ? Fight_result::Draw : Fight_result::Lost));
    }

    void add_beast(const Beast &beast)
    {
        g_beasts.push_back(beast);
    }

    void print_beasts()
    {
        for (const auto &beast : g_beasts)
        {
            std::cout << "Name:\t\t" << beast.m_name << "\n";
            std::cout << "Strength:\t" << static_cast<int>(beast.m_strength) << "\n";
            std::cout << "Constitution:\t" << static_cast<int>(beast.m_constitution) << "\n";
            std::cout << "Intelligence:\t" << static_cast<int>(beast.m_intelligence) << "\n";
            std::cout << "Charisma:\t" << static_cast<int>(beast.m_charisma) << "\n";
            std::cout << "Luck:\t\t" << static_cast<int>(beast.m_luck) << "\n";
        }
    }

    std::string fight_result_to_string(Fight_result result)
    {
        switch (result)
        {
        case Fight_result::Win:
            return "Win";
        case Fight_result::Draw:
            return "Draw";
        case Fight_result::Lost:
            return "Lost";
        default:
            throw std::runtime_error("Unrecognized fight result");
        }
    }

    Fight_result lucky_shot(Fight_result result, Luck aggr_luck, Luck def_luck)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0U, 100U);
        uint16_t rand_val = distr(gen);
        if ((aggr_luck / (def_luck + rand_val)) >= 1U) // Bug here!
        {
            result = Fight_result::Win;
        }

        return result;
    }

    Fight_result fight(const std::string &aggressor, const std::string &defender, Attribute attribute)
    {
        Fight_result result;

        const auto &aggr = find_beast(aggressor);
        const auto &def = find_beast(defender);

        switch (attribute)
        {
        case Attribute::Strenght:
            result = calculate<Attribute::Strenght>(aggr, def);
            break;
        case Attribute::Constitution:
            result = calculate<Attribute::Constitution>(aggr, def);
            break;
        case Attribute::Intelligence:
            result = calculate<Attribute::Intelligence>(aggr, def);
            break;
        case Attribute::Charisma:
            result = calculate<Attribute::Charisma>(aggr, def);
            break;
        case Attribute::Luck:
            throw std::runtime_error("Cannot use Luck for fight");
        default:
            throw std::runtime_error("Unrecognized attribute");
        }

        // Dummy luck calculation for defeated aggressor
        if (result != Fight_result::Win)
        {
            result = lucky_shot(result, aggr.m_luck, def.m_luck);
        }

        return result;
    }

}
