/** @file racial_stats.h
 *  @brief Racial hit-die and attribute modifier lookup helpers.
 */

#pragma once

#include "race.h"

/** @brief Return the default hit-die size for a race. */
static inline const int get_racial_hd(race_t rt) {
    switch (rt) {
    case race_t::HUMAN: return 8;
    case race_t::ELF: return 6;
    case race_t::DWARF: return 10;
    default: break;
    }
    return 8;
}

/** @brief Return the racial modifier for a race/stat-index pair. */
static inline int get_racial_modifiers(race_t r, int stat) {
    switch (r) {
    case race_t::HUMAN:
        return 0;
    case race_t::ELF:
        switch (stat) {
        case 0: return 0;
        case 1: return 2;
        case 2: return 0;
        case 3: return 0;
        case 4: return -2;
        case 5: return 0;
        default: return 0;
        }
    case race_t::DWARF:
        switch (stat) {
        case 0: return 0;
        case 1: return 0;
        case 2: return 0;
        case 3: return 0;
        case 4: return 2;
        case 5: return -2;
        default: return 0;
        }
    case race_t::GREEN_SLIME:
        switch (stat) {
        case 0: return -2;
        case 1: return -2;
        case 2: return -2;
        case 3: return -2;
        case 4: return -2;
        case 5: return -2;
        default: return 0;
        }
    default: break;
    }

    return 0;
}
