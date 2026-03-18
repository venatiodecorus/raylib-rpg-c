/** @file load_sfx.h
 *  @brief Sound-effect loading from code-defined paths.
 */

#pragma once

#include "gamestate.h"
#include "audio_defs.h"

/** @brief Load all sound effects from the compiled SFX_PATHS table. */
static inline void libdraw_load_sfx(gamestate& g) {
    g.sfx.clear();
    for (int i = 0; i < SFX_DEF_COUNT; i++) {
        Sound sound = LoadSound(SFX_PATHS[i]);
        SetSoundVolume(sound, g.get_sfx_volume());
        g.sfx.push_back(sound);
    }
}
