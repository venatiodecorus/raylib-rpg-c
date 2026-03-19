/** @file rarity.h
 *  @brief Item rarity identifiers.
 */

#pragma once

/// @brief Supported rarity tiers for generated items or rewards.
enum class rarity_t
{
    NONE,
    COMMON,
    UNCOMMON,
    RARE,
    EPIC,
    LEGENDARY,
    COUNT,
};
