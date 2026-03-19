/** @file get_racial_modifiers.h
 *  @brief Racial attribute modifier lookup helper.
 */

#pragma once

#include "race.h"

/** @brief Return the racial modifier for a race/stat-index pair. */
static inline int get_racial_modifiers(race_t r, int stat) {
    switch (r) {
    case race_t::HUMAN:
        if (stat >= 0 && stat < 6) return 0;
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
        // other race cases below...
    default: break;
    }


    return 0;
}
