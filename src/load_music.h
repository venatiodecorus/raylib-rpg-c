/** @file load_music.h
 *  @brief Background music loading and stream-rotation helpers.
 */

#pragma once

#include "gamestate.h"
#include "libdraw_context.h"
#include "libgame_defines.h"
#include "audio_defs.h"
#include <random>

/** @brief Load one random music track from the compiled music path list. */
static inline void load_random_music(gamestate& g) {
    minfo2("BEGIN load_random_music");
    uniform_int_distribution<int> gen(0, MUSIC_DEF_COUNT - 1);
    const int index = gen(g.mt);
    const char* music_path = MUSIC_PATHS[index];
    minfo("Music path: %s", music_path);
    libdraw_ctx.audio.current_music_index = static_cast<unsigned int>(index);
    libdraw_ctx.audio.music = LoadMusicStream(music_path);
    libdraw_ctx.audio.music.looping = false;
    minfo2("END load_random_music");
}

/** @brief Advance the active music stream and rotate tracks when playback ends. */
static inline void handle_music_stream(gamestate& g) {
    UpdateMusicStream(libdraw_ctx.audio.music);
    if (!IsMusicStreamPlaying(libdraw_ctx.audio.music)) {
        StopMusicStream(libdraw_ctx.audio.music);
        UnloadMusicStream(libdraw_ctx.audio.music);
        load_random_music(g);

#ifdef MUSIC_VOLUME
        libdraw_ctx.audio.music_volume = MUSIC_VOLUME;
#endif

#ifdef MUSIC_OFF
        libdraw_ctx.audio.music_volume = 0.0f;
#endif

        SetMusicVolume(libdraw_ctx.audio.music, libdraw_ctx.audio.music_volume);
        PlayMusicStream(libdraw_ctx.audio.music);
    }
}

/** @brief Initialize music playback, master volume, and starting track state. */
static inline void libdraw_load_music(gamestate& g) {
    minfo2("BEGIN load_music");
    load_random_music(g);

#ifndef MASTER_VOLUME
    libdraw_ctx.audio.master_volume = DEFAULT_MASTER_VOLUME;
#else
    libdraw_ctx.audio.master_volume = MASTER_VOLUME;
#endif

#ifdef MUSIC_OFF
    libdraw_ctx.audio.music_volume = 0.0f;
#else
#ifdef MUSIC_VOLUME
    libdraw_ctx.audio.music_volume = MUSIC_VOLUME;
#else
    libdraw_ctx.audio.music_volume = DEFAULT_MUSIC_VOLUME;
#endif
#endif

    SetMasterVolume(libdraw_ctx.audio.master_volume);
    SetMusicVolume(libdraw_ctx.audio.music, libdraw_ctx.audio.music_volume);

    PlayMusicStream(libdraw_ctx.audio.music);
    minfo2("END load_music");
}
