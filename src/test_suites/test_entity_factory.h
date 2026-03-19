#pragma once

#include "../gamestate.h"
#include <cxxtest/TestSuite.h>

class EntityFactoryTestSuite : public CxxTest::TestSuite {
private:
    vec3 add_initialized_floor(gamestate& g, int width = 8, int height = 8) {
        auto df = g.d.create_floor(biome_t::STONE, width, height);
        df->df_set_all_tiles(tiletype_t::FLOOR_STONE_00);
        g.d.add_floor(df);
        g.d.is_initialized = true;
        return vec3{1, 1, 0};
    }

public:
    void testAlignmentToStrReturnsExpectedNames() {
        TS_ASSERT_EQUALS(alignment_to_str(alignment_t::GOOD_LAWFUL), "lawful good");
        TS_ASSERT_EQUALS(alignment_to_str(alignment_t::NEUTRAL_NEUTRAL), "true neutral");
        TS_ASSERT_EQUALS(alignment_to_str(alignment_t::EVIL_CHAOTIC), "chaotic evil");
    }

    void testCreateWeaponAtWithSetsExpectedWeaponComponents() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_weapon_at_with(g.ct, loc, g.dagger_init());

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.ct.get<entitytype>(id).value_or(entitytype_t::NONE), entitytype_t::ITEM);
        TS_ASSERT_EQUALS(g.ct.get<itemtype>(id).value_or(itemtype_t::NONE), itemtype_t::WEAPON);
        TS_ASSERT_EQUALS(g.ct.get<weapontype>(id).value_or(weapontype_t::NONE), weapontype_t::DAGGER);
        TS_ASSERT_EQUALS(g.ct.get<name>(id).value_or(""), "dagger");
        TS_ASSERT_EQUALS(g.ct.get<description>(id).value_or(""), "Stabby stabby.");
        TS_ASSERT(g.ct.get<update>(id).value_or(false));
        TS_ASSERT(g.ct.get<location>(id).has_value());
        TS_ASSERT(vec3_equal(g.ct.get<location>(id).value_or(vec3{-1, -1, -1}), loc));

        auto damage_value = g.ct.get<damage>(id);
        TS_ASSERT(damage_value.has_value());
        TS_ASSERT_EQUALS(damage_value.value().x, 1);
        TS_ASSERT_EQUALS(damage_value.value().y, 4);
        TS_ASSERT_EQUALS(damage_value.value().z, 0);

        tile_t& tile = g.d.get_floor(0)->tile_at(loc);
        TS_ASSERT_EQUALS(tile.get_cached_item(), id);
    }

    void testCreateShieldAtWithSetsExpectedShieldComponents() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_shield_at_with(g.ct, loc, g.shield_init());

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.ct.get<entitytype>(id).value_or(entitytype_t::NONE), entitytype_t::ITEM);
        TS_ASSERT_EQUALS(g.ct.get<itemtype>(id).value_or(itemtype_t::NONE), itemtype_t::SHIELD);
        TS_ASSERT_EQUALS(g.ct.get<shieldtype>(id).value_or(shieldtype_t::NONE), shieldtype_t::KITE);
        TS_ASSERT_EQUALS(g.ct.get<name>(id).value_or(""), "kite shield");
        TS_ASSERT_EQUALS(g.ct.get<description>(id).value_or(""), "Standard knight's shield");
        TS_ASSERT_EQUALS(g.ct.get<block_chance>(id).value_or(0), 90);
        TS_ASSERT(!g.ct.get<update>(id).value_or(true));
        TS_ASSERT(vec3_equal(g.ct.get<location>(id).value_or(vec3{-1, -1, -1}), loc));

        tile_t& tile = g.d.get_floor(0)->tile_at(loc);
        TS_ASSERT_EQUALS(tile.get_cached_item(), id);
    }

    void testCreatePotionAtWithSetsExpectedPotionComponents() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_potion_at_with(loc, g.potion_init(potiontype_t::HP_SMALL));

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.ct.get<entitytype>(id).value_or(entitytype_t::NONE), entitytype_t::ITEM);
        TS_ASSERT_EQUALS(g.ct.get<itemtype>(id).value_or(itemtype_t::NONE), itemtype_t::POTION);
        TS_ASSERT_EQUALS(g.ct.get<potiontype>(id).value_or(potiontype_t::NONE), potiontype_t::HP_SMALL);
        TS_ASSERT_EQUALS(g.ct.get<name>(id).value_or(""), "small healing potion");
        TS_ASSERT_EQUALS(g.ct.get<description>(id).value_or(""), "a small healing potion");
        TS_ASSERT(g.ct.get<update>(id).value_or(false));
        TS_ASSERT(vec3_equal(g.ct.get<location>(id).value_or(vec3{-1, -1, -1}), loc));

        auto healing_value = g.ct.get<healing>(id);
        TS_ASSERT(healing_value.has_value());
        TS_ASSERT_EQUALS(healing_value.value().x, 1);
        TS_ASSERT_EQUALS(healing_value.value().y, 6);
        TS_ASSERT_EQUALS(healing_value.value().z, 0);

        tile_t& tile = g.d.get_floor(0)->tile_at(loc);
        TS_ASSERT_EQUALS(tile.get_cached_item(), id);
    }

    void testCreatePropAtWithSetsExpectedPropComponents() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_prop_at_with(proptype_t::DUNGEON_STATUE_00, loc, dungeon_prop_init(proptype_t::DUNGEON_STATUE_00));

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.ct.get<entitytype>(id).value_or(entitytype_t::NONE), entitytype_t::PROP);
        TS_ASSERT_EQUALS(g.ct.get<proptype>(id).value_or(proptype_t::NONE), proptype_t::DUNGEON_STATUE_00);
        TS_ASSERT_EQUALS(g.ct.get<name>(id).value_or(""), "statue");
        TS_ASSERT_EQUALS(g.ct.get<description>(id).value_or(""), "A heavy carved statue worn smooth by years of damp air and passing hands.");
        TS_ASSERT(g.ct.get<solid>(id).value_or(false));
        TS_ASSERT(g.ct.get<pushable>(id).value_or(false));
        TS_ASSERT(g.ct.get<update>(id).value_or(false));
        TS_ASSERT(vec3_equal(g.ct.get<location>(id).value_or(vec3{-1, -1, -1}), loc));

        tile_t& tile = g.d.get_floor(0)->tile_at(loc);
        TS_ASSERT_EQUALS(tile.get_cached_prop(), id);
    }

    void testCreateChestAtWithSetsExpectedChestComponents() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_chest_at_with(loc, [](CT&, const entityid) {});

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.ct.get<entitytype>(id).value_or(entitytype_t::NONE), entitytype_t::CHEST);
        TS_ASSERT_EQUALS(g.ct.get<name>(id).value_or(""), "treasure chest");
        TS_ASSERT_EQUALS(g.ct.get<description>(id).value_or(""), "A stout treasure chest reinforced with iron bands and built to survive rough handling.");
        TS_ASSERT(g.ct.get<pushable>(id).value_or(false));
        TS_ASSERT(g.ct.get<pullable>(id).value_or(false));
        TS_ASSERT(g.ct.get<solid>(id).value_or(false));
        TS_ASSERT(!g.ct.get<door_open>(id).value_or(true));
        TS_ASSERT(g.ct.get<inventory>(id).has_value());
        TS_ASSERT_EQUALS(g.ct.get<inventory>(id).value()->size(), 0U);
        TS_ASSERT(vec3_equal(g.ct.get<location>(id).value_or(vec3{-1, -1, -1}), loc));

        tile_t& tile = g.d.get_floor(0)->tile_at(loc);
        TS_ASSERT_EQUALS(tile.get_cached_chest(), id);
    }

    void testCreateOrcAtWithSetsExpectedNpcComponents() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_orc_at_with(loc, [](CT&, const entityid) {});

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.ct.get<entitytype>(id).value_or(entitytype_t::NONE), entitytype_t::NPC);
        TS_ASSERT_EQUALS(g.ct.get<race>(id).value_or(race_t::NONE), race_t::ORC);
        TS_ASSERT_EQUALS(g.ct.get<alignment>(id).value_or(alignment_t::NONE), alignment_t::EVIL_CHAOTIC);
        TS_ASSERT(g.ct.get<name>(id).has_value());
        TS_ASSERT(!g.ct.get<name>(id).value_or("").empty());
        TS_ASSERT_EQUALS(g.ct.get<dialogue_text>(id).value_or(""), "They give you a guarded look but say nothing.");
        TS_ASSERT(!g.ct.get<dead>(id).value_or(true));
        TS_ASSERT(g.ct.get<aggro>(id).value_or(false));
        TS_ASSERT(g.ct.get<update>(id).value_or(false));
        TS_ASSERT(g.ct.get<inventory>(id).has_value());
        TS_ASSERT_EQUALS(g.ct.get<inventory>(id).value()->size(), 0U);
        const vec2 hp_value = g.ct.get<hp>(id).value_or(vec2{0, 0});
        TS_ASSERT(hp_value.x >= 1);
        TS_ASSERT(hp_value.y >= 1);
        TS_ASSERT(vec3_equal(g.ct.get<location>(id).value_or(vec3{-1, -1, -1}), loc));

        tile_t& tile = g.d.get_floor(0)->tile_at(loc);
        TS_ASSERT_EQUALS(tile.get_cached_live_npc(), id);
    }

    void testCreateNpcAtWithUsesRaceDefaultsForNameAlignmentAndAggro() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_npc_at_with(race_t::DWARF, loc, [](CT&, const entityid) {});

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.ct.get<name>(id).value_or(""), "dwarf");
        TS_ASSERT_EQUALS(g.ct.get<dialogue_text>(id).value_or(""), "They give you a guarded look but say nothing.");
        TS_ASSERT_EQUALS(g.ct.get<alignment>(id).value_or(alignment_t::NONE), alignment_t::GOOD_LAWFUL);
        TS_ASSERT(!g.ct.get<aggro>(id).value_or(true));
    }

    void testCreateGreenSlimeUsesPassiveDefaultsAndRacialModifiers() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_npc_at_with(race_t::GREEN_SLIME, loc, [](CT&, const entityid) {});

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.ct.get<name>(id).value_or(""), "green slime");
        TS_ASSERT_EQUALS(g.ct.get<alignment>(id).value_or(alignment_t::NONE), alignment_t::NEUTRAL_NEUTRAL);
        TS_ASSERT(!g.ct.get<aggro>(id).value_or(true));
        TS_ASSERT_EQUALS(get_racial_modifiers(race_t::GREEN_SLIME, 0), -2);
        TS_ASSERT_EQUALS(get_racial_modifiers(race_t::GREEN_SLIME, 1), -2);
        TS_ASSERT_EQUALS(get_racial_modifiers(race_t::GREEN_SLIME, 2), -2);
        TS_ASSERT_EQUALS(get_racial_modifiers(race_t::GREEN_SLIME, 3), -2);
        TS_ASSERT_EQUALS(get_racial_modifiers(race_t::GREEN_SLIME, 4), -2);
        TS_ASSERT_EQUALS(get_racial_modifiers(race_t::GREEN_SLIME, 5), -2);
    }

    void testCreatePlayerAtWithUsesCharacterCreationAlignment() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);
        g.chara_creation.race = race_t::DWARF;
        g.chara_creation.alignment = alignment_t::GOOD_LAWFUL;

        const entityid id = g.create_player_at_with(loc, "hero", g.player_init(12));

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.ct.get<entitytype>(id).value_or(entitytype_t::NONE), entitytype_t::PLAYER);
        TS_ASSERT_EQUALS(g.ct.get<alignment>(id).value_or(alignment_t::NONE), alignment_t::GOOD_LAWFUL);
    }

    void testApplyPermanentAttributeIncreaseCanBeUsedOutsideLevelUp() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);
        const entityid id = g.create_player_at_with(loc, "hero", g.player_init(12));

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        const int old_int = g.ct.get<intelligence>(id).value_or(0);

        TS_ASSERT(g.apply_permanent_attribute_increase(id, 3, 2));
        TS_ASSERT_EQUALS(g.ct.get<intelligence>(id).value_or(0), old_int + 2);
        TS_ASSERT_EQUALS(g.ct.get<level>(id).value_or(0), 1);
    }

    void testCreateBoxWithSetsDefaultNameAndDescription() {
        gamestate g;

        const entityid id = g.create_box_with();

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.ct.get<name>(id).value_or(""), "box");
        TS_ASSERT_EQUALS(g.ct.get<description>(id).value_or(""), "A plain wooden box with rough handles cut into the sides for hauling.");
    }

    void testCreateDoorWithSetsDefaultNameAndDescription() {
        gamestate g;

        const entityid id = g.create_door_with([](CT&, const entityid) {});

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.ct.get<name>(id).value_or(""), "door");
        TS_ASSERT_EQUALS(g.ct.get<description>(id).value_or(""), "A heavy wooden door bound with iron straps and swollen from the dungeon damp.");
    }
};
