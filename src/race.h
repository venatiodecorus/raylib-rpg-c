/** @file race.h
 *  @brief Race identifiers and string conversion helpers.
 */

#pragma once

#include "massert.h"
#include <string.h>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

/// @brief Supported player and NPC races used by factories and gameplay logic.
enum class race_t
{
    NONE = 0,
    HALFLING,
    GOBLIN,
    HUMAN,
    ELF,
    DWARF,
    ORC,
    GREEN_SLIME,
    BAT,
    WOLF,
    WARG,
    ZOMBIE,
    SKELETON,
    RAT,
    COUNT
};

/// @brief Mapping from race enum values to lowercase display/debug names.
const unordered_map<race_t, string> rn = {{race_t::NONE, "race_none"},
                                          {race_t::COUNT, "race_count"},
                                          {race_t::HUMAN, "human"},
                                          {race_t::HALFLING, "halfling"},
                                          {race_t::GOBLIN, "goblin"},
                                          {race_t::ELF, "elf"},
                                          {race_t::DWARF, "dwarf"},
                                          {race_t::ORC, "orc"},
                                          {race_t::GREEN_SLIME, "green slime"},
                                          {race_t::BAT, "bat"},
                                          {race_t::WOLF, "wolf"},
                                          {race_t::WARG, "warg"},
                                          {race_t::ZOMBIE, "zombie"},
                                          {race_t::SKELETON, "skeleton"},
                                          {race_t::RAT, "rat"}};

/** @brief Convert a race enum into its configured string label. */
const static inline string race2str(const race_t r) {
    massert(r >= race_t::NONE && r < race_t::COUNT, "Invalid race name");
    massert(rn.find(r) != rn.end(), "race doesn't exist in map!: %d", static_cast<int>(r));
    return rn.at(r);
}

/** @brief Convert a configured race string into a race enum, or `race_t::NONE`. */
const static inline race_t str2race(const string& str) {
    for (const auto& i : rn)
        if (str == i.second)
            return i.first;
    return race_t::NONE;
}
