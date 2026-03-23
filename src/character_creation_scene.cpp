/** @file character_creation_scene.cpp
 *  @brief Character creation scene implementation.
 */

#include "character_creation_scene.h"
#include "ecs_actor_components.h"
#include "ecs_core_components.h"
#include "gamestate.h"
#include <entt/entt.hpp>

static bool try_append_character_creation_char(gamestate& g, int codepoint) {
    constexpr size_t max_name_len = 16;
    if (codepoint < 33 || codepoint > 126) {
        return false;
    }
    if (g.chara_creation.name.size() >= max_name_len) {
        return false;
    }
    g.chara_creation.name.push_back(static_cast<char>(codepoint));
    return true;
}

static bool backspace_character_creation_name(gamestate& g) {
    if (g.chara_creation.name.empty()) {
        return false;
    }
    g.chara_creation.name.pop_back();
    return true;
}

static bool handle_character_creation_text_input(gamestate& g, inputstate& is) {
    bool changed = false;
    if (inputstate_is_pressed(is, KEY_BACKSPACE)) {
        changed = backspace_character_creation_name(g) || changed;
    }
    int codepoint = GetCharPressed();
    while (codepoint > 0) {
        changed = try_append_character_creation_char(g, codepoint) || changed;
        codepoint = GetCharPressed();
    }
    return changed;
}

void CharacterCreationScene::handle_input(gamestate& g, inputstate& is) {
    if (inputstate_is_pressed(is, KEY_ESCAPE)) {
        g.do_quit = true;
        return;
    }
    bool changed = handle_character_creation_text_input(g, is);
    if (inputstate_is_pressed(is, KEY_ENTER)) {
        g.audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        int myhd = g.chara_creation.hitdie;
        int maxhp_roll = -1;
        while (maxhp_roll < 1) {
            maxhp_roll = do_roll_best_of_3(vec3{1, myhd, 0}) + get_stat_bonus(g.chara_creation.constitution);
        }
        shared_ptr<dungeon_floor> df = g.d.floors[0];
        vec3 start_loc = vec3{2, 2, 0};
        massert(!vec3_invalid(start_loc), "start_loc is (-1,-1,-1) - no valid start location exists");
        const string player_name = g.chara_creation.name.empty() ? "hero" : g.chara_creation.name;
        g.entity_turn = g.create_player_at_with(start_loc, player_name, g.player_init(maxhp_roll));
        massert(g.hero_id != ENTITYID_INVALID, "heroid is invalid");
        g.make_all_npcs_target_player();
        g.current_scene = scene_t::GAMEPLAY;
        g.dirty_entities = true;
        g.new_entityid_begin = 0;
        g.new_entityid_end = g.next_entityid;
        if (!g.keyboard_profile_confirmed) {
            g.open_keyboard_profile_prompt();
        }
        changed = true;
    }
    else if (inputstate_is_pressed(is, KEY_SPACE)) {
        g.audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        g.chara_creation.strength = do_roll_best_of_3(vec3{3, 6, 0});
        g.chara_creation.dexterity = do_roll_best_of_3(vec3{3, 6, 0});
        g.chara_creation.intelligence = do_roll_best_of_3(vec3{3, 6, 0});
        g.chara_creation.wisdom = do_roll_best_of_3(vec3{3, 6, 0});
        g.chara_creation.constitution = do_roll_best_of_3(vec3{3, 6, 0});
        g.chara_creation.charisma = do_roll_best_of_3(vec3{3, 6, 0});
        changed = true;
    }
    else if (inputstate_is_pressed(is, KEY_LEFT)) {
        g.audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        int race = static_cast<int>(g.chara_creation.race);
        if (race > 1) {
            race--;
        }
        else {
            race = static_cast<int>(race_t::COUNT) - 1;
        }
        g.chara_creation.race = static_cast<race_t>(race);
        g.chara_creation.hitdie = get_racial_hd(g.chara_creation.race);
        changed = true;
    }
    else if (inputstate_is_pressed(is, KEY_RIGHT)) {
        g.audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        int race = static_cast<int>(g.chara_creation.race);
        if (race < static_cast<int>(race_t::COUNT) - 1) {
            race++;
        }
        else {
            race = static_cast<int>(race_t::NONE) + 1;
        }
        g.chara_creation.race = static_cast<race_t>(race);
        g.chara_creation.hitdie = get_racial_hd(g.chara_creation.race);
        changed = true;
    }
    else if (inputstate_is_pressed(is, KEY_UP)) {
        g.audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        g.chara_creation.alignment = alignment_prev(g.chara_creation.alignment);
        changed = true;
    }
    else if (inputstate_is_pressed(is, KEY_DOWN)) {
        g.audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        g.chara_creation.alignment = alignment_next(g.chara_creation.alignment);
        changed = true;
    }
    if (changed) {
        g.frame_dirty = true;
    }
}
