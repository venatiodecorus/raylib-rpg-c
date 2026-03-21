#pragma once

#include "../gamestate.h"
#include <cxxtest/TestSuite.h>

class GamestateLifecycleTestSuite : public CxxTest::TestSuite {
public:
    void press_key(inputstate& is, int key) {
        const int idx = key / BITS_PER_LONG;
        const int bit = key % BITS_PER_LONG;
        is.pressed[idx] |= (1ULL << bit);
    }

    void hold_key(inputstate& is, int key) {
        const int idx = key / BITS_PER_LONG;
        const int bit = key % BITS_PER_LONG;
        is.held[idx] |= (1ULL << bit);
    }

    void add_floor(gamestate& g, int width = 8, int height = 8) {
        auto df = g.d.create_floor(biome_t::STONE, width, height);
        df->df_set_all_tiles(tiletype_t::FLOOR_STONE_00);
        g.d.add_floor(df);
        g.d.is_initialized = true;
    }

    void testGamestateEmpty() {
    }

    void testGamestateDefaults() {
        gamestate g;

        TS_ASSERT_EQUALS(g.session.version, std::string(GAME_VERSION));
        TS_ASSERT_EQUALS(g.hero_id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.entity_turn, 1);
        TS_ASSERT_EQUALS(g.next_entityid, 1);
        TS_ASSERT_EQUALS(g.new_entityid_begin, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.new_entityid_end, ENTITYID_INVALID);
        TS_ASSERT(g.presentation.cam_lockon);
        TS_ASSERT(!g.presentation.cam_changed);
        TS_ASSERT(!g.dirty_entities);
        TS_ASSERT(!g.messages.is_active);
        TS_ASSERT_EQUALS(g.current_scene, scene_t::TITLE);
        TS_ASSERT_EQUALS(g.controlmode, controlmode_t::PLAYER);
        TS_ASSERT_EQUALS(g.ui.confirm_action, confirm_action_t::NONE);
        TS_ASSERT_DELTA(g.audio.get_music_volume(), DEFAULT_MUSIC_VOLUME, 0.001f);
        TS_ASSERT_DELTA(g.audio.get_sfx_volume(), DEFAULT_MASTER_VOLUME, 0.001f);
        TS_ASSERT_EQUALS(g.ui.window_box_bgcolor.r, 0);
        TS_ASSERT_EQUALS(g.ui.window_box_bgcolor.g, 0);
        TS_ASSERT_EQUALS(g.ui.window_box_bgcolor.b, 255);
        TS_ASSERT_EQUALS(g.ui.window_box_bgcolor.a, 128);
        TS_ASSERT_EQUALS(g.get_debug_panel_bgcolor().a, 255);
        TS_ASSERT_EQUALS(g.chara_creation.race, race_t::HUMAN);
        TS_ASSERT_EQUALS(g.chara_creation.alignment, alignment_t::NEUTRAL_NEUTRAL);
        TS_ASSERT_DELTA(g.presentation.cam2d.zoom, DEFAULT_ZOOM_LEVEL, 0.001f);
        TS_ASSERT_DELTA(g.presentation.cam2d.rotation, 0.0f, 0.001f);
        TS_ASSERT_EQUALS(g.presentation.cam2d.target.x, 0.0f);
        TS_ASSERT_EQUALS(g.presentation.cam2d.target.y, 0.0f);
        TS_ASSERT_EQUALS(g.presentation.cam2d.offset.x, 0.0f);
        TS_ASSERT_EQUALS(g.presentation.cam2d.offset.y, 0.0f);
        TS_ASSERT_EQUALS(g.messages.system.size(), 0U);
        TS_ASSERT_EQUALS(g.messages.history.size(), 0U);
        TS_ASSERT_EQUALS(g.d.floors.size(), 0U);
        TS_ASSERT(!g.d.is_initialized);
    }

    void testGamestateAddEntityTracksDirtyRange() {
        gamestate g;

        const entityid first = g.add_entity();
        const entityid second = g.add_entity();
        const entityid third = g.add_entity();

        TS_ASSERT_EQUALS(first, 1);
        TS_ASSERT_EQUALS(second, 2);
        TS_ASSERT_EQUALS(third, 3);
        TS_ASSERT_EQUALS(g.next_entityid, 4);
        TS_ASSERT(g.dirty_entities);
        TS_ASSERT_EQUALS(g.new_entityid_begin, 1);
        TS_ASSERT_EQUALS(g.new_entityid_end, 4);
    }

    void testGamestateSetHeroIdAndReset() {
        gamestate g;
        const entityid hero = g.add_entity();

        TS_ASSERT(g.set_hero_id(hero));
        TS_ASSERT_EQUALS(g.hero_id, hero);

        g.reset();

        TS_ASSERT_EQUALS(g.hero_id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.next_entityid, 1);
        TS_ASSERT_EQUALS(g.new_entityid_begin, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.new_entityid_end, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.chara_creation.alignment, alignment_t::NEUTRAL_NEUTRAL);
        TS_ASSERT(!g.dirty_entities);
        TS_ASSERT_EQUALS(g.d.floors.size(), 0U);
        TS_ASSERT_EQUALS(g.messages.system.size(), 0U);
        TS_ASSERT_EQUALS(g.messages.history.size(), 0U);
    }

    void testGamestateMessageQueuesAndReset() {
        gamestate g;

        TS_ASSERT(g.messages.add("hello %d", 1));
        g.messages.add_history("history %s", "entry");

        TS_ASSERT(g.messages.is_active);
        TS_ASSERT_EQUALS(g.messages.system.size(), 1U);
        TS_ASSERT_EQUALS(g.messages.system.front(), "hello 1");
        TS_ASSERT_EQUALS(g.messages.history.size(), 1U);
        TS_ASSERT_EQUALS(g.messages.history.front(), "history entry");

        g.reset();

        TS_ASSERT(!g.messages.is_active);
        TS_ASSERT_EQUALS(g.messages.system.size(), 0U);
        TS_ASSERT_EQUALS(g.messages.history.size(), 0U);
    }

    void testGamestateResetRestoresCameraAndUiState() {
        gamestate g;

        g.presentation.cam_lockon = false;
        g.presentation.cam_changed = true;
        g.ui.display_inventory_menu = true;
        g.ui.display_action_menu = true;
        g.ui.display_help_menu = true;
        g.ui.display_confirm_prompt = true;
        g.do_quit = true;
        g.ui.confirm_action = confirm_action_t::QUIT;
        g.ui.confirm_prompt_message = "quit?";
        g.current_scene = scene_t::GAMEPLAY;
        g.audio.set_music_volume(0.25f);
        g.presentation.cam2d.target = Vector2{12.0f, 34.0f};
        g.presentation.cam2d.offset = Vector2{56.0f, 78.0f};
        g.presentation.cam2d.zoom = 9.0f;
        g.presentation.cam2d.rotation = 45.0f;

        g.reset();

        TS_ASSERT(g.presentation.cam_lockon);
        TS_ASSERT(!g.presentation.cam_changed);
        TS_ASSERT(!g.ui.display_inventory_menu);
        TS_ASSERT(!g.ui.display_action_menu);
        TS_ASSERT(!g.ui.display_help_menu);
        TS_ASSERT(!g.ui.display_confirm_prompt);
        TS_ASSERT(!g.do_quit);
        TS_ASSERT_EQUALS(g.ui.confirm_action, confirm_action_t::NONE);
        TS_ASSERT_EQUALS(g.ui.confirm_prompt_message, "");
        TS_ASSERT_EQUALS(g.current_scene, scene_t::TITLE);
        TS_ASSERT_DELTA(g.audio.get_music_volume(), DEFAULT_MUSIC_VOLUME, 0.001f);
        TS_ASSERT_DELTA(g.presentation.cam2d.target.x, 0.0f, 0.001f);
        TS_ASSERT_DELTA(g.presentation.cam2d.target.y, 0.0f, 0.001f);
        TS_ASSERT_DELTA(g.presentation.cam2d.offset.x, 0.0f, 0.001f);
        TS_ASSERT_DELTA(g.presentation.cam2d.offset.y, 0.0f, 0.001f);
        TS_ASSERT_DELTA(g.presentation.cam2d.zoom, DEFAULT_ZOOM_LEVEL, 0.001f);
        TS_ASSERT_DELTA(g.presentation.cam2d.rotation, 0.0f, 0.001f);
    }

    void testGamestateMusicDefaultsAndResetBehavior() {
        gamestate g;

        TS_ASSERT_DELTA(g.audio.get_music_volume(), DEFAULT_MUSIC_VOLUME, 0.001f);
        TS_ASSERT(!g.audio.get_music_volume_changed());
        TS_ASSERT_EQUALS(g.audio.get_current_music_index(), 0U);

        g.audio.set_music_volume(0.25f);
        g.audio.set_music_volume_changed(true);
        g.audio.set_current_music_index(9);

        g.reset();

        TS_ASSERT_DELTA(g.audio.get_music_volume(), DEFAULT_MUSIC_VOLUME, 0.001f);
        TS_ASSERT(!g.audio.get_music_volume_changed());
        TS_ASSERT_EQUALS(g.audio.get_current_music_index(), 0U);
    }

    void testRestartGameReinitializesGameplayWithoutLeavingTitleScene() {
        gamestate g;
        g.test = true;
        g.random.mt.seed(12345);
        g.logic_init();
        g.presentation.targetwidth = DEFAULT_TARGET_WIDTH;
        g.presentation.targetheight = DEFAULT_TARGET_HEIGHT;
        g.presentation.windowwidth = 1234;
        g.presentation.windowheight = 777;

        g.current_scene = scene_t::GAMEPLAY;
        g.do_restart = true;
        g.do_quit = true;
        g.gameover = true;
        g.session.restart_count = 7;
        g.messages.add("stale");

        g.restart_game();

        TS_ASSERT(g.d.is_initialized);
        TS_ASSERT_EQUALS(g.d.get_floor_count(), 4U);
        TS_ASSERT_EQUALS(g.current_scene, scene_t::TITLE);
        TS_ASSERT_EQUALS(g.session.restart_count, 8U);
        TS_ASSERT(!g.do_restart);
        TS_ASSERT(!g.do_quit);
        TS_ASSERT(!g.gameover);
        TS_ASSERT(g.frame_dirty);
        TS_ASSERT(g.messages.system.size() >= 2U);
        TS_ASSERT_EQUALS(g.messages.system.front(), "Welcome to the game! Press enter to cycle messages.");
        TS_ASSERT_EQUALS(g.presentation.targetwidth, DEFAULT_TARGET_WIDTH);
        TS_ASSERT_EQUALS(g.presentation.targetheight, DEFAULT_TARGET_HEIGHT);
        TS_ASSERT_EQUALS(g.presentation.windowwidth, 1234);
        TS_ASSERT_EQUALS(g.presentation.windowheight, 777);
        TS_ASSERT_DELTA(g.presentation.cam2d.offset.x, DEFAULT_TARGET_WIDTH / 4.0f, 0.001f);
        TS_ASSERT_DELTA(g.presentation.cam2d.offset.y, DEFAULT_TARGET_HEIGHT / 4.0f, 0.001f);
    }

    void testHeroOpensLevelUpModalAtTenXpAndCanApplySelection() {
        gamestate g;
        g.test = true;
        g.current_scene = scene_t::GAMEPLAY;

        auto df = g.d.create_floor(biome_t::STONE, 8, 8);
        df->df_set_all_tiles(tiletype_t::FLOOR_STONE_00);
        g.d.add_floor(df);
        g.d.is_initialized = true;

        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(10));
        const entityid slime = g.create_npc_at_with(race_t::GREEN_SLIME, vec3{2, 1, 0}, [](gamestate&, const entityid) {});
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(slime, ENTITYID_INVALID);

        g.ct.set<xp>(hero, 9);
        const int old_str = g.ct.get_or<strength>(hero, 0);
        const vec2 old_hp = g.ct.get_or<hp>(hero, vec2{0, 0});

        g.update_npc_xp(hero, slime);

        TS_ASSERT_EQUALS(g.ct.get_or<xp>(hero, 0), 10);
        TS_ASSERT(g.ui.display_level_up_modal);
        TS_ASSERT_EQUALS(g.controlmode, controlmode_t::LEVEL_UP);

        inputstate is;
        inputstate_reset(is);
        press_key(is, KEY_ENTER);
        g.handle_input(is);

        TS_ASSERT(!g.ui.display_level_up_modal);
        TS_ASSERT_EQUALS(g.controlmode, controlmode_t::PLAYER);
        TS_ASSERT_EQUALS(g.ct.get_or<level>(hero, 0), 2);
        TS_ASSERT_EQUALS(g.ct.get_or<strength>(hero, 0), old_str + 1);
        const vec2 new_hp = g.ct.get_or<hp>(hero, vec2{0, 0});
        TS_ASSERT_EQUALS(new_hp.x, old_hp.x);
        TS_ASSERT(new_hp.y > old_hp.y);
    }

    void testApplyLevelUpRewardsAddsHitDieWorthOfMaxHp() {
        gamestate g;
        auto df = g.d.create_floor(biome_t::STONE, 8, 8);
        df->df_set_all_tiles(tiletype_t::FLOOR_STONE_00);
        g.d.add_floor(df);
        g.d.is_initialized = true;

        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(10));
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);

        g.ct.set<hd>(hero, vec3{1, 1, 0});
        g.ct.set<hp>(hero, vec2{7, 10});

        g.apply_level_up_rewards(hero);

        TS_ASSERT_EQUALS(g.ct.get_or<level>(hero, 0), 2);
        TS_ASSERT(vec2_equal(g.ct.get_or<hp>(hero, vec2{0, 0}), vec2{7, 11}));
    }

    void testConfirmPromptResolvesYesInputEvenIfControlmodeDrifted() {
        gamestate g;
        g.test = true;
        inputstate is;
        inputstate_reset(is);

        g.current_scene = scene_t::GAMEPLAY;
        g.open_confirm_prompt(confirm_action_t::QUIT, "quit?");
        g.controlmode = controlmode_t::PLAYER;
        press_key(is, KEY_Y);

        g.handle_input(is);

        TS_ASSERT(!g.ui.display_confirm_prompt);
        TS_ASSERT_EQUALS(g.ui.confirm_action, confirm_action_t::NONE);
        TS_ASSERT(g.do_quit);
    }

    void testConfirmPromptResolvesNoInput() {
        gamestate g;
        g.test = true;
        inputstate is;
        inputstate_reset(is);

        g.current_scene = scene_t::GAMEPLAY;
        g.open_confirm_prompt(confirm_action_t::QUIT, "quit?");
        press_key(is, KEY_N);

        g.handle_input(is);

        TS_ASSERT(!g.ui.display_confirm_prompt);
        TS_ASSERT_EQUALS(g.ui.confirm_action, confirm_action_t::NONE);
        TS_ASSERT(!g.do_quit);
        TS_ASSERT_EQUALS(g.controlmode, controlmode_t::PLAYER);
    }

    void testCharacterCreationNameHelpersAppendAndBackspace() {
        gamestate g;

        g.chara_creation.name.clear();
        TS_ASSERT(g.try_append_character_creation_char('A'));
        TS_ASSERT(g.try_append_character_creation_char('b'));
        TS_ASSERT(g.try_append_character_creation_char('3'));
        TS_ASSERT_EQUALS(g.chara_creation.name, "Ab3");
        TS_ASSERT(g.backspace_character_creation_name());
        TS_ASSERT_EQUALS(g.chara_creation.name, "Ab");
    }

    void testCharacterCreationNameHelpersRejectControlCharsAndLengthOverflow() {
        gamestate g;

        g.chara_creation.name.clear();
        TS_ASSERT(!g.try_append_character_creation_char('\n'));
        TS_ASSERT(!g.try_append_character_creation_char(' '));
        TS_ASSERT_EQUALS(g.chara_creation.name, "");
        for (int i = 0; i < 16; i++) {
            TS_ASSERT(g.try_append_character_creation_char('a'));
        }
        TS_ASSERT_EQUALS(g.chara_creation.name.size(), 16U);
        TS_ASSERT(!g.try_append_character_creation_char('b'));
        TS_ASSERT_EQUALS(g.chara_creation.name, "aaaaaaaaaaaaaaaa");
    }

    void testCharacterCreationTransitionsIntoKeyboardProfilePrompt() {
        gamestate g;
        g.test = true;
        g.audio.sfx.resize(71);
        add_floor(g);
        g.current_scene = scene_t::CHARACTER_CREATION;

        inputstate is = {};
        inputstate_reset(is);
        press_key(is, KEY_ENTER);

        g.handle_input_character_creation_scene(is);

        TS_ASSERT_EQUALS(g.current_scene, scene_t::GAMEPLAY);
        TS_ASSERT(g.ui.display_keyboard_profile_prompt);
        TS_ASSERT_EQUALS(g.controlmode, controlmode_t::KEYBOARD_PROFILE);
        TS_ASSERT_DIFFERS(g.hero_id, ENTITYID_INVALID);
    }

    void testKeyboardProfilePromptCanSelectLaptopProfile() {
        gamestate g;
        g.test = true;
        g.current_scene = scene_t::GAMEPLAY;
        g.open_keyboard_profile_prompt();

        inputstate is = {};
        inputstate_reset(is);
        press_key(is, KEY_RIGHT);
        g.handle_input_keyboard_profile_prompt(is);
        TS_ASSERT_EQUALS(g.ui.keyboard_profile_selection, static_cast<unsigned int>(keyboard_profile_t::LAPTOP));

        inputstate_reset(is);
        press_key(is, KEY_ENTER);
        g.handle_input_keyboard_profile_prompt(is);

        TS_ASSERT_EQUALS(g.keybind.keyboard_profile, keyboard_profile_t::LAPTOP);
        TS_ASSERT(g.keybind.keyboard_profile_confirmed);
        TS_ASSERT(!g.ui.display_keyboard_profile_prompt);
        TS_ASSERT_EQUALS(g.controlmode, controlmode_t::PLAYER);
    }

    void testCharacterCreationSkipsKeyboardPromptAfterProfileAlreadyConfirmed() {
        gamestate g;
        g.test = true;
        g.audio.sfx.resize(71);
        add_floor(g);
        g.current_scene = scene_t::CHARACTER_CREATION;
        g.keybind.keyboard_profile = keyboard_profile_t::LAPTOP;
        g.keybind.keyboard_profile_confirmed = true;

        inputstate is = {};
        inputstate_reset(is);
        press_key(is, KEY_ENTER);

        g.handle_input_character_creation_scene(is);

        TS_ASSERT_EQUALS(g.current_scene, scene_t::GAMEPLAY);
        TS_ASSERT(!g.ui.display_keyboard_profile_prompt);
        TS_ASSERT_EQUALS(g.controlmode, controlmode_t::PLAYER);
        TS_ASSERT_EQUALS(g.keybind.keyboard_profile, keyboard_profile_t::LAPTOP);
        TS_ASSERT_DIFFERS(g.hero_id, ENTITYID_INVALID);
    }

    void testLaptopProfileUsesHjklMovementKeys() {
        gamestate g;
        g.test = true;
        add_floor(g);
        const entityid hero = g.create_player_at_with(vec3{3, 3, 0}, "hero", g.player_init(10));
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);

        g.current_scene = scene_t::GAMEPLAY;
        g.hero_id = hero;
        g.keybind.keyboard_profile = keyboard_profile_t::LAPTOP;

        inputstate is = {};
        inputstate_reset(is);
        press_key(is, KEY_H);

        TS_ASSERT(g.handle_move_left(is, false));
        TS_ASSERT(vec3_equal(g.ct.get_or<location>(hero, vec3{-1, -1, -1}), vec3{2, 3, 0}));
    }

    void testControlsMenuCanRebindAttackKeyForCurrentProfile() {
        gamestate g;
        g.test = true;
        g.current_scene = scene_t::GAMEPLAY;
        g.keybind.keyboard_profile = keyboard_profile_t::LAPTOP;
        g.open_controls_menu();
        g.ui.controls_menu_selection = gameplay_input_action_t::ATTACK + 2;

        inputstate is = {};
        inputstate_reset(is);
        press_key(is, KEY_ENTER);
        g.handle_input_controls_menu(is);
        TS_ASSERT(g.keybind.controls_menu_waiting_for_key);

        inputstate_reset(is);
        press_key(is, KEY_T);
        g.handle_input_controls_menu(is);

        TS_ASSERT(!g.keybind.controls_menu_waiting_for_key);
        TS_ASSERT_EQUALS(g.get_keybinding_primary(keyboard_profile_t::LAPTOP, gameplay_input_action_t::ATTACK), KEY_T);
    }

    void testOptionMenuCanOpenSoundMenuAndAdjustVolumes() {
        gamestate g;
        g.test = true;
        g.current_scene = scene_t::GAMEPLAY;
        g.ui.display_option_menu = true;
        g.controlmode = controlmode_t::OPTION_MENU;

        inputstate is = {};
        inputstate_reset(is);
        press_key(is, KEY_ENTER);
        g.handle_input_option_menu(is);

        TS_ASSERT(g.ui.display_sound_menu);
        TS_ASSERT(!g.ui.display_option_menu);
        TS_ASSERT_EQUALS(g.controlmode, controlmode_t::SOUND_MENU);

        inputstate_reset(is);
        press_key(is, KEY_RIGHT);
        g.handle_input_sound_menu(is);
        TS_ASSERT_DELTA(g.audio.get_master_volume(), DEFAULT_MASTER_VOLUME, 0.001f);

        inputstate_reset(is);
        press_key(is, KEY_DOWN);
        g.handle_input_sound_menu(is);
        inputstate_reset(is);
        press_key(is, KEY_LEFT);
        g.handle_input_sound_menu(is);
        TS_ASSERT_DELTA(g.audio.get_music_volume(), DEFAULT_MUSIC_VOLUME - rpg::AudioManager::VOLUME_STEP, 0.001f);

        inputstate_reset(is);
        press_key(is, KEY_DOWN);
        g.handle_input_sound_menu(is);
        inputstate_reset(is);
        press_key(is, KEY_LEFT);
        g.handle_input_sound_menu(is);
        TS_ASSERT_DELTA(g.audio.get_sfx_volume(), DEFAULT_MASTER_VOLUME - rpg::AudioManager::VOLUME_STEP, 0.001f);

        inputstate_reset(is);
        press_key(is, KEY_ESCAPE);
        g.handle_input_sound_menu(is);
        TS_ASSERT(!g.ui.display_sound_menu);
        TS_ASSERT(g.ui.display_option_menu);
        TS_ASSERT_EQUALS(g.controlmode, controlmode_t::OPTION_MENU);
    }

    void testOptionMenuCanOpenWindowColorMenuAndResetDefaults() {
        gamestate g;
        g.test = true;
        g.current_scene = scene_t::GAMEPLAY;
        g.ui.display_option_menu = true;
        g.controlmode = controlmode_t::OPTION_MENU;
        g.ui.options_menu.incr_selection();

        inputstate is = {};
        inputstate_reset(is);
        press_key(is, KEY_ENTER);
        g.handle_input_option_menu(is);

        TS_ASSERT(g.ui.display_window_color_menu);
        TS_ASSERT_EQUALS(g.controlmode, controlmode_t::WINDOW_COLOR_MENU);

        inputstate_reset(is);
        hold_key(is, KEY_RIGHT);
        g.handle_input_window_color_menu(is);
        TS_ASSERT_EQUALS(g.ui.window_box_bgcolor.r, 1);

        for (int i = 0; i < 8; i++) {
            inputstate_reset(is);
            press_key(is, KEY_DOWN);
            g.handle_input_window_color_menu(is);
        }
        inputstate_reset(is);
        press_key(is, KEY_ENTER);
        g.handle_input_window_color_menu(is);

        TS_ASSERT_EQUALS(g.ui.window_box_bgcolor.r, 0);
        TS_ASSERT_EQUALS(g.ui.window_box_bgcolor.g, 0);
        TS_ASSERT_EQUALS(g.ui.window_box_bgcolor.b, 255);
        TS_ASSERT_EQUALS(g.ui.window_box_bgcolor.a, 128);
        TS_ASSERT_EQUALS(g.ui.window_box_fgcolor.r, 255);
        TS_ASSERT_EQUALS(g.ui.window_box_fgcolor.g, 255);
        TS_ASSERT_EQUALS(g.ui.window_box_fgcolor.b, 255);
        TS_ASSERT_EQUALS(g.ui.window_box_fgcolor.a, 255);
    }

    void testOptionMenuCanToggleMiniInventoryPreference() {
        gamestate g;
        g.test = true;
        g.current_scene = scene_t::GAMEPLAY;
        g.ui.display_option_menu = true;
        g.controlmode = controlmode_t::OPTION_MENU;
        g.ui.options_menu.incr_selection();
        g.ui.options_menu.incr_selection();
        g.ui.options_menu.incr_selection();

        inputstate is = {};
        inputstate_reset(is);
        press_key(is, KEY_ENTER);
        g.handle_input_option_menu(is);

        TS_ASSERT(g.ui.prefer_mini_inventory_menu);
        TS_ASSERT(g.ui.display_option_menu);
        TS_ASSERT_EQUALS(g.controlmode, controlmode_t::OPTION_MENU);
    }

    void testFaceDirectionAttackUsesBoundActionInsteadOfHardCodedKey() {
        gamestate g;
        g.test = true;
        add_floor(g);
        const entityid hero = g.create_player_at_with(vec3{3, 3, 0}, "hero", g.player_init(10));
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);

        g.current_scene = scene_t::GAMEPLAY;
        g.hero_id = hero;
        g.player_changing_dir = true;
        g.keybind.keyboard_profile = keyboard_profile_t::FULL;

        inputstate is = {};
        inputstate_reset(is);
        press_key(is, KEY_APOSTROPHE);
        TS_ASSERT(g.handle_change_dir(is));
        TS_ASSERT(g.ct.get_or<attacking>(hero, false));
        TS_ASSERT(!g.player_changing_dir);

        g.ct.set<attacking>(hero, false);
        g.player_changing_dir = true;
        g.set_keybinding_primary(keyboard_profile_t::FULL, gameplay_input_action_t::FACE_ATTACK, KEY_T);
        inputstate_reset(is);
        press_key(is, KEY_APOSTROPHE);
        TS_ASSERT(g.handle_change_dir(is));
        TS_ASSERT(!g.ct.get_or<attacking>(hero, false));
        TS_ASSERT(g.player_changing_dir);

        inputstate_reset(is);
        press_key(is, KEY_T);
        TS_ASSERT(g.handle_change_dir(is));
        TS_ASSERT(g.ct.get_or<attacking>(hero, false));
        TS_ASSERT(!g.player_changing_dir);
    }

    void testDefaultStairsBindingUsesKeyFForBothProfiles() {
        gamestate g;

        TS_ASSERT_EQUALS(g.get_keybinding_primary(keyboard_profile_t::FULL, gameplay_input_action_t::STAIRS), KEY_F);
        TS_ASSERT_EQUALS(g.get_keybinding_primary(keyboard_profile_t::LAPTOP, gameplay_input_action_t::STAIRS), KEY_F);
    }

    void testLaptopDirectionModeDefaultMovesOffKeyF() {
        gamestate g;

        TS_ASSERT_EQUALS(g.get_keybinding_primary(keyboard_profile_t::LAPTOP, gameplay_input_action_t::DIRECTION_MODE), KEY_G);
        TS_ASSERT_DIFFERS(g.get_keybinding_primary(keyboard_profile_t::LAPTOP, gameplay_input_action_t::DIRECTION_MODE), KEY_F);
    }

    void testRestartGamePreservesConfirmedKeyboardProfileChoice() {
        gamestate g;
        g.test = true;
        g.random.mt.seed(12345);
        g.keybind.keyboard_profile = keyboard_profile_t::LAPTOP;
        g.keybind.keyboard_profile_confirmed = true;

        g.logic_init();
        g.current_scene = scene_t::GAMEPLAY;

        g.restart_game();

        TS_ASSERT_EQUALS(g.current_scene, scene_t::TITLE);
        TS_ASSERT_EQUALS(g.keybind.keyboard_profile, keyboard_profile_t::LAPTOP);
        TS_ASSERT(g.keybind.keyboard_profile_confirmed);
    }

    void testLogicInitAddsFloorFourPressurePlateTutorialSetup() {
        gamestate g;
        g.test = true;
        g.random.mt.seed(12345);

        g.logic_init();

        TS_ASSERT_EQUALS(g.d.get_floor_count(), 4U);
        TS_ASSERT_EQUALS(g.pressure_plate_state.floor_pressure_plates.size(), 2U);
        const entityid linked_door_id = g.pressure_plate_state.floor_pressure_plates.front().linked_door_id;
        TS_ASSERT_DIFFERS(linked_door_id, ENTITYID_INVALID);
        for (const floor_pressure_plate_t& plate : g.pressure_plate_state.floor_pressure_plates) {
            TS_ASSERT_EQUALS(plate.loc.z, 2);
            TS_ASSERT(!plate.destroyed);
            TS_ASSERT_EQUALS(plate.txkey_up, TX_SWITCHES_PRESSURE_PLATE_UP_00);
            TS_ASSERT_EQUALS(plate.txkey_down, TX_SWITCHES_PRESSURE_PLATE_DOWN_00);
            TS_ASSERT_EQUALS(plate.linked_door_id, linked_door_id);
        }
        TS_ASSERT_EQUALS(g.ct.get_or<entitytype>(linked_door_id, entitytype_t::NONE), entitytype_t::DOOR);
        const vec3 tutorial_door_loc = g.ct.get_or<location>(linked_door_id, vec3{-1, -1, -1});
        TS_ASSERT(vec3_valid(tutorial_door_loc));
        TS_ASSERT_EQUALS(tutorial_door_loc.z, 2);
        TS_ASSERT(vec3_valid(g.pressure_plate_state.floor_four_tutorial_orc_spawn));
        TS_ASSERT_EQUALS(g.pressure_plate_state.floor_four_tutorial_orc_spawn.z, 2);

        auto floor_four = g.d.get_floor(2);
        TS_ASSERT_EQUALS(floor_four->tile_at(vec3{tutorial_door_loc.x, tutorial_door_loc.y - 1, tutorial_door_loc.z}).get_type(), tiletype_t::STONE_WALL_00);
        const vec3 tutorial_downstairs = floor_four->get_downstairs_loc();
        TS_ASSERT(vec3_valid(tutorial_downstairs));
        TS_ASSERT_EQUALS(tutorial_downstairs.z, 2);
        TS_ASSERT(tutorial_downstairs.x > 6);
        TS_ASSERT_EQUALS(floor_four->tile_at(tutorial_downstairs).get_type(), tiletype_t::DOWNSTAIRS);
        TS_ASSERT(tile_is_walkable(floor_four->tile_at(g.pressure_plate_state.floor_four_tutorial_orc_spawn).get_type()));

        size_t floor_four_orc_count = 0;
        bool found_orc_in_tutorial_room = false;
        g.for_entities_of_type(entitytype_t::NPC, [&](entityid id) {
            if (g.ct.get_or<race>(id, race_t::NONE) != race_t::ORC) {
                return;
            }
            const vec3 loc = g.ct.get_or<location>(id, vec3{-1, -1, -1});
            if (loc.z != 2) {
                return;
            }
            floor_four_orc_count++;
            found_orc_in_tutorial_room = vec3_equal(loc, g.pressure_plate_state.floor_four_tutorial_orc_spawn);
        });

        TS_ASSERT_EQUALS(floor_four_orc_count, 1U);
        TS_ASSERT(found_orc_in_tutorial_room);
    }

    void testLogicInitAddsFloorThreePullablesTutorialSign() {
        gamestate g;
        g.test = true;
        g.random.mt.seed(12345);

        g.logic_init();

        size_t floor_three_sign_count = 0;
        g.for_entities_of_type(entitytype_t::PROP, [&](entityid id) {
            if (g.ct.get_or<proptype>(id, proptype_t::NONE) != proptype_t::DUNGEON_WOODEN_SIGN) {
                return;
            }

            const vec3 loc = g.ct.get_or<location>(id, vec3{-1, -1, -1});
            TS_ASSERT(vec3_valid(loc));
            TS_ASSERT_EQUALS(loc.z, 2);
            TS_ASSERT_EQUALS(g.ct.get_or<name>(id, ""), "wooden sign");
            TS_ASSERT_EQUALS(g.ct.get_or<description>(id, ""), "Pull something onto the\npressure plate to keep the door open.");
            TS_ASSERT(g.ct.get_or<solid>(id, false));
            TS_ASSERT(!g.ct.get_or<pushable>(id, true));
            TS_ASSERT(!g.ct.get_or<pullable>(id, true));
            floor_three_sign_count++;
        });

        TS_ASSERT_EQUALS(floor_three_sign_count, 1U);
    }

    void testLogicInitAddsFloorThreePullableTutorialProps() {
        gamestate g;
        g.test = true;
        g.random.mt.seed(12345);

        g.logic_init();

        size_t floor_three_pullable_prop_count = 0;
        g.for_entities_of_type(entitytype_t::PROP, [&](entityid id) {
            if (g.ct.get_or<proptype>(id, proptype_t::NONE) != proptype_t::DUNGEON_CANDLE_00) {
                return;
            }

            const vec3 loc = g.ct.get_or<location>(id, vec3{-1, -1, -1});
            if (loc.z != 2) {
                return;
            }

            TS_ASSERT(vec3_valid(loc));
            TS_ASSERT(g.ct.get_or<pullable>(id, false));
            TS_ASSERT(!g.ct.get_or<solid>(id, true));
            floor_three_pullable_prop_count++;
        });

        TS_ASSERT_EQUALS(floor_three_pullable_prop_count, 4U);
    }

    void testLogicInitAddsPlaceholderFourthFloorWithPropsAndBoxes() {
        gamestate g;
        g.test = true;
        g.random.mt.seed(12345);

        g.logic_init();

        TS_ASSERT_EQUALS(g.d.get_floor_count(), 4U);
        auto floor = g.d.get_floor(3);
        TS_ASSERT_EQUALS(floor->get_width(), 16);
        TS_ASSERT_EQUALS(floor->get_height(), 16);
        TS_ASSERT(vec3_valid(floor->get_upstairs_loc()));

        size_t floor_four_box_count = 0;
        size_t floor_four_prop_count = 0;
        g.for_entities_of_type(entitytype_t::BOX, [&](entityid id) {
            const vec3 loc = g.ct.get_or<location>(id, vec3{-1, -1, -1});
            if (!vec3_valid(loc) || loc.z != 3) {
                return;
            }
            floor_four_box_count++;
        });
        g.for_entities_of_type(entitytype_t::PROP, [&](entityid id) {
            const vec3 loc = g.ct.get_or<location>(id, vec3{-1, -1, -1});
            if (!vec3_valid(loc) || loc.z != 3) {
                return;
            }
            floor_four_prop_count++;
        });

        TS_ASSERT(floor_four_box_count >= 3U);
        TS_ASSERT(floor_four_prop_count >= 1U);
    }
};
