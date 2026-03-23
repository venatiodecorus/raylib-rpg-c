#include "ecs_gameplay_components.h"
#include "gamestate.h"

/** @file gamestate_inventory.cpp
 *  @brief Inventory/chest management helpers implemented on `gamestate`.
 */


bool gamestate::use_mini_inventory_menu() const {
    return ui.prefer_mini_inventory_menu;
}

size_t gamestate::get_inventory_selection_index() const {
    if (!use_mini_inventory_menu()) {
        return static_cast<size_t>(ui.inventory_cursor.y) * 7 + static_cast<size_t>(ui.inventory_cursor.x);
    }
    return static_cast<size_t>(ui.mini_inventory_scroll_offset) + static_cast<size_t>(ui.inventory_cursor.y);
}

void gamestate::clamp_inventory_selection(size_t item_count) {
    if (!use_mini_inventory_menu()) {
        if (item_count == 0) {
            ui.inventory_cursor = Vector2{0, 0};
            return;
        }
        const size_t max_index = item_count - 1;
        size_t index = static_cast<size_t>(ui.inventory_cursor.y) * 7 + static_cast<size_t>(ui.inventory_cursor.x);
        if (index > max_index) {
            ui.inventory_cursor.x = static_cast<float>(max_index % 7);
            ui.inventory_cursor.y = static_cast<float>(max_index / 7);
        }
        return;
    }

    if (item_count == 0) {
        ui.mini_inventory_scroll_offset = 0;
        ui.inventory_cursor = Vector2{0, 0};
        return;
    }

    const size_t visible_count = std::max(1U, ui.mini_inventory_visible_count);
    const size_t max_index = item_count - 1;
    size_t index = get_inventory_selection_index();
    if (index > max_index) {
        index = max_index;
    }
    const size_t max_scroll = item_count > visible_count ? item_count - visible_count : 0;
    if (index < ui.mini_inventory_scroll_offset) {
        ui.mini_inventory_scroll_offset = static_cast<unsigned int>(index);
    }
    else if (index >= ui.mini_inventory_scroll_offset + visible_count) {
        ui.mini_inventory_scroll_offset = static_cast<unsigned int>(index - visible_count + 1);
    }
    ui.mini_inventory_scroll_offset = static_cast<unsigned int>(std::min(static_cast<size_t>(ui.mini_inventory_scroll_offset), max_scroll));
    ui.inventory_cursor.x = 0.0f;
    ui.inventory_cursor.y = static_cast<float>(index - ui.mini_inventory_scroll_offset);
}

void gamestate::move_inventory_selection(int delta) {
    if (!use_mini_inventory_menu()) {
        if (delta < 0) {
            if (ui.inventory_cursor.x > 0) {
                ui.inventory_cursor.x--;
            }
            else if (ui.inventory_cursor.y > 0) {
                ui.inventory_cursor.y--;
                ui.inventory_cursor.x = 6;
            }
        }
        else if (delta > 0) {
            ui.inventory_cursor.x++;
            if (ui.inventory_cursor.x > 6) {
                ui.inventory_cursor.x = 0;
                ui.inventory_cursor.y++;
            }
        }
        return;
    }

    int next_index = static_cast<int>(get_inventory_selection_index()) + delta;
    if (next_index < 0) {
        next_index = 0;
    }
    const size_t current_index = static_cast<size_t>(next_index);
    ui.mini_inventory_scroll_offset = static_cast<unsigned int>(std::min(static_cast<size_t>(ui.mini_inventory_scroll_offset), current_index));
    ui.inventory_cursor.x = 0.0f;
    ui.inventory_cursor.y = static_cast<float>(current_index - ui.mini_inventory_scroll_offset);
}

bool gamestate::remove_from_inventory(entityid actor_id, entityid item_id) {
    auto* maybe_inventory = get_component<Inventory>(actor_id);
    if (!maybe_inventory) {
        merror("maybe_inventory has no value for actor id %d", actor_id);
        return false;
    }
    auto& my_items = maybe_inventory->value;
    bool success = false;
    for (auto it = my_items.begin(); it != my_items.end(); it++) {
        if (*it == item_id) {
            my_items.erase(it);
            success = true;
            break;
        }
    }

    if (!success) {
        merror("Failed to find item id %d", item_id);
    }
    else {
        msuccess("Successfully removed item id %d", item_id);
    }
    return success;
}

bool gamestate::drop_from_inventory(entityid actor_id, entityid item_id) {
    if (remove_from_inventory(actor_id, item_id)) {
        auto* maybe_loc = get_component<Position>(actor_id);
        if (!maybe_loc) {
            merror("actor id %d has no location -- cannot drop item", actor_id);
            return false;
        }
        vec3 loc = maybe_loc->value;
        auto df = d.get_current_floor();
        entityid retval = df->df_add_at(item_id, entitytype_t::ITEM, loc);
        if (retval == ENTITYID_INVALID) {
            merror("Failed to add to tile");
            return false;
        }
        msuccess("Drop item successful");
        return true;
    }
    merror("Remove from inventory failed for some reason");
    return false;
}

bool gamestate::drop_all_from_inventory(entityid actor_id) {
    const auto* maybe_inventory = get_component<Inventory>(actor_id);
    if (!maybe_inventory) {
        merror("no inventory");
        return false;
    }
    auto& inventory = maybe_inventory->value;
    while (inventory.size() > 0) {
        auto id = inventory.back();
        drop_from_inventory(actor_id, id);
    }
    return true;
}

void gamestate::handle_hero_inventory_equip_weapon(entityid item_id) {
    entityid current_weapon = get_component_or<EquippedWeapon>(hero_id, ENTITYID_INVALID);
    if (current_weapon == item_id) {
    }
    else {
    }
    flag = gamestate_flag_t::PLAYER_ANIM;
    controlmode = controlmode_t::PLAYER;
    ui.display_inventory_menu = false;
}

void gamestate::handle_hero_inventory_equip_shield(entityid item_id) {
    entityid current_shield = get_component_or<EquippedShield>(hero_id, ENTITYID_INVALID);
    if (current_shield == item_id) {
    }
    else {
    }
    flag = gamestate_flag_t::PLAYER_ANIM;
    controlmode = controlmode_t::PLAYER;
    ui.display_inventory_menu = false;
}

void gamestate::handle_hero_inventory_equip_item(entityid item_id) {
    itemtype_t item_type = get_component_or<ItemSubtype>(item_id, itemtype_t::NONE);
    switch (item_type) {
    case itemtype_t::NONE: break;
    case itemtype_t::WEAPON: handle_hero_inventory_equip_weapon(item_id); break;
    case itemtype_t::SHIELD: handle_hero_inventory_equip_shield(item_id); break;
    default: break;
    }
}

void gamestate::handle_hero_inventory_equip() {
    audio.queue("sfx/Minifantasy_Dungeon_SFX/08_human_charge_1.wav");
    size_t index = get_inventory_selection_index();
    const auto* my_inventory = get_component<Inventory>(hero_id);
    if (!my_inventory) {
        return;
    }
    auto& inventory = my_inventory->value;
    if (index >= inventory.size()) {
        return;
    }
    entityid item_id = inventory.at(index);
    entitytype_t type = (get_component<EntityTypeTag>(item_id) ? get_component<EntityTypeTag>(item_id)->type : entitytype_t::NONE);
    if (type == entitytype_t::ITEM) {
        run_equip_item_action(hero_id, item_id);
    }
}

bool gamestate::drop_item_from_hero_inventory() {
    if (!has_component<Inventory>(hero_id)) {
        return false;
    }
    size_t index = get_inventory_selection_index();
    const auto* maybe_inventory = get_component<Inventory>(hero_id);
    if (!maybe_inventory) {
        return false;
    }
    auto& inventory = maybe_inventory->value;
    if (index >= inventory.size()) {
        return false;
    }
    entityid item_id = inventory.at(index);
    return drop_inventory_item(hero_id, item_id);
}

bool gamestate::drop_inventory_item(entityid actor_id, entityid item_id) {
    if (actor_id == ENTITYID_INVALID || item_id == ENTITYID_INVALID) {
        return false;
    }
    if (!is_in_inventory(actor_id, item_id)) {
        return false;
    }

    const auto* maybe_loc = get_component<Position>(actor_id);
    if (!maybe_loc) {
        return false;
    }

    if (!remove_from_inventory(actor_id, item_id)) {
        return false;
    }

    if (item_id == get_component_or<EquippedWeapon>(actor_id, ENTITYID_INVALID)) {
    }
    if (item_id == get_component_or<EquippedShield>(actor_id, ENTITYID_INVALID)) {
    }

    const vec3 loc = maybe_loc->value;
    auto df = d.get_current_floor();
    if (static_cast<size_t>(loc.z) < d.get_floor_count()) {
        df = d.get_floor(loc.z);
    }
    if (df->df_add_at(item_id, entitytype_t::ITEM, loc) == ENTITYID_INVALID) {
        merror("Failed to add to %d, %d, %d", loc.x, loc.y, loc.z);
        add_to_inventory(actor_id, item_id);
        return false;
    }
    return true;
}

bool gamestate::is_in_inventory(entityid actor_id, entityid item_id) {
    const auto* maybe_inventory = get_component<Inventory>(actor_id);
    if (!maybe_inventory) {
        merror("maybe_inventory has no value for actor id %d", actor_id);
        return false;
    }
    auto& my_items = maybe_inventory->value;
    for (auto it = my_items.begin(); it != my_items.end(); it++) {
        if (*it == item_id) {
            return true;
        }
    }
    return false;
}

bool gamestate::use_potion(entityid actor_id, entityid item_id) {
    massert(actor_id != ENTITYID_INVALID, "actor_id is invalid");
    massert(item_id != ENTITYID_INVALID, "actor_id is invalid");
    bool is_item = (get_component<EntityTypeTag>(item_id) ? get_component<EntityTypeTag>(item_id)->type : entitytype_t::NONE) == entitytype_t::ITEM;
    bool is_potion = get_component_or<ItemSubtype>(item_id, itemtype_t::NONE) == itemtype_t::POTION;
    bool in_inventory = is_in_inventory(actor_id, item_id);
    if (is_item && is_potion && in_inventory) {
        const HealingDice* heal_ptr = get_component<HealingDice>(item_id);
        if (heal_ptr) {
            vec3 heal = heal_ptr->value;
            int amount = do_roll(heal);
            vec2 actor_hp = get_component_or<HitPoints>(actor_id, vec2{-1, -1});
            if (vec2_invalid(actor_hp)) {
                merror("actor has no hp component");
                return false;
            }
            actor_hp.x = std::min(actor_hp.y, actor_hp.x + amount);
            if (actor_id == hero_id) {
                string n = get_component_or<EntityName>(actor_id, "no-name");
                messages.add_history("%s used a healing potion", n.c_str());
                messages.add_history("%s restored %d hp", n.c_str(), amount);
            }
        }
        else {
            merror("Potion has no healing component");
            return false;
        }
        remove_from_inventory(actor_id, item_id);
        return true;
    }
    merror("id %d is not an item, potion, or isnt in the inventory", item_id);
    return false;
}

void gamestate::handle_hero_potion_use(entityid id) {
    entitytype_t type = (get_component<EntityTypeTag>(id) ? get_component<EntityTypeTag>(id)->type : entitytype_t::NONE);
    if (type != entitytype_t::ITEM) {
        return;
    }
    itemtype_t i_type = get_component_or<ItemSubtype>(id, itemtype_t::NONE);
    if (i_type == itemtype_t::NONE || i_type != itemtype_t::POTION) {
        return;
    }
    run_use_item_action(hero_id, id);
}

void gamestate::handle_hero_item_use() {
    size_t index = get_inventory_selection_index();
    const auto* maybe_inventory = get_component<Inventory>(hero_id);
    if (!maybe_inventory) {
        return;
    }
    auto& inventory = maybe_inventory->value;
    if (index >= inventory.size()) {
        return;
    }
    entityid item_id = inventory.at(index);
    entitytype_t type = (get_component<EntityTypeTag>(item_id) ? get_component<EntityTypeTag>(item_id)->type : entitytype_t::NONE);
    if (type != entitytype_t::ITEM) {
        return;
    }
    itemtype_t i_type = get_component_or<ItemSubtype>(item_id, itemtype_t::NONE);
    if (i_type == itemtype_t::NONE) {
        return;
    }
    if (i_type == itemtype_t::POTION) {
        handle_hero_potion_use(item_id);
    }
}

bool gamestate::transfer_inventory_item(entityid from_id, entityid to_id, entityid item_id) {
    if (from_id == ENTITYID_INVALID || to_id == ENTITYID_INVALID || item_id == ENTITYID_INVALID) {
        return false;
    }
    if (!is_in_inventory(from_id, item_id)) {
        return false;
    }
    if (!remove_from_inventory(from_id, item_id)) {
        return false;
    }
    if (!add_to_inventory(to_id, item_id)) {
        add_to_inventory(from_id, item_id);
        return false;
    }
    if (from_id == hero_id) {
        if (item_id == get_component_or<EquippedWeapon>(hero_id, ENTITYID_INVALID)) {
        }
        if (item_id == get_component_or<EquippedShield>(hero_id, ENTITYID_INVALID)) {
        }
    }
    return true;
}

bool gamestate::open_chest_menu(entityid chest_id) {
    if (chest_id == ENTITYID_INVALID || (get_component<EntityTypeTag>(chest_id) ? get_component<EntityTypeTag>(chest_id)->type : entitytype_t::NONE) != entitytype_t::CHEST) {
        return false;
    }
    sync_registry_open_state(chest_id, true);
    ui.active_chest_id = chest_id;
    ui.display_chest_menu = true;
    ui.display_inventory_menu = false;
    ui.chest_deposit_mode = false;
    ui.mini_inventory_scroll_offset = 0;
    ui.inventory_cursor = Vector2{0, 0};
    controlmode = controlmode_t::CHEST;
    frame_dirty = true;
    audio.queue("sfx/Minifantasy_Dungeon_SFX/01_chest_open_1.wav");
    return true;
}

void gamestate::close_chest_menu() {
    if (ui.active_chest_id != ENTITYID_INVALID) {
        sync_registry_open_state(ui.active_chest_id, false);
    }
    ui.display_chest_menu = false;
    ui.chest_deposit_mode = false;
    ui.active_chest_id = ENTITYID_INVALID;
    controlmode = controlmode_t::PLAYER;
    frame_dirty = true;
}

void gamestate::toggle_chest_menu_mode() {
    if (!ui.display_chest_menu || ui.active_chest_id == ENTITYID_INVALID) {
        return;
    }
    ui.chest_deposit_mode = !ui.chest_deposit_mode;
    ui.mini_inventory_scroll_offset = 0;
    ui.inventory_cursor = Vector2{0, 0};
    frame_dirty = true;
}

void gamestate::handle_chest_menu_confirm() {
    if (!ui.display_chest_menu || ui.active_chest_id == ENTITYID_INVALID) {
        return;
    }
    entityid source_id = ui.chest_deposit_mode ? hero_id : ui.active_chest_id;
    entityid target_id = ui.chest_deposit_mode ? ui.active_chest_id : hero_id;
    const auto* maybe_inventory = get_component<Inventory>(source_id);
    if (!maybe_inventory) {
        return;
    }
    auto& items = maybe_inventory->value;
    size_t index = get_inventory_selection_index();
    if (index >= items.size()) {
        return;
    }
    entityid item_id = items.at(index);
    if (run_chest_transfer_action(source_id, target_id, item_id)) {
        auto* updated_inventory = get_component<Inventory>(source_id);
        clamp_inventory_selection(updated_inventory ? updated_inventory->value.size() : 0);
    }
}

void gamestate::handle_input_chest(inputstate& is) {
    if (controlmode != controlmode_t::CHEST || !ui.display_chest_menu) {
        return;
    }
    if (inputstate_is_pressed(is, KEY_D) || inputstate_is_pressed(is, KEY_ESCAPE)) {
        close_chest_menu();
        return;
    }
    if (inputstate_is_pressed(is, KEY_TAB)) {
        toggle_chest_menu_mode();
        return;
    }
    if (inputstate_is_pressed(is, KEY_LEFT)) {
        if (use_mini_inventory_menu()) {
            move_inventory_selection(-1);
        }
        else if (ui.inventory_cursor.x > 0) {
            ui.inventory_cursor.x--;
        }
    }
    else if (inputstate_is_pressed(is, KEY_RIGHT)) {
        if (use_mini_inventory_menu()) {
            move_inventory_selection(1);
        }
        else {
            ui.inventory_cursor.x++;
        }
    }
    else if (inputstate_is_pressed(is, KEY_UP)) {
        if (use_mini_inventory_menu()) {
            move_inventory_selection(-1);
        }
        else if (ui.inventory_cursor.y > 0) {
            ui.inventory_cursor.y--;
        }
    }
    else if (inputstate_is_pressed(is, KEY_DOWN)) {
        if (use_mini_inventory_menu()) {
            move_inventory_selection(1);
        }
        else {
            ui.inventory_cursor.y++;
        }
    }
    else if (inputstate_is_pressed(is, KEY_ENTER)) {
        handle_chest_menu_confirm();
    }
    auto* items = get_component<Inventory>(ui.chest_deposit_mode ? hero_id : ui.active_chest_id);
    clamp_inventory_selection(items ? items->value.size() : 0);
    frame_dirty = true;
}
