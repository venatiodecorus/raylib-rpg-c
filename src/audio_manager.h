/** @file audio_manager.h
 *  @brief Centralized audio subsystem for music, SFX, and queued playback.
 */

#pragma once

#include "libgame_defines.h"
#include <random>
#include <raylib.h>
#include <vector>

/**
 * @brief Manages all audio state: loaded sound effects, music stream,
 *        volume levels, and queued gameplay audio requests.
 */
class AudioManager {
public:
    float master_volume  = DEFAULT_MASTER_VOLUME;
    float music_volume   = DEFAULT_MUSIC_VOLUME;
    float sfx_volume     = DEFAULT_MASTER_VOLUME;
    bool music_volume_changed = false;
    unsigned int current_music_index = 0;
    bool audio_device_initialized = false;
    bool music_loaded = false;

    Music music = {};
    std::vector<Sound> sfx;
    std::vector<int> pending_sfx;

    static constexpr float VOLUME_STEP = 0.1f;

    void adjust_master(int dir);
    void adjust_music(int dir);
    void adjust_sfx(int dir);
    void apply_settings(bool test_mode = false);

    void queue(int sfx_id);
    void flush(bool test_mode = false);
    void load_sfx_assets();
    void load_random_music(std::mt19937& rng);
    void init(std::mt19937& rng, bool test_mode = false);
    void update_music(std::mt19937& rng, bool test_mode = false);
    void shutdown();

    float get_master_volume() const { return master_volume; }
    float get_music_volume() const { return music_volume; }
    float get_sfx_volume() const { return sfx_volume; }
    bool get_music_volume_changed() const { return music_volume_changed; }
    unsigned int get_current_music_index() const { return current_music_index; }

    void set_master_volume(float v) { master_volume = v; }
    void set_music_volume(float v) { music_volume = v; }
    void set_sfx_volume(float v) { sfx_volume = v; }
    void set_music_volume_changed(bool v) { music_volume_changed = v; }
    void set_current_music_index(unsigned int v) { current_music_index = v; }

    /** @brief Reset all volumes and transient playback state to defaults. */
    void reset_defaults();
};
