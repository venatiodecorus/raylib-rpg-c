/** @file audio_manager.h
 *  @brief Centralized audio subsystem — owns SFX handles and volume state.
 */

#pragma once

#include "libgame_defines.h"
#include <raylib.h>
#include <vector>

/**
 * @brief Manages all audio state: loaded sound effects, music stream,
 *        and volume levels.
 *
 * Extracted from the former gamestate god-object and libdraw_audio_state_t.
 * The renderer still owns the Music stream for now (load_music.h), but
 * volume control and SFX playback are routed through this class.
 */
class AudioManager {
public:
    // --- Volume state (formerly in libdraw_audio_state_t) ---
    float master_volume  = DEFAULT_MASTER_VOLUME;
    float music_volume   = DEFAULT_MUSIC_VOLUME;
    float sfx_volume     = DEFAULT_MASTER_VOLUME;
    bool  music_volume_changed = false;
    unsigned int current_music_index = 0;

    // --- Loaded SFX handles (formerly gamestate::sfx) ---
    std::vector<Sound> sfx;

    // --- Volume adjustment ---
    static constexpr float VOLUME_STEP = 0.1f;

    void adjust_master(int dir);
    void adjust_music(int dir);
    void adjust_sfx(int dir);
    void apply_settings(bool test_mode = false);

    // --- Convenience getters/setters (const-correct) ---
    float get_master_volume() const { return master_volume; }
    float get_music_volume()  const { return music_volume; }
    float get_sfx_volume()    const { return sfx_volume; }
    bool  get_music_volume_changed() const { return music_volume_changed; }
    unsigned int get_current_music_index() const { return current_music_index; }

    void set_master_volume(float v) { master_volume = v; }
    void set_music_volume(float v)  { music_volume = v; }
    void set_sfx_volume(float v)    { sfx_volume = v; }
    void set_music_volume_changed(bool v) { music_volume_changed = v; }
    void set_current_music_index(unsigned int v) { current_music_index = v; }

    /** @brief Play a sound effect by SFX ID index. No-op if test mode or device not ready. */
    void play(int sfx_id, bool test_mode = false) const;

    /** @brief Reset all volumes to defaults. */
    void reset_defaults();
};
