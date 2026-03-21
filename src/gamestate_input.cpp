#include "ecs_gameplay_components.h"
#include "gamestate.h"

/** @file gamestate_input.cpp
 *  @brief Input-handling, modal, and level-up helpers implemented on `gamestate`.
 */


void gamestate::open_confirm_prompt(rpg::confirm_action_t action, const char* fmt, ...) {
    massert(fmt, "format string is NULL");
    char buffer[MAX_MSG_LENGTH] = {0};
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, MAX_MSG_LENGTH - 1, fmt, args);
    va_end(args);
    ui.confirm_prompt_message = buffer;
    ui.confirm_action = action;
    ui.display_confirm_prompt = true;
    controlmode_before_confirm = controlmode;
    controlmode = controlmode_t::CONFIRM_PROMPT;
    frame_dirty = true;
}

void gamestate::handle_confirm_quit() {
#ifdef WEB
    restart_game();
#else
    do_quit = true;
#endif
    frame_dirty = true;
}

void gamestate::resolve_confirm_prompt(bool confirmed) {
    const rpg::confirm_action_t action = ui.confirm_action;
    ui.display_confirm_prompt = false;
    ui.confirm_action = rpg::confirm_action_t::NONE;
    ui.confirm_prompt_message.clear();
    controlmode = controlmode_before_confirm == controlmode_t::CONFIRM_PROMPT ? controlmode_t::PLAYER : controlmode_before_confirm;
    controlmode_before_confirm = controlmode_t::PLAYER;
    frame_dirty = true;
    if (!confirmed) {
        return;
    }
    switch (action) {
    case rpg::confirm_action_t::QUIT:
        handle_confirm_quit();
        break;
    case rpg::confirm_action_t::NONE:
    default:
        break;
    }
}

void gamestate::open_interaction_modal(entityid id, const string& title, const string& body) {
    ui.active_interaction_entity_id = id;
    ui.interaction_title = title;
    ui.interaction_body = body;
    ui.display_interaction_modal = true;
    controlmode_before_confirm = controlmode;
    controlmode = controlmode_t::INTERACTION;
    frame_dirty = true;
}

void gamestate::close_interaction_modal() {
    ui.display_interaction_modal = false;
    ui.active_interaction_entity_id = ENTITYID_INVALID;
    ui.interaction_title.clear();
    ui.interaction_body.clear();
    controlmode = controlmode_before_confirm == controlmode_t::INTERACTION ? controlmode_t::PLAYER : controlmode_before_confirm;
    controlmode_before_confirm = controlmode_t::PLAYER;
    frame_dirty = true;
}

void gamestate::handle_input_interaction(inputstate& is) {
    if (controlmode != controlmode_t::INTERACTION || !ui.display_interaction_modal) {
        return;
    }
    if (inputstate_any_pressed(is)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        close_interaction_modal();
    }
}

int gamestate::get_level_up_xp_threshold(entityid id) {
    const int actor_level = get_component_or<EntityLevel>(id, 1) < 1 ? 1 : get_component_or<EntityLevel>(id, 1);
    return actor_level * 10;
}

void gamestate::open_level_up_modal() {
    ui.display_level_up_modal = true;
    ui.level_up_selection = 0;
    controlmode = controlmode_t::LEVEL_UP;
    frame_dirty = true;
}

bool gamestate::apply_permanent_attribute_increase(entityid id, unsigned int attribute_index, int amount) {
    if (id == ENTITYID_INVALID || amount == 0) {
        return false;
    }

    switch (attribute_index % 6) {
    case 0:
        ct.set<strength>(id, get_component_or<StrengthAttr>(id, 10) + amount);
        return true;
    case 1:
        ct.set<dexterity>(id, get_component_or<DexterityAttr>(id, 10) + amount);
        return true;
    case 2:
        ct.set<constitution>(id, get_component_or<ConstitutionAttr>(id, 10) + amount);
        return true;
    case 3:
        ct.set<intelligence>(id, get_component_or<IntelligenceAttr>(id, 10) + amount);
        return true;
    case 4:
        ct.set<wisdom>(id, get_component_or<WisdomAttr>(id, 10) + amount);
        return true;
    case 5:
        ct.set<charisma>(id, get_component_or<CharismaAttr>(id, 10) + amount);
        return true;
    default:
        break;
    }
    return false;
}

int gamestate::roll_level_up_max_hp_gain(entityid id) {
    if (id == ENTITYID_INVALID) {
        return 0;
    }
    const vec3 hitdie = get_component_or<HitDie>(id, vec3{1, 1, 0});
    const int rolled = do_roll(hitdie);
    return rolled > 0 ? rolled : 1;
}

void gamestate::apply_level_up_rewards(entityid id) {
    if (id == ENTITYID_INVALID) {
        return;
    }

    const int new_level = get_component_or<EntityLevel>(id, 1) + 1;
    ct.set<level>(id, new_level);

    vec2 hp_value = get_component_or<HitPoints>(id, vec2{1, 1});
    const int hp_gain = roll_level_up_max_hp_gain(id);
    hp_value.y += hp_gain;
    if (hp_value.y < 1) {
        hp_value.y = 1;
    }
    if (hp_value.x > hp_value.y) {
        hp_value.x = hp_value.y;
    }
    ct.set<hp>(id, hp_value);
}

void gamestate::apply_level_up_selection() {
    if (hero_id == ENTITYID_INVALID || !ui.display_level_up_modal) {
        return;
    }

    const char* stat_name = "strength";
    switch (ui.level_up_selection % 6) {
    case 0:
        stat_name = "strength";
        break;
    case 1:
        stat_name = "dexterity";
        break;
    case 2:
        stat_name = "constitution";
        break;
    case 3:
        stat_name = "intelligence";
        break;
    case 4:
        stat_name = "wisdom";
        break;
    case 5:
        stat_name = "charisma";
        break;
    default:
        break;
    }

    if (!apply_permanent_attribute_increase(hero_id, ui.level_up_selection, 1)) {
        return;
    }
    apply_level_up_rewards(hero_id);
    ui.pending_level_ups = ui.pending_level_ups > 0 ? ui.pending_level_ups - 1 : 0;
    ui.display_level_up_modal = false;
    controlmode = controlmode_t::PLAYER;
    messages.add("%s increased by 1", stat_name);
    messages.add_history("%s reached level %d", get_component_or<EntityName>(hero_id, "hero").c_str(), get_component_or<EntityLevel>(hero_id, 1));
    frame_dirty = true;

    if (ui.pending_level_ups > 0) {
        open_level_up_modal();
    }
}

void gamestate::maybe_unlock_level_up(entityid id) {
    if (id != hero_id || (get_component<EntityTypeTag>(id) ? get_component<EntityTypeTag>(id)->type : entitytype_t::NONE) != entitytype_t::PLAYER) {
        return;
    }

    const int current_xp = get_component_or<Experience>(id, 0);
    int current_level = get_component_or<EntityLevel>(id, 1) + ui.pending_level_ups;
    if (current_level < 1) {
        current_level = 1;
    }
    while (current_xp >= current_level * 10) {
        ui.pending_level_ups++;
        current_level++;
    }

    if (ui.pending_level_ups > 0 && !ui.display_level_up_modal) {
        open_level_up_modal();
    }
}

void gamestate::handle_input_level_up(inputstate& is) {
    if (controlmode != controlmode_t::LEVEL_UP || !ui.display_level_up_modal) {
        return;
    }
    if (inputstate_is_pressed(is, KEY_LEFT)) {
        if (ui.level_up_selection % 2 == 1) {
            ui.level_up_selection--;
        }
        frame_dirty = true;
    }
    else if (inputstate_is_pressed(is, KEY_RIGHT)) {
        if (ui.level_up_selection % 2 == 0) {
            ui.level_up_selection++;
        }
        frame_dirty = true;
    }
    else if (inputstate_is_pressed(is, KEY_UP)) {
        if (ui.level_up_selection >= 2) {
            ui.level_up_selection -= 2;
        }
        frame_dirty = true;
    }
    else if (inputstate_is_pressed(is, KEY_DOWN)) {
        if (ui.level_up_selection + 2 < 6) {
            ui.level_up_selection += 2;
        }
        frame_dirty = true;
    }
    else if (inputstate_is_pressed(is, KEY_ENTER)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        apply_level_up_selection();
    }
}

void gamestate::handle_input_confirm_prompt(inputstate& is) {
    if (controlmode != controlmode_t::CONFIRM_PROMPT) {
        controlmode = controlmode_t::CONFIRM_PROMPT;
    }
    if (inputstate_is_pressed(is, KEY_Y)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        resolve_confirm_prompt(true);
    }
    else if (inputstate_is_pressed(is, KEY_N) || inputstate_is_pressed(is, KEY_ESCAPE)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        resolve_confirm_prompt(false);
    }
}

void gamestate::handle_camera_move(inputstate& is) {
    if (inputstate_is_held(is, KEY_RIGHT)) {
        cam2d.offset.x += cam2d.zoom;
        frame_dirty = true;
    }
    else if (inputstate_is_held(is, KEY_LEFT)) {
        cam2d.offset.x -= cam2d.zoom;
        frame_dirty = true;
    }
    else if (inputstate_is_held(is, KEY_UP)) {
        cam2d.offset.y -= cam2d.zoom;
        frame_dirty = true;
    }
    else if (inputstate_is_held(is, KEY_DOWN)) {
        cam2d.offset.y += cam2d.zoom;
        frame_dirty = true;
    }
}

void gamestate::handle_input_inventory(inputstate& is) {
    if (controlmode != controlmode_t::INVENTORY || !ui.display_inventory_menu) {
        return;
    }
    if (inputstate_is_pressed(is, KEY_I) || inputstate_is_pressed(is, KEY_ESCAPE)) {
        controlmode = controlmode_t::PLAYER;
        ui.display_inventory_menu = false;
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        return;
    }
    if (inputstate_is_pressed(is, KEY_LEFT)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        if (use_mini_inventory_menu()) {
            move_inventory_selection(-1);
        }
        else if (ui.inventory_cursor.x > 0) {
            ui.inventory_cursor.x--;
        }
    }
    else if (inputstate_is_pressed(is, KEY_RIGHT)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        if (use_mini_inventory_menu()) {
            move_inventory_selection(1);
        }
        else {
            ui.inventory_cursor.x++;
        }
    }
    else if (inputstate_is_pressed(is, KEY_UP)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        if (use_mini_inventory_menu()) {
            move_inventory_selection(-1);
        }
        else if (ui.inventory_cursor.y > 0) {
            ui.inventory_cursor.y--;
        }
    }
    else if (inputstate_is_pressed(is, KEY_DOWN)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        if (use_mini_inventory_menu()) {
            move_inventory_selection(1);
        }
        else {
            ui.inventory_cursor.y++;
        }
    }
    else if (inputstate_is_pressed(is, KEY_E)) {
        handle_hero_inventory_equip();
    }
    else if (inputstate_is_pressed(is, KEY_Q)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/27_sword_miss_1.wav");
        size_t index = get_inventory_selection_index();
        auto* my_items = get_component<Inventory>(hero_id);
        if (my_items && index < my_items->value.size()) {
            run_drop_item_action(hero_id, my_items->value.at(index));
        }
    }
    else if (inputstate_is_pressed(is, KEY_ENTER)) {
        handle_hero_item_use();
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
    }
    auto* items = get_component<Inventory>(hero_id);
    clamp_inventory_selection(items ? items->value.size() : 0);
    frame_dirty = true;
}

bool gamestate::handle_cycle_messages(inputstate& is) {
    if (messages.is_active && inputstate_is_pressed(is, KEY_ENTER)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        messages.cycle();
        return true;
    }
    return false;
}

bool gamestate::handle_cycle_messages_test() {
    if (!messages.is_active) {
        return false;
    }
    audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
    messages.cycle();
    return true;
}

void gamestate::handle_camera_zoom([[maybe_unused]] inputstate& is) {
    // Zoom disabled — rendering at fixed DEFAULT_ZOOM_LEVEL for uniform scaling
}

void gamestate::change_player_dir(direction_t dir) {
    if (get_component_or<DeadFlag>(hero_id, true)) {
        return;
    }
    ct.set<direction>(hero_id, dir);
    ct.set<update>(hero_id, true);
    player_changing_dir = false;
    frame_dirty = true;
}

bool gamestate::handle_change_dir(inputstate& is) {
    if (!player_changing_dir) {
        return false;
    }
    const DeadFlag* maybe_player_is_dead = get_component<DeadFlag>(hero_id);
    if (!maybe_player_is_dead) {
        return true;
    }
    bool is_dead = maybe_player_is_dead->value;
    if (is_action_pressed(is, gameplay_input_action_t::FACE_WAIT)) {
        player_changing_dir = false;
        flag = gamestate_flag_t::PLAYER_ANIM;
    }
    else if (is_action_pressed(is, gameplay_input_action_t::FACE_UP)) {
        change_player_dir(direction_t::UP);
    }
    else if (is_action_pressed(is, gameplay_input_action_t::FACE_DOWN)) {
        change_player_dir(direction_t::DOWN);
    }
    else if (is_action_pressed(is, gameplay_input_action_t::FACE_LEFT)) {
        change_player_dir(direction_t::LEFT);
    }
    else if (is_action_pressed(is, gameplay_input_action_t::FACE_RIGHT)) {
        change_player_dir(direction_t::RIGHT);
    }
    else if (is_action_pressed(is, gameplay_input_action_t::FACE_UP_LEFT)) {
        change_player_dir(direction_t::UP_LEFT);
    }
    else if (is_action_pressed(is, gameplay_input_action_t::FACE_UP_RIGHT)) {
        change_player_dir(direction_t::UP_RIGHT);
    }
    else if (is_action_pressed(is, gameplay_input_action_t::FACE_DOWN_LEFT)) {
        change_player_dir(direction_t::DOWN_LEFT);
    }
    else if (is_action_pressed(is, gameplay_input_action_t::FACE_DOWN_RIGHT)) {
        change_player_dir(direction_t::DOWN_RIGHT);
    }
    else if (is_action_pressed(is, gameplay_input_action_t::FACE_ATTACK)) {
        if (is_dead) {
            return messages.add("You cannot attack while dead");
        }
        ct.set<attacking>(hero_id, true);
        ct.set<update>(hero_id, true);
        flag = gamestate_flag_t::PLAYER_ANIM;
        player_changing_dir = false;
    }
    return true;
}

bool gamestate::handle_change_dir_intent(inputstate& is) {
    if (is_action_pressed(is, gameplay_input_action_t::DIRECTION_MODE)) {
        player_changing_dir = true;
        return true;
    }
    return false;
}

bool gamestate::handle_display_inventory(inputstate& is) {
    if (is_action_pressed(is, gameplay_input_action_t::INVENTORY)) {
        ui.display_inventory_menu = true;
        controlmode = controlmode_t::INVENTORY;
        ui.mini_inventory_scroll_offset = 0;
        ui.inventory_cursor = Vector2{0, 0};
        frame_dirty = true;
        audio.queue("sfx/Minifantasy_Dungeon_SFX/04_sack_open_2.wav");
        return true;
    }
    return false;
}

bool gamestate::handle_toggle_full_light(inputstate& is) {
    if (!is_action_pressed(is, gameplay_input_action_t::TOGGLE_FULL_LIGHT)) {
        return false;
    }
    shared_ptr<dungeon_floor> df = d.get_current_floor();
    massert(df, "current floor is null");
    df->toggle_full_light();
    messages.add_history("Floor %d full-light %s", df->get_floor(), df->get_full_light() ? "enabled" : "disabled");
    frame_dirty = true;
    return true;
}

bool gamestate::handle_restart(inputstate& is) {
    if (is_action_pressed(is, gameplay_input_action_t::RESTART)) {
        do_restart = true;
        return true;
    }
    return false;
}

void gamestate::handle_input_gameplay_controlmode_player(inputstate& is) {
    if (flag != gamestate_flag_t::PLAYER_INPUT) {
        return;
    }
    if (inputstate_is_pressed(is, KEY_ESCAPE)) {
        open_confirm_prompt(rpg::confirm_action_t::QUIT, "Do You Want To Exit? Press Y or N");
        return;
    }
    handle_camera_zoom(is);
    if (handle_cycle_messages(is)) {
        return;
    }
    if (is_action_pressed(is, gameplay_input_action_t::HELP)) {
        ui.display_help_menu = true;
        controlmode = controlmode_t::HELP_MENU;
    }
    bool is_dead = get_component_or<DeadFlag>(hero_id, true);
    if (is_dead) {
        handle_restart(is);
        return;
    }
    if (test && framecount % 20 == 0) {
        if (handle_cycle_messages_test()) {
            return;
        }
        bool test_is_dead = get_component_or<DeadFlag>(hero_id, true);
        if (test_is_dead) {
            return;
        }
        uniform_int_distribution<int> dist(-1, 1);
        int vx = dist(random.mt);
        int vy = dist(random.mt);
        while (vx == 0 && vy == 0) {
            vx = dist(random.mt);
            vy = dist(random.mt);
        }
        if (run_move_action(hero_id, vec3{vx, vy, 0})) {
            msuccess2("hero moved randomly successfully");
        }
        else {
            minfo2("hero failed to move randomly");
        }
        flag = gamestate_flag_t::PLAYER_ANIM;
        return;
    }
    else if (test) {
        return;
    }
    else if (is_action_pressed(is, gameplay_input_action_t::OPTIONS)) {
        ui.display_option_menu = true;
        controlmode = controlmode_t::OPTION_MENU;
        return;
    }
    else if (is_action_pressed(is, gameplay_input_action_t::PULL)) {
        run_pull_action(hero_id);
        flag = gamestate_flag_t::PLAYER_ANIM;
        return;
    }
    if (handle_toggle_full_light(is) || handle_change_dir(is) || handle_change_dir_intent(is) || handle_display_inventory(is)) {
        return;
    }
    else if (handle_move_up(is, is_dead) || handle_move_down(is, is_dead) || handle_move_left(is, is_dead) || handle_move_right(is, is_dead)) {
        return;
    }
    else if (
        handle_move_up_left(is, is_dead) || handle_move_up_right(is, is_dead) || handle_move_down_left(is, is_dead) ||
        handle_move_down_right(is, is_dead)) {
        return;
    }
    else if (
        handle_attack(is, is_dead) || handle_pickup_item(is, is_dead) || handle_traverse_stairs(is, is_dead) || handle_open_door(is, is_dead) ||
        handle_interact(is, is_dead)) {
        return;
    }
}

void gamestate::handle_input_action_menu(inputstate& is) {
    massert(controlmode == controlmode_t::ACTION_MENU, "controlmode isnt in action menu: %d", controlmode);
    if (inputstate_is_pressed(is, KEY_SPACE)) {
        ui.display_action_menu = false;
        controlmode = controlmode_t::PLAYER;
        return;
    }
    else if (inputstate_is_pressed(is, KEY_DOWN)) {
        ui.action_selection++;
    }
    else if (inputstate_is_pressed(is, KEY_UP)) {
        ui.action_selection--;
    }
}

void gamestate::handle_input_option_menu(inputstate& is) {
    massert(controlmode == controlmode_t::OPTION_MENU, "controlmode isnt in option menu: %d", controlmode);
    if (inputstate_is_pressed(is, KEY_GRAVE) || inputstate_is_pressed(is, KEY_ESCAPE)) {
        ui.display_option_menu = false;
        controlmode = controlmode_t::PLAYER;
        frame_dirty = true;
    }
    else if (inputstate_is_pressed(is, KEY_UP)) {
        ui.options_menu.decr_selection();
        frame_dirty = true;
    }
    else if (inputstate_is_pressed(is, KEY_DOWN)) {
        ui.options_menu.incr_selection();
        frame_dirty = true;
    }
    else if (inputstate_is_pressed(is, KEY_ENTER)) {
        const option_type selected = ui.options_menu.get_option(ui.options_menu.get_selection());
        if (selected == option_type::SOUND) {
            open_sound_menu();
        }
        else if (selected == option_type::WINDOW_BOXES) {
            open_window_color_menu();
        }
        else if (selected == option_type::CONTROLS) {
            open_controls_menu();
        }
        else if (selected == option_type::INVENTORY_MENU) {
            ui.prefer_mini_inventory_menu = !ui.prefer_mini_inventory_menu;
            ui.mini_inventory_scroll_offset = 0;
            ui.inventory_cursor = Vector2{0, 0};
            frame_dirty = true;
        }
    }
}

void gamestate::handle_input_sound_menu(inputstate& is) {
    if (controlmode != controlmode_t::SOUND_MENU || !ui.display_sound_menu) {
        return;
    }

    constexpr size_t option_count = 3;
    if (inputstate_is_pressed(is, KEY_ESCAPE) || inputstate_is_pressed(is, KEY_GRAVE)) {
        close_sound_menu();
        return;
    }
    if (inputstate_is_pressed(is, KEY_UP)) {
        ui.sound_menu_selection = ui.sound_menu_selection == 0 ? option_count - 1 : ui.sound_menu_selection - 1;
        frame_dirty = true;
        return;
    }
    if (inputstate_is_pressed(is, KEY_DOWN)) {
        ui.sound_menu_selection = (ui.sound_menu_selection + 1) % option_count;
        frame_dirty = true;
        return;
    }
    if (inputstate_is_pressed(is, KEY_LEFT)) {
        if (ui.sound_menu_selection == 0) {
            audio.adjust_master(-1);
        }
        else if (ui.sound_menu_selection == 1) {
            audio.adjust_music(-1);
        }
        else if (ui.sound_menu_selection == 2) {
            audio.adjust_sfx(-1);
        }
        frame_dirty = true;
        return;
    }
    if (inputstate_is_pressed(is, KEY_RIGHT)) {
        if (ui.sound_menu_selection == 0) {
            audio.adjust_master(1);
        }
        else if (ui.sound_menu_selection == 1) {
            audio.adjust_music(1);
        }
        else if (ui.sound_menu_selection == 2) {
            audio.adjust_sfx(1);
        }
        frame_dirty = true;
        return;
    }
}

void gamestate::handle_input_window_color_menu(inputstate& is) {
    if (controlmode != controlmode_t::WINDOW_COLOR_MENU || !ui.display_window_color_menu) {
        return;
    }

    constexpr size_t option_count = 9;
    if (inputstate_is_pressed(is, KEY_ESCAPE) || inputstate_is_pressed(is, KEY_GRAVE)) {
        close_window_color_menu();
        return;
    }
    if (inputstate_is_pressed(is, KEY_UP)) {
        ui.window_color_menu_selection = ui.window_color_menu_selection == 0 ? option_count - 1 : ui.window_color_menu_selection - 1;
        frame_dirty = true;
        return;
    }
    if (inputstate_is_pressed(is, KEY_DOWN)) {
        ui.window_color_menu_selection = (ui.window_color_menu_selection + 1) % option_count;
        frame_dirty = true;
        return;
    }
    if (inputstate_is_pressed(is, KEY_ENTER) && ui.window_color_menu_selection == option_count - 1) {
        reset_window_box_colors();
        return;
    }
    const bool adjust_left = inputstate_is_held(is, KEY_LEFT);
    const bool adjust_right = inputstate_is_held(is, KEY_RIGHT);
    if (!adjust_left && !adjust_right) {
        return;
    }

    const int dir = adjust_right ? 1 : -1;
    switch (ui.window_color_menu_selection) {
    case 0: adjust_window_box_bg_channel(0, dir); return;
    case 1: adjust_window_box_bg_channel(1, dir); return;
    case 2: adjust_window_box_bg_channel(2, dir); return;
    case 3: adjust_window_box_bg_channel(3, dir); return;
    case 4: adjust_window_box_fg_channel(0, dir); return;
    case 5: adjust_window_box_fg_channel(1, dir); return;
    case 6: adjust_window_box_fg_channel(2, dir); return;
    case 7: adjust_window_box_fg_channel(3, dir); return;
    default: break;
    }
}

void gamestate::handle_input_help_menu(inputstate& is) {
    massert(controlmode == controlmode_t::HELP_MENU, "controlmode isnt in help menu: %d", controlmode);
    if (inputstate_any_pressed(is)) {
        ui.display_help_menu = false;
        controlmode = controlmode_t::PLAYER;
    }
}

void gamestate::handle_input_gameplay_scene(inputstate& is) {
    minfo2("handle input gameplay scene");
    if (controlmode == controlmode_t::KEYBOARD_PROFILE) {
        handle_input_keyboard_profile_prompt(is);
        return;
    }
    if (controlmode == controlmode_t::LEVEL_UP) {
        handle_input_level_up(is);
        return;
    }
    if (controlmode == controlmode_t::CONFIRM_PROMPT) {
        handle_input_confirm_prompt(is);
        return;
    }
    if (controlmode == controlmode_t::INTERACTION) {
        handle_input_interaction(is);
        return;
    }
    if (controlmode == controlmode_t::SOUND_MENU) {
        handle_input_sound_menu(is);
        return;
    }
    if (controlmode == controlmode_t::WINDOW_COLOR_MENU) {
        handle_input_window_color_menu(is);
        return;
    }
    if (controlmode == controlmode_t::CONTROLS_MENU) {
        handle_input_controls_menu(is);
        return;
    }
    if (is_action_pressed(is, gameplay_input_action_t::CAMERA_TOGGLE)) {
        if (controlmode == controlmode_t::PLAYER) {
            controlmode = controlmode_t::CAMERA;
        }
        else if (controlmode == controlmode_t::CAMERA) {
            controlmode = controlmode_t::PLAYER;
        }
        frame_dirty = true;
    }

    if (controlmode == controlmode_t::CAMERA) {
        handle_camera_move(is);
    }
    else if (controlmode == controlmode_t::PLAYER) {
        handle_input_gameplay_controlmode_player(is);
    }
    else if (controlmode == controlmode_t::INVENTORY) {
        handle_input_inventory(is);
    }
    else if (controlmode == controlmode_t::CHEST) {
        handle_input_chest(is);
    }
    else if (controlmode == controlmode_t::ACTION_MENU) {
        handle_input_action_menu(is);
    }
    else if (controlmode == controlmode_t::OPTION_MENU) {
        handle_input_option_menu(is);
    }
    else if (controlmode == controlmode_t::HELP_MENU) {
        handle_input_help_menu(is);
    }
    else if (controlmode == controlmode_t::LEVEL_UP) {
        handle_input_level_up(is);
    }
    else if (controlmode == controlmode_t::INTERACTION) {
        handle_input_interaction(is);
    }
}

void gamestate::handle_input(inputstate& is) {
    minfo2("handle input: current_scene: %d", current_scene);
    if (inputstate_is_pressed(is, KEY_P)) {
        debugpanelon = !debugpanelon;
        minfo2("Toggling debug panel: %s", debugpanelon ? "ON" : "OFF");
    }
    if (ui.display_confirm_prompt || controlmode == controlmode_t::CONFIRM_PROMPT) {
        handle_input_confirm_prompt(is);
        return;
    }
    if (current_scene == scene_t::TITLE) {
        handle_input_title_scene(is);
    }
    else if (current_scene == scene_t::MAIN_MENU) {
        handle_input_main_menu_scene(is);
    }
    else if (current_scene == scene_t::CHARACTER_CREATION) {
        handle_input_character_creation_scene(is);
    }
    else if (current_scene == scene_t::GAMEPLAY) {
        handle_input_gameplay_scene(is);
    }
}
