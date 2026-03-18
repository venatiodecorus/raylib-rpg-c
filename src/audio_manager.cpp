/** @file audio_manager.cpp
 *  @brief AudioManager implementation.
 */

#include "audio_manager.h"
#include "audio_defs.h"
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
    music_volume = std::clamp(music_volume, 0.0f, 1.0f);
    sfx_volume = std::clamp(sfx_volume, 0.0f, 1.0f);

    if (test_mode || !audio_device_initialized || !IsAudioDeviceReady()) {
        return;
    }

    SetMasterVolume(master_volume);
    if (music_loaded) {
        SetMusicVolume(music, music_volume);
    }
    for (size_t i = 0; i < sfx.size(); i++) {
        SetSoundVolume(sfx[i], sfx_volume);
    }
}

void AudioManager::queue(int sfx_id) {
    if (sfx_id >= 0) {
        pending_sfx.push_back(sfx_id);
    }
}

void AudioManager::flush(bool test_mode) {
    if (test_mode || !audio_device_initialized || !IsAudioDeviceReady()) {
        pending_sfx.clear();
        return;
    }

    for (const int sfx_id : pending_sfx) {
        if (sfx_id < 0 || sfx_id >= static_cast<int>(sfx.size())) {
            continue;
        }
        PlaySound(sfx[sfx_id]);
    }

    pending_sfx.clear();
}

void AudioManager::load_sfx_assets() {
    for (Sound sound : sfx) {
        UnloadSound(sound);
    }

    sfx.clear();
    sfx.reserve(SFX_DEF_COUNT);

    if (!audio_device_initialized || !IsAudioDeviceReady()) {
        return;
    }

    for (int i = 0; i < SFX_DEF_COUNT; i++) {
        Sound sound = LoadSound(SFX_PATHS[i]);
        SetSoundVolume(sound, sfx_volume);
        sfx.push_back(sound);
    }
}

void AudioManager::load_random_music(std::mt19937& rng) {
    if (!audio_device_initialized || !IsAudioDeviceReady()) {
        return;
    }

    if (music_loaded) {
        StopMusicStream(music);
        UnloadMusicStream(music);
        music = {};
        music_loaded = false;
    }

    std::uniform_int_distribution<int> gen(0, MUSIC_DEF_COUNT - 1);
    const int index = gen(rng);
    current_music_index = static_cast<unsigned int>(index);
    music = LoadMusicStream(MUSIC_PATHS[index]);
    music.looping = false;
    music_loaded = true;
}

void AudioManager::init(std::mt19937& rng, bool test_mode) {
    pending_sfx.clear();

#ifndef MASTER_VOLUME
    master_volume = DEFAULT_MASTER_VOLUME;
#else
    master_volume = MASTER_VOLUME;
#endif

#ifdef MUSIC_OFF
    music_volume = 0.0f;
#else
#ifdef MUSIC_VOLUME
    music_volume = MUSIC_VOLUME;
#else
    music_volume = DEFAULT_MUSIC_VOLUME;
#endif
#endif

    if (test_mode) {
        audio_device_initialized = false;
        music_loaded = false;
        return;
    }

    if (!IsAudioDeviceReady()) {
        InitAudioDevice();
        while (!IsAudioDeviceReady()) {
        }
    }

    audio_device_initialized = IsAudioDeviceReady();
    if (!audio_device_initialized) {
        return;
    }

    load_sfx_assets();
    load_random_music(rng);
    apply_settings();

    if (music_loaded) {
        PlayMusicStream(music);
    }
}

void AudioManager::update_music(std::mt19937& rng, bool test_mode) {
    if (test_mode || !audio_device_initialized || !music_loaded || !IsAudioDeviceReady()) {
        return;
    }

    UpdateMusicStream(music);
    if (IsMusicStreamPlaying(music)) {
        return;
    }

    load_random_music(rng);
    apply_settings();
    if (music_loaded) {
        PlayMusicStream(music);
    }
}

void AudioManager::shutdown() {
    pending_sfx.clear();

    if (audio_device_initialized && IsAudioDeviceReady()) {
        if (music_loaded) {
            StopMusicStream(music);
            UnloadMusicStream(music);
            music = {};
            music_loaded = false;
        }

        for (Sound sound : sfx) {
            UnloadSound(sound);
        }

        CloseAudioDevice();
    }

    sfx.clear();
    audio_device_initialized = false;
}

void AudioManager::reset_defaults() {
    master_volume = DEFAULT_MASTER_VOLUME;
    music_volume = DEFAULT_MUSIC_VOLUME;
    sfx_volume = DEFAULT_MASTER_VOLUME;
    music_volume_changed = false;
    current_music_index = 0;
    music = {};
    sfx.clear();
    pending_sfx.clear();
    audio_device_initialized = false;
    music_loaded = false;
}
