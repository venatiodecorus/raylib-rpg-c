/** @file attack_result.h
 *  @brief Outcome categories for resolved attack attempts.
 */

#pragma once

/// @brief Coarse result categories returned from attack resolution.
enum class attack_result_t
{
    NONE,
    HIT,
    MISS,
    BLOCK,
    COUNT,
};
