/** @file audio_manager.cpp
 *  @brief AudioManager implementation.
 */

#include "audio_manager.h"
#include "libdraw_context.h"
#include <algorithm>

void AudioManager::adjust_master(int dir) {
    master_volume = std::clamp(master_volume + VOLUME_STEP * dir, 0.0f, 1.0f);
    apply_settings();
}

void AudioManager::adjust_music(int dir) {
    music_volume = std::clamp(music_volume + VOLUME_STEP * dir, 0.0f, 1.0f);
    music_volume_changed = true;
    apply_settings();
}

void AudioManager::adjust_sfx(int dir) {
    sfx_volume = std::clamp(sfx_volume + VOLUME_STEP * dir, 0.0f, 1.0f);
    apply_settings();
}

void AudioManager::apply_settings(bool test_mode) {
    master_volume = std::clamp(master_volume, 0.0f, 1.0f);
    music_volume  = std::clamp(music_volume, 0.0f, 1.0f);
    sfx_volume    = std::clamp(sfx_volume, 0.0f, 1.0f);

    // Sync to the libdraw audio state (renderer still reads these for music stream)
    libdraw_ctx.audio.master_volume = master_volume;
    libdraw_ctx.audio.music_volume  = music_volume;
    libdraw_ctx.audio.sfx_volume    = sfx_volume;
    libdraw_ctx.audio.music_volume_changed = music_volume_changed;
    libdraw_ctx.audio.current_music_index  = current_music_index;

    if (test_mode || !IsAudioDeviceReady()) {
        return;
    }

    SetMasterVolume(master_volume);
    SetMusicVolume(libdraw_ctx.audio.music, music_volume);
    for (size_t i = 0; i < sfx.size(); i++) {
        SetSoundVolume(sfx[i], sfx_volume);
    }
}

void AudioManager::play(int sfx_id, bool test_mode) const {
    if (test_mode || !IsAudioDeviceReady()) return;
    if (sfx_id < 0 || sfx_id >= static_cast<int>(sfx.size())) return;
    PlaySound(sfx[sfx_id]);
}

void AudioManager::reset_defaults() {
    master_volume = DEFAULT_MASTER_VOLUME;
    music_volume  = DEFAULT_MUSIC_VOLUME;
    sfx_volume    = DEFAULT_MASTER_VOLUME;
    music_volume_changed = false;
    current_music_index  = 0;
}
