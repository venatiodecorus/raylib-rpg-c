/** @file environments.h
 *  @brief Environment identifiers for world or content tagging.
 */

#pragma once

/// @brief Broad environment categories available to world/content systems.
enum class environment_t
{
    NONE,
    DESERT,
    FOREST,
    DUNGEON,
    CAVE,
    TOWN,
    TUNDRA,
    WASTELAND,
    HELLSCAPE,
    COUNT
};
