#pragma once

#include "../audio_manager.h"
#include <cxxtest/TestSuite.h>
#include <random>

class AudioManagerTestSuite : public CxxTest::TestSuite {
public:
    void testDefaultVolumes() {
        rpg::AudioManager am;
        TS_ASSERT_DELTA(am.get_master_volume(), DEFAULT_MASTER_VOLUME, 0.001f);
        TS_ASSERT_DELTA(am.get_music_volume(), DEFAULT_MUSIC_VOLUME, 0.001f);
        TS_ASSERT_DELTA(am.get_sfx_volume(), DEFAULT_MASTER_VOLUME, 0.001f);
    }

    void testAdjustMasterClampsAtZero() {
        rpg::AudioManager am;
        am.set_master_volume(0.0f);
        am.apply_settings(true);
        am.set_master_volume(std::clamp(am.get_master_volume() + rpg::AudioManager::VOLUME_STEP * -1, 0.0f, 1.0f));
        am.apply_settings(true);
        TS_ASSERT_DELTA(am.get_master_volume(), 0.0f, 0.001f);
    }

    void testAdjustMasterClampsAtOne() {
        rpg::AudioManager am;
        am.set_master_volume(1.0f);
        am.apply_settings(true);
        am.set_master_volume(std::clamp(am.get_master_volume() + rpg::AudioManager::VOLUME_STEP * 1, 0.0f, 1.0f));
        am.apply_settings(true);
        TS_ASSERT_DELTA(am.get_master_volume(), 1.0f, 0.001f);
    }

    void testAdjustMusicVolume() {
        rpg::AudioManager am;
        const float before = am.get_music_volume();
        am.set_music_volume(std::clamp(before + rpg::AudioManager::VOLUME_STEP, 0.0f, 1.0f));
        am.apply_settings(true);
        TS_ASSERT_DELTA(am.get_music_volume(), std::clamp(before + rpg::AudioManager::VOLUME_STEP, 0.0f, 1.0f), 0.001f);
    }

    void testAdjustSfxVolume() {
        rpg::AudioManager am;
        const float before = am.get_sfx_volume();
        am.set_sfx_volume(std::clamp(before - rpg::AudioManager::VOLUME_STEP, 0.0f, 1.0f));
        am.apply_settings(true);
        TS_ASSERT_DELTA(am.get_sfx_volume(), before - rpg::AudioManager::VOLUME_STEP, 0.001f);
    }

    void testQueueAndFlush() {
        rpg::AudioManager am;
        am.queue("a");
        am.queue("b");
        TS_ASSERT_EQUALS(am.pending_sfx.size(), 2U);
        am.flush(true);
        TS_ASSERT(am.pending_sfx.empty());
    }

    void testResetDefaults() {
        rpg::AudioManager am;
        am.set_master_volume(0.3f);
        am.set_music_volume(0.1f);
        am.set_sfx_volume(0.2f);
        am.queue("sfx/click.wav");

        am.reset_defaults();

        TS_ASSERT_DELTA(am.get_master_volume(), DEFAULT_MASTER_VOLUME, 0.001f);
        TS_ASSERT_DELTA(am.get_music_volume(), DEFAULT_MUSIC_VOLUME, 0.001f);
        TS_ASSERT_DELTA(am.get_sfx_volume(), DEFAULT_MASTER_VOLUME, 0.001f);
        TS_ASSERT(am.pending_sfx.empty());
        TS_ASSERT(!am.get_music_volume_changed());
    }

    void testApplySettingsClampsVolumes() {
        rpg::AudioManager am;
        am.set_master_volume(5.0f);
        am.set_music_volume(-1.0f);
        am.set_sfx_volume(999.0f);

        am.apply_settings(true);

        TS_ASSERT_DELTA(am.get_master_volume(), 1.0f, 0.001f);
        TS_ASSERT_DELTA(am.get_music_volume(), 0.0f, 0.001f);
        TS_ASSERT_DELTA(am.get_sfx_volume(), 1.0f, 0.001f);
    }
};
