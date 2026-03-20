#include "gamestate.h"
#include "ecs_core_components.h"
#include "ecs_actor_components.h"
#include <entt/entt.hpp>

/** @file gamestate_lifecycle.cpp
 *  @brief Core gameplay lifecycle, visibility, and tick helpers implemented on `gamestate`.
 */


void gamestate::update_tile(tile_t& tile) {
    tile.set_explored(true);
    tile.set_visible(true);
}

bool gamestate::path_blocked(vec3 a, vec3 b) {
    vector<vec3> path = calculate_path_with_thickness(a, b);
    auto df = d.get_current_floor();
    for (const auto& loc : path) {
        tile_t& t = df->tile_at(loc);
        if (tiletype_is_none(t.get_type())) {
            return true;
        }
        else if (tiletype_is_wall(t.get_type())) {
            return true;
        }
        entityid door_id = t.get_cached_door();
        if (door_id != INVALID) {
            bool door_is_open = ct.get_or<door_open>(door_id, false);
            if (!door_is_open) {
                return true;
            }
        }
    }
    return false;
}

bool gamestate::visibility_path_blocked(vec3 a, vec3 b) {
    vector<vec3> path = calculate_path_with_thickness(a, b);
    auto df = d.get_current_floor();
    for (const auto& loc : path) {
        tile_t& t = df->tile_at(loc);
        if (tiletype_is_none(t.get_type())) {
            return true;
        }
        if (tiletype_is_wall(t.get_type())) {
            return true;
        }
        entityid door_id = t.get_cached_door();
        if (door_id != INVALID && !vec3_equal(loc, b)) {
            bool door_is_open = ct.get_or<door_open>(door_id, false);
            if (!door_is_open) {
                return true;
            }
        }
    }
    return false;
}

bool gamestate::update_player_tiles_explored() {
    if (current_scene != scene_t::GAMEPLAY) {
        return false;
    }
    if (hero_id == ENTITYID_INVALID) {
        merror2("hero_id is invalid");
        return false;
    }
    auto df = d.get_current_floor();
    const vec3* hero_loc_ptr = ct.get<location>(hero_id);
    if (!hero_loc_ptr) {
        merror2("hero location lacks value");
        return false;
    }
    vec3 hero_loc = *hero_loc_ptr;
    for (int y = 0; y < df->get_height(); y++) {
        for (int x = 0; x < df->get_width(); x++) {
            df->tile_at(vec3{x, y, hero_loc.z}).set_visible(false);
        }
    }
    int light_radius0 = ct.get_or<light_radius>(hero_id, 1);
    int min_x = std::max(0, hero_loc.x - light_radius0);
    int max_x = std::min(df->get_width() - 1, hero_loc.x + light_radius0);
    int min_y = std::max(0, hero_loc.y - light_radius0);
    int max_y = std::min(df->get_height() - 1, hero_loc.y + light_radius0);
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            if (abs(x - hero_loc.x) + abs(y - hero_loc.y) > light_radius0) {
                continue;
            }
            vec3 loc = {x, y, hero_loc.z};
            if (visibility_path_blocked(hero_loc, loc)) {
                continue;
            }
            tile_t& t = df->tile_at(loc);
            update_tile(t);
        }
    }
    return true;
}

bool gamestate::update_player_state() {
    if (hero_id == ENTITYID_INVALID) {
        merror2("hero_id is invalid");
        return false;
    }
    if (ct.get_or<dead>(hero_id, true)) {
        merror2("hero_id is dead");
        gameover = true;
        return true;
    }
    ct.set<blocking>(hero_id, false);
    ct.set<block_success>(hero_id, false);
    ct.set<damaged>(hero_id, false);
    return true;
}

void gamestate::update_npcs_state() {
    minfo2("BEGIN update_npcs_state");
    auto view = registry.view<LegacyEntityId, NpcTag>();
    for (auto entity : view) {
        entityid id = view.get<LegacyEntityId>(entity).id;
        ct.set<damaged>(id, false);
        update_npc_behavior(id);
    }
}

void gamestate::update_npc_behavior(entityid id) {
    if (ct.get_or<entitytype>(id, entitytype_t::NONE) != entitytype_t::NPC) {
        return;
    }
    if (ct.get_or<dead>(id, true)) {
        ct.set<entity_default_action>(id, entity_default_action_t::NONE);
        ct.set<target_id>(id, ENTITYID_INVALID);
        return;
    }

    const bool is_aggressive = ct.get_or<aggro>(id, false);
    if (hero_id == ENTITYID_INVALID || !is_aggressive || !ct.has<location>(hero_id)) {
        ct.set<entity_default_action>(id, entity_default_action_t::RANDOM_MOVE);
        ct.set<target_id>(id, ENTITYID_INVALID);
        return;
    }

    const vec3 npc_loc = ct.get_or<location>(id, vec3{-1, -1, -1});
    const vec3 hero_loc = ct.get_or<location>(hero_id, vec3{-1, -1, -1});
    if (vec3_invalid(npc_loc) || vec3_invalid(hero_loc) || npc_loc.z != hero_loc.z) {
        ct.set<entity_default_action>(id, entity_default_action_t::RANDOM_MOVE);
        ct.set<target_id>(id, ENTITYID_INVALID);
        return;
    }

    ct.set<target_id>(id, hero_id);
    if (is_entity_adjacent(id, hero_id)) {
        ct.set<entity_default_action>(id, entity_default_action_t::ATTACK_TARGET_IF_ADJACENT);
        return;
    }

    ct.set<entity_default_action>(id, entity_default_action_t::MOVE_TO_TARGET_AND_ATTACK_TARGET_IF_ADJACENT);
}

void gamestate::logic_init() {
    minfo("gamestate.logic_init");
    srand(time(NULL));
    SetRandomSeed(time(NULL));
    constexpr float parts = 1.0;
    massert(!d.is_initialized, "dungeon is already initialized");
    create_and_add_df_1(biome_t::STONE, 8, 8, 4, parts);
    create_and_add_df_1(biome_t::STONE, 24, 24, 4, parts);
    create_and_add_df_1(biome_t::STONE, 16, 16, 4, parts);
    create_and_add_df_0(biome_t::STONE, 16, 16, 4, parts);
    const bool stairs_assigned = assign_random_stairs();
    massert(stairs_assigned, "failed to assign dungeon stairs");
    if (!stairs_assigned) {
        merror("failed to assign dungeon stairs");
        return;
    }
    d.current_floor = 0;
    d.is_initialized = true;

    massert(d.floors.size() > 0, "dungeon.floors.size is 0");
    setup_floor_four_pressure_plate_tutorial();
    place_doors();
    place_props();
    place_floor_three_pullable_props();
    place_floor_three_pullable_sign();
    place_first_floor_chest();
    auto floor_zero = d.get_current_floor();
    constexpr int num_boxes = 1;
    for (int i = 0; i < num_boxes; i++) {
        create_box_at_with(floor_zero->get_random_loc());
    }
    auto floor_three = d.get_floor(3);
    constexpr int floor_three_box_count = 3;
    for (int i = 0; i < floor_three_box_count; i++) {
        create_box_at_with(floor_three->get_random_loc());
    }
    create_weapon_at_with(ct, floor_zero->get_random_loc(), sword_init());
    create_shield_at_with(ct, floor_zero->get_random_loc(), shield_init());
    auto green_slime_init = [](CT& ct, const entityid id) {
        ct.set<name>(id, "green slime");
        ct.set<dialogue_text>(id, "The slime jiggles quietly.");
        ct.set<aggro>(id, false);
        ct.set<level>(id, 1);
        ct.set<xp>(id, 0);
    };
    auto armed_orc_init = [this](CT& ct, const entityid id) {
        vector<with_fun> weapon_inits = {
            dagger_init(),
            sword_init(),
            axe_init(),
        };
        uniform_int_distribution<int> weapon_dist(0, static_cast<int>(weapon_inits.size()) - 1);
        const entityid weapon_id = create_weapon_with(weapon_inits[weapon_dist(random.mt)]);
        const entityid potion_id = create_potion_with(potion_init(potiontype_t::HP_SMALL));
        add_to_inventory(id, weapon_id);
        add_to_inventory(id, potion_id);
        ct.set<equipped_weapon>(id, weapon_id);
        ct.set<aggro>(id, true);
    };

    const race_t friendly_race = static_cast<race_t>(GetRandomValue(static_cast<int>(race_t::NONE) + 1, static_cast<int>(race_t::COUNT) - 1));
    const vec3 friendly_loc = d.get_floor(0)->get_random_loc();
    create_npc_at_with(friendly_race, friendly_loc, [](CT& ct, const entityid id) {
        ct.set<aggro>(id, false);
    });

    auto floor_one = d.get_floor(1);
    for (int i = 0; i < 9; i++) {
        const vec3 slime_loc = floor_one->get_random_loc();
        create_npc_at_with(race_t::GREEN_SLIME, slime_loc, green_slime_init);
    }

    const vec3 floor_two_orc_loc = vec3_valid(floor_four_tutorial_orc_spawn) ? floor_four_tutorial_orc_spawn : d.get_floor(2)->get_random_loc();
    create_orc_at_with(floor_two_orc_loc, armed_orc_init);
    msuccess("end creating monsters...");
    messages.add("Welcome to the game! Press enter to cycle messages.");
    messages.add("For help, press ?");
    msuccess("liblogic_init: Game state initialized");
}

void gamestate::restart_game() {
    const unsigned int previous_restart_count = restart_count;
    const int previous_target_width = targetwidth;
    const int previous_target_height = targetheight;
    const int previous_window_width = windowwidth;
    const int previous_window_height = windowheight;
    logic_close();
    reset();
    targetwidth = previous_target_width > 0 ? previous_target_width : DEFAULT_TARGET_WIDTH;
    targetheight = previous_target_height > 0 ? previous_target_height : DEFAULT_TARGET_HEIGHT;
    windowwidth = previous_window_width > 0 ? previous_window_width : DEFAULT_WIN_WIDTH;
    windowheight = previous_window_height > 0 ? previous_window_height : DEFAULT_WIN_HEIGHT;
    cam2d.offset = Vector2{targetwidth / 4.0f, targetheight / 4.0f};
    logic_init();
    do_restart = false;
    restart_count = previous_restart_count + 1;
    current_scene = scene_t::TITLE;
    frame_dirty = true;
}

void gamestate::advance_animation_phase() {
    minfo2(
        "handle test flag: %s",
        flag == gamestate_flag_t::PLAYER_ANIM    ? "player anim"
        : flag == gamestate_flag_t::PLAYER_INPUT ? "player input"
        : flag == gamestate_flag_t::NPC_TURN     ? "npc turn"
        : flag == gamestate_flag_t::NPC_ANIM     ? "npc anim"
                                              : "Unknown");
    if (flag == gamestate_flag_t::PLAYER_ANIM) {
#ifndef NPCS_ALL_AT_ONCE
        entity_turn++;
        if (entity_turn >= next_entityid) {
            entity_turn = 1;
        }
#endif
        flag = gamestate_flag_t::NPC_TURN;
    }
    else if (flag == gamestate_flag_t::NPC_ANIM) {
#ifndef NPCS_ALL_AT_ONCE
        entity_turn++;
        if (entity_turn >= next_entityid) {
            entity_turn = 1;
        }
        if (entity_turn == hero_id) {
            flag = gamestate_flag_t::PLAYER_INPUT;
            turn_count++;
        }
        else {
            flag = gamestate_flag_t::NPC_TURN;
        }
#else
        flag = gamestate_flag_t::PLAYER_INPUT;
        turn_count++;
#endif
    }
}

void gamestate::finalize_render_feedback() {
    auto view = registry.view<LegacyEntityId>();
    for (auto entity : view) {
        entityid id = view.get<LegacyEntityId>(entity).id;
        if (ct.has<update>(id)) {
            ct.set<update>(id, false);
        }
        if (ct.has<attacking>(id)) {
            ct.set<attacking>(id, false);
        }
        if (ct.has<block_success>(id)) {
            ct.set<block_success>(id, false);
        }
        if (ct.has<spritemove>(id)) {
            ct.set<spritemove>(id, Rectangle{0, 0, 0, 0});
        }
    }

    dirty_entities = false;
    new_entityid_begin = ENTITYID_INVALID;
    new_entityid_end = ENTITYID_INVALID;
}

void gamestate::tick(inputstate& is) {
    minfo3("tick");

    if (!update_player_tiles_explored()) {
        merror3("update player tiles explored failed");
    }

    if (!update_player_state()) {
        merror3("update player state failed");
    }

    update_npcs_state();
    handle_input(is);
    handle_npcs();
    damage_popups_sys.update(test ? (1.0f / DEFAULT_TARGET_FPS) : std::max(GetFrameTime(), 1.0f / 240.0f));
    if (damage_popups_sys.dirty) {
        frame_dirty = true;
        damage_popups_sys.dirty = false;
    }
#ifdef DEBUG
    update_debug_panel_buffer(is);
#endif
    currenttime = time(NULL);
    currenttimetm = localtime(&currenttime);
    strftime(currenttimebuf, GAMESTATE_SIZEOFTIMEBUF, "Current Time: %Y-%m-%d %H:%M:%S", currenttimetm);
    ticks++;
    minfo2("end tick");
}
