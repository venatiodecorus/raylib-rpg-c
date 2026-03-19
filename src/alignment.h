/** @file alignment.h
 *  @brief Alignment identifiers and cycling/string helpers.
 */

#pragma once

#include <string>

/// @brief Moral and ethical alignment values used by characters and NPCs.
enum class alignment_t {
    NONE = 0,
    GOOD_LAWFUL,
    GOOD_NEUTRAL,
    GOOD_CHAOTIC,
    NEUTRAL_LAWFUL,
    NEUTRAL_NEUTRAL,
    NEUTRAL_CHAOTIC,
    EVIL_LAWFUL,
    EVIL_NEUTRAL,
    EVIL_CHAOTIC,
    COUNT,
};

/** @brief Convert an alignment enum into a user-facing string. */
static inline std::string alignment_to_str(alignment_t alignment) {
    switch (alignment) {
    case alignment_t::NONE: return "none";
    case alignment_t::GOOD_LAWFUL: return "lawful good";
    case alignment_t::GOOD_NEUTRAL: return "neutral good";
    case alignment_t::GOOD_CHAOTIC: return "chaotic good";
    case alignment_t::NEUTRAL_LAWFUL: return "lawful neutral";
    case alignment_t::NEUTRAL_NEUTRAL: return "true neutral";
    case alignment_t::NEUTRAL_CHAOTIC: return "chaotic neutral";
    case alignment_t::EVIL_LAWFUL: return "lawful evil";
    case alignment_t::EVIL_NEUTRAL: return "neutral evil";
    case alignment_t::EVIL_CHAOTIC: return "chaotic evil";
    case alignment_t::COUNT: return "alignment count";
    default: break;
    }
    return "invalid alignment";
}

/** @brief Return the next selectable non-none alignment, wrapping at the end. */
static inline alignment_t alignment_next(alignment_t alignment) {
    if (alignment < alignment_t::GOOD_LAWFUL || alignment >= alignment_t::EVIL_CHAOTIC) {
        return alignment_t::GOOD_LAWFUL;
    }
    return static_cast<alignment_t>(static_cast<int>(alignment) + 1);
}

/** @brief Return the previous selectable non-none alignment, wrapping at the start. */
static inline alignment_t alignment_prev(alignment_t alignment) {
    if (alignment <= alignment_t::GOOD_LAWFUL || alignment >= alignment_t::COUNT) {
        return alignment_t::EVIL_CHAOTIC;
    }
    return static_cast<alignment_t>(static_cast<int>(alignment) - 1);
}
