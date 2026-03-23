#include "ecs_actor_components.h"
#include "ecs_core_components.h"
#include "ecs_gameplay_components.h"
#include "gamestate.h"
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
            bool door_is_open = get_component_or<DoorOpenFlag>(door_id, false);
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
            bool door_is_open = get_component_or<DoorOpenFlag>(door_id, false);
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
    const Position* hero_pos = get_component<Position>(hero_id);
    if (!hero_pos) {
        merror2("hero location lacks value");
        return false;
    }
    vec3 hero_loc = hero_pos->value;
    for (int y = 0; y < df->get_height(); y++) {
        for (int x = 0; x < df->get_width(); x++) {
            df->tile_at(vec3{x, y, hero_loc.z}).set_visible(false);
        }
    }
    int light_radius0 = get_component_or<LightRadius>(hero_id, 1);
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
    if (get_component_or<DeadFlag>(hero_id, true)) {
        merror2("hero_id is dead");
        gameover = true;
        return true;
    }
    return true;
}

void gamestate::update_npcs_state() {
    minfo2("BEGIN update_npcs_state");
    auto view = registry.view<NpcTag>();
    for (auto entity : view) {
        update_npc_behavior(entity);
    }
}

void gamestate::update_npc_behavior(entityid id) {
    if ((get_component<EntityTypeTag>(id) ? get_component<EntityTypeTag>(id)->type : entitytype_t::NONE) != entitytype_t::NPC) {
        return;
    }
    if (get_component_or<DeadFlag>(id, true)) {
        return;
    }

    const bool is_aggressive = get_component_or<AggroFlag>(id, false);
    if (hero_id == ENTITYID_INVALID || !is_aggressive || !has_component<Position>(hero_id)) {
        return;
    }

    const vec3 npc_loc = get_component_or<Position>(id, vec3{-1, -1, -1});
    const vec3 hero_loc = get_component_or<Position>(hero_id, vec3{-1, -1, -1});
    if (vec3_invalid(npc_loc) || vec3_invalid(hero_loc) || npc_loc.z != hero_loc.z) {
        return;
    }

    if (is_entity_adjacent(id, hero_id)) {
        return;
    }
}

void gamestate::logic_init() {
    minfo("gamestate.logic_init");
    srand(time(NULL));
    SetRandomSeed(time(NULL));
    massert(!d.is_initialized, "dungeon is already initialized");
    create_and_add_static_floor(biome_t::STONE);
    d.current_floor = 0;
    d.is_initialized = true;

    massert(d.floors.size() > 0, "dungeon.floors.size is 0");
    auto floor_zero = d.get_current_floor();

    // Place a box in Room B
    create_box_at_with(vec3{8, 2, 0});

    // Place a weapon and shield in Room A
    create_weapon_at_with(vec3{2, 2, 0}, sword_init());
    create_shield_at_with(vec3{2, 3, 0}, shield_init());

    // Place a friendly NPC in Room A
    create_npc_at_with(race_t::HUMAN, vec3{3, 2, 0}, [](gamestate& g, const entityid id) {
        g.registry.emplace_or_replace<AggroFlag>(id, AggroFlag{false});
    });

    // Place a hostile slime in Room C
    auto green_slime_init = [](gamestate& g, const entityid id) {
        const auto e = id;
        g.registry.emplace_or_replace<EntityName>(e, EntityName{"green slime"});
        g.registry.emplace_or_replace<DialogueLine>(e, DialogueLine{"The slime jiggles quietly."});
        g.registry.emplace_or_replace<AggroFlag>(e, AggroFlag{true});
        g.registry.emplace_or_replace<EntityLevel>(e, EntityLevel{1});
        g.registry.emplace_or_replace<Experience>(e, Experience{0});
    };
    create_npc_at_with(race_t::GREEN_SLIME, vec3{5, 8, 0}, green_slime_init);

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
        flag = gamestate_flag_t::NPC_TURN;
    }
    else if (flag == gamestate_flag_t::NPC_ANIM) {
        flag = gamestate_flag_t::PLAYER_INPUT;
        turn_count++;
    }
}

void gamestate::finalize_render_feedback() {
    for (auto entity : registry.view<EntityTypeTag>()) {
        if (auto* upd = get_component<NeedsUpdate>(entity)) {
            upd->value = false;
        }
        if (auto* atk = get_component<AttackingFlag>(entity)) {
            atk->value = false;
        }
        if (auto* blk = get_component<BlockSuccessFlag>(entity)) {
            blk->value = false;
        }
        if (auto* sm = get_component<SpriteMoveState>(entity)) {
            sm->value = Rectangle{0, 0, 0, 0};
        }
    }

    new_entities.clear();
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
