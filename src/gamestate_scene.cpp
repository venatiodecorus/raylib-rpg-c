#include "gamestate.h"
#include "ecs_core_components.h"
#include "ecs_actor_components.h"
#include <entt/entt.hpp>

/** @file gamestate_scene.cpp
 *  @brief Scene-specific input and character-creation helpers implemented on `gamestate`.
 */


void gamestate::handle_input_title_scene(inputstate& is) {
    if (inputstate_is_pressed(is, KEY_ESCAPE)) {
        do_quit = true;
        return;
    }
    if (inputstate_is_pressed(is, KEY_ENTER) || inputstate_is_pressed(is, KEY_SPACE)) {
        current_scene = scene_t::MAIN_MENU;
        frame_dirty = true;
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
    }
}

void gamestate::handle_input_main_menu_scene(inputstate& is) {
    if (inputstate_is_pressed(is, KEY_ENTER) || inputstate_is_pressed(is, KEY_SPACE)) {
        if (ui.title_screen_selection == 0) {
            current_scene = scene_t::CHARACTER_CREATION;
            frame_dirty = true;
        }
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
    }
    else if (inputstate_is_pressed(is, KEY_DOWN)) {
        ui.title_screen_selection++;
        if (ui.title_screen_selection >= ui.max_title_screen_selections) {
            ui.title_screen_selection = 0;
        }
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
    }
    else if (inputstate_is_pressed(is, KEY_UP)) {
        ui.title_screen_selection--;
        if (ui.title_screen_selection < 0) {
            ui.title_screen_selection = ui.max_title_screen_selections - 1;
        }
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
    }
    else if (inputstate_is_pressed(is, KEY_ESCAPE)) {
        do_quit = true;
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
    }
    frame_dirty = true;
}

void gamestate::make_all_npcs_target_player() {
    massert(hero_id != ENTITYID_INVALID, "hero_id is invalid");
    auto view = registry.view<ActorKind, LegacyEntityId>();
    for (auto entity : view) {
        entityid id = view.get<LegacyEntityId>(entity).id;
        entitytype_t t = ct.get<entitytype>(id).value_or(entitytype_t::NONE);
        if (t != entitytype_t::NPC) {
            continue;
        }
        ct.set<target_id>(id, hero_id);
    }
}

bool gamestate::try_append_character_creation_char(int codepoint) {
    constexpr size_t max_name_len = 16;
    if (codepoint < 33 || codepoint > 126) {
        return false;
    }
    if (chara_creation.name.size() >= max_name_len) {
        return false;
    }
    chara_creation.name.push_back(static_cast<char>(codepoint));
    return true;
}

bool gamestate::backspace_character_creation_name() {
    if (chara_creation.name.empty()) {
        return false;
    }
    chara_creation.name.pop_back();
    return true;
}

bool gamestate::handle_character_creation_text_input(inputstate& is) {
    bool changed = false;
    if (inputstate_is_pressed(is, KEY_BACKSPACE)) {
        changed = backspace_character_creation_name() || changed;
    }
    int codepoint = GetCharPressed();
    while (codepoint > 0) {
        changed = try_append_character_creation_char(codepoint) || changed;
        codepoint = GetCharPressed();
    }
    return changed;
}

void gamestate::handle_input_character_creation_scene(inputstate& is) {
    if (inputstate_is_pressed(is, KEY_ESCAPE)) {
        do_quit = true;
        return;
    }
    bool changed = handle_character_creation_text_input(is);
    if (inputstate_is_pressed(is, KEY_ENTER)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        int myhd = chara_creation.hitdie;
        int maxhp_roll = -1;
        while (maxhp_roll < 1) {
            maxhp_roll = do_roll_best_of_3(vec3{1, myhd, 0}) + get_stat_bonus(chara_creation.constitution);
        }
        shared_ptr<dungeon_floor> df = d.floors[0];
        vec3 start_loc = df->get_random_loc();
        massert(!vec3_invalid(start_loc), "start_loc is (-1,-1,-1) - no valid start location exists");
        const string player_name = chara_creation.name.empty() ? "hero" : chara_creation.name;
        entity_turn = create_player_at_with(start_loc, player_name, player_init(maxhp_roll));
        massert(hero_id != ENTITYID_INVALID, "heroid is invalid");
        make_all_npcs_target_player();
        current_scene = scene_t::GAMEPLAY;
        dirty_entities = true;
        new_entityid_begin = 0;
        new_entityid_end = next_entityid;
        if (!keyboard_profile_confirmed) {
            open_keyboard_profile_prompt();
        }
        changed = true;
    }
    else if (inputstate_is_pressed(is, KEY_SPACE)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        chara_creation.strength = do_roll_best_of_3(vec3{3, 6, 0});
        chara_creation.dexterity = do_roll_best_of_3(vec3{3, 6, 0});
        chara_creation.intelligence = do_roll_best_of_3(vec3{3, 6, 0});
        chara_creation.wisdom = do_roll_best_of_3(vec3{3, 6, 0});
        chara_creation.constitution = do_roll_best_of_3(vec3{3, 6, 0});
        chara_creation.charisma = do_roll_best_of_3(vec3{3, 6, 0});
        changed = true;
    }
    else if (inputstate_is_pressed(is, KEY_LEFT)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        int race = static_cast<int>(chara_creation.race);
        if (race > 1) {
            race--;
        }
        else {
            race = static_cast<int>(race_t::COUNT) - 1;
        }
        chara_creation.race = static_cast<race_t>(race);
        chara_creation.hitdie = get_racial_hd(chara_creation.race);
        changed = true;
    }
    else if (inputstate_is_pressed(is, KEY_RIGHT)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        int race = static_cast<int>(chara_creation.race);
        if (race < static_cast<int>(race_t::COUNT) - 1) {
            race++;
        }
        else {
            race = static_cast<int>(race_t::NONE) + 1;
        }
        chara_creation.race = static_cast<race_t>(race);
        chara_creation.hitdie = get_racial_hd(chara_creation.race);
        changed = true;
    }
    else if (inputstate_is_pressed(is, KEY_UP)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        chara_creation.alignment = alignment_prev(chara_creation.alignment);
        changed = true;
    }
    else if (inputstate_is_pressed(is, KEY_DOWN)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        chara_creation.alignment = alignment_next(chara_creation.alignment);
        changed = true;
    }
    if (changed) {
        frame_dirty = true;
    }
}
