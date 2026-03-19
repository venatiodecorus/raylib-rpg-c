/** @file get_racial_hd.h
 *  @brief Racial hit-die lookup helper.
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
