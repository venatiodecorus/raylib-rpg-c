#include "draw_dungeon_floor.h"

#include "draw_sprite.h"
#include "ecs_gameplay_components.h"
#include "get_txkey_for_tiletype.h"
#include "libdraw_context.h"

#include <algorithm>
#include <functional>

bool libdraw_draw_player_target_box(gamestate& g, rpg::Renderer& renderer) {
    entityid id = g.hero_id;
    if (id == -1) {
        return false;
    }
    const Facing* dir_ptr = g.get_component<Facing>(id);
    const Position* loc_ptr = g.get_component<Position>(id);
    if (!dir_ptr || !loc_ptr) {
        return false;
    }
    direction_t dir = dir_ptr->value;
    vec3 loc = loc_ptr->value;
    float x = loc.x + get_x_from_dir(dir);
    float y = loc.y + get_y_from_dir(dir);
    float w = DEFAULT_TILE_SIZE;
    float h = DEFAULT_TILE_SIZE;
    float a = 0.75f;
    if (g.player_changing_dir) {
        a = 0.9f;
    }
    float time = static_cast<float>(GetTime());
    SetShaderValue(renderer.shaders[1], GetShaderLocation(renderer.shaders[1], "time"), &time, SHADER_UNIFORM_FLOAT);
    SetShaderValue(renderer.shaders[1], GetShaderLocation(renderer.shaders[1], "alpha"), &a, SHADER_UNIFORM_FLOAT);
    BeginShaderMode(renderer.shaders[1]);
    DrawRectangleLinesEx(Rectangle{x * w + DEFAULT_OFFSET, y * h + DEFAULT_OFFSET, w, h}, 1, Fade(GREEN, a));
    EndShaderMode();
    return true;
}

constexpr int manhattan_distance(vec3 a, vec3 b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

bool draw_dungeon_floor_tile(gamestate& g, rpg::Renderer& renderer, int x, int y, int z, [[maybe_unused]] int light_dist, [[maybe_unused]] vec3 hero_loc, [[maybe_unused]] int distance) {
    massert(z >= 0 && static_cast<size_t>(z) < g.d.get_floor_count(), "z is oob");
    const float px = x * DEFAULT_TILE_SIZE + DEFAULT_OFFSET;
    const float py = y * DEFAULT_TILE_SIZE + DEFAULT_OFFSET;
    constexpr float ts = static_cast<float>(DEFAULT_TILE_SIZE);
    const Rectangle dest = {px, py, ts, ts};
    auto df = g.d.get_floor(z);
    massert(df, "dungeon_floor is NULL");
    massert(x >= 0 && x < df->get_width(), "x is oob");
    massert(y >= 0 && y < df->get_height(), "y is oob");
    massert(!vec3_invalid(vec3{x, y, z}), "loc is invalid");
    tile_t& tile = df->tile_at(vec3{x, y, z});
    if (tile.get_type() == tiletype_t::NONE) {
        return true;
    }
    const int txkey = get_txkey_for_tiletype(tile.get_type());
    massert(txkey >= 0, "txkey is invalid");
    const Texture2D* texture = &renderer.txinfo[txkey].texture;
    massert(texture->id > 0, "texture->id is <= 0");
    const Rectangle src = {0, 0, static_cast<float>(texture->width), static_cast<float>(texture->height)};
    DrawTexturePro(*texture, src, dest, Vector2{0, 0}, 0, WHITE);
    return true;
}

void draw_dungeon_floor_pressure_plates(gamestate& g, rpg::Renderer& renderer, int light_rad) {
    auto df = g.d.get_current_floor();
    const int z = g.d.current_floor;
    const vec3 hero_loc = g.get_component_or<Position>(g.hero_id, vec3{-1, -1, -1});
    const bool full_light = df->get_full_light();
    constexpr float ts = static_cast<float>(DEFAULT_TILE_SIZE);

    for (const floor_pressure_plate_t& plate : g.floor_pressure_plates) {
        if (plate.destroyed || plate.loc.z != z) {
            continue;
        }

        if (!full_light) {
            if (abs(plate.loc.x - hero_loc.x) + abs(plate.loc.y - hero_loc.y) > light_rad) {
                continue;
            }

            tile_t& tile = df->tile_at(plate.loc);
            if (!tile.get_explored() || !tile.get_visible()) {
                continue;
            }
        }

        const int txkey = plate.active ? plate.txkey_down : plate.txkey_up;
        const Texture2D* texture = &renderer.txinfo[txkey].texture;
        massert(texture->id > 0, "pressure plate texture->id is <= 0");

        const Rectangle src = {0, 0, static_cast<float>(texture->width), static_cast<float>(texture->height)};
        const float px = plate.loc.x * DEFAULT_TILE_SIZE + DEFAULT_OFFSET;
        const float py = plate.loc.y * DEFAULT_TILE_SIZE + DEFAULT_OFFSET;
        const Rectangle dest = {px, py, ts, ts};
        DrawTexturePro(*texture, src, dest, Vector2{0, 0}, 0, WHITE);
    }
}

void draw_dungeon_floor_entitytype(gamestate& g, rpg::Renderer& renderer, entitytype_t type_0, [[maybe_unused]] int vision_dist, [[maybe_unused]] int light_rad, const std::function<bool(gamestate&, entityid)>& extra_check) {
    auto df = g.d.get_current_floor();
    for (int y = 0; y < df->get_height(); y++) {
        for (int x = 0; x < df->get_width(); x++) {
            const vec3 loc = {x, y, g.d.current_floor};
            tile_t& tile = df->tile_at(loc);
            auto tiletype = tile.get_type();
            if (tiletype_is_none(tiletype) || tiletype_is_wall(tiletype)) {
                continue;
            }
            if (tile.entity_count() == 0) {
                continue;
            }

            if (type_0 == entitytype_t::NPC) {
                entityid dead_npc_id = tile.get_cached_dead_npc();
                if (dead_npc_id != INVALID && extra_check(g, dead_npc_id)) {
                    draw_sprite_and_shadow(g, renderer, dead_npc_id);
                }
                entityid npc_id = tile.get_cached_live_npc();
                if (npc_id != INVALID && extra_check(g, npc_id)) {
                    draw_sprite_and_shadow(g, renderer, npc_id);
                }
            }
            else if (type_0 == entitytype_t::PLAYER) {
                bool player_present = tile.get_cached_player_present();
                if (player_present && extra_check(g, g.hero_id)) {
                    draw_sprite_and_shadow(g, renderer, g.hero_id);
                }
            }
            else if (type_0 == entitytype_t::BOX) {
                entityid box_id = tile.get_cached_box();
                if (box_id != INVALID && extra_check(g, box_id)) {
                    draw_sprite_and_shadow(g, renderer, box_id);
                }
            }
            else if (type_0 == entitytype_t::CHEST) {
                entityid chest_id = tile.get_cached_chest();
                if (chest_id != INVALID && extra_check(g, chest_id)) {
                    draw_sprite_and_shadow(g, renderer, chest_id);
                }
            }
            else if (type_0 == entitytype_t::PROP) {
                entityid prop_id = tile.get_cached_prop();
                if (prop_id != INVALID && extra_check(g, prop_id)) {
                    draw_sprite_and_shadow(g, renderer, prop_id);
                }
            }
            else if (type_0 == entitytype_t::ITEM) {
                entityid item_id = tile.get_cached_item();
                if (item_id != INVALID && extra_check(g, item_id)) {
                    draw_sprite_and_shadow(g, renderer, item_id);
                }
            }
            else if (type_0 == entitytype_t::DOOR) {
                entityid door_id = tile.get_cached_door();
                if (door_id != INVALID && extra_check(g, door_id)) {
                    draw_sprite_and_shadow(g, renderer, door_id);
                }
            }
        }
    }
}

bool draw_dungeon_floor(gamestate& g, rpg::Renderer& renderer, int vision_dist, int light_rad) {
    shared_ptr<dungeon_floor> df = g.d.get_current_floor();
    const int z = g.d.current_floor;
    const Position* hero_loc_ptr = g.get_component<Position>(g.hero_id);
    if (!hero_loc_ptr) {
        return false;
    }

    const vec3 hero_loc = hero_loc_ptr->value;
    for (int y = 0; y < df->get_height(); y++) {
        for (int x = 0; x < df->get_width(); x++) {
            const vec3 loc = {x, y, z};
            const int distance = manhattan_distance(loc, hero_loc);
            draw_dungeon_floor_tile(g, renderer, x, y, z, light_rad, hero_loc, distance);
        }
    }
    draw_dungeon_floor_pressure_plates(g, renderer, light_rad);

    auto mydefault = [](gamestate& g, entityid id) { return true; };
    auto alive_check = [](gamestate& g, entityid id) {
        const DeadFlag* dead_ptr = g.get_component<DeadFlag>(id);
        if (dead_ptr) {
            return !dead_ptr->value;
        }
        return false;
    };

    auto dead_check = [](gamestate& g, entityid id) {
        const DeadFlag* dead_ptr = g.get_component<DeadFlag>(id);
        if (dead_ptr) {
            return dead_ptr->value;
        }
        return false;
    };

    // TODO: re-enable DOOR, PROP, CHEST, ITEM, BOX drawing when proper sprites are mapped
    libdraw_draw_player_target_box(g, renderer);
    draw_dungeon_floor_entitytype(g, renderer, entitytype_t::NPC, vision_dist, light_rad, dead_check);
    draw_dungeon_floor_entitytype(g, renderer, entitytype_t::NPC, vision_dist, light_rad, alive_check);
    draw_dungeon_floor_entitytype(g, renderer, entitytype_t::PLAYER, vision_dist, light_rad, mydefault);
    return true;
}
