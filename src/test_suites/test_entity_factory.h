#pragma once

#include "../ecs_gameplay_components.h"
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

        const entityid id = g.create_weapon_at_with(loc, g.dagger_init());

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS((g.get_component<EntityTypeTag>(id) ? g.get_component<EntityTypeTag>(id)->type : entitytype_t::NONE), entitytype_t::ITEM);
        TS_ASSERT_EQUALS(g.get_component_or<ItemSubtype>(id, itemtype_t::NONE), itemtype_t::WEAPON);
        TS_ASSERT_EQUALS(g.get_component_or<WeaponSubtype>(id, weapontype_t::NONE), weapontype_t::DAGGER);
        TS_ASSERT_EQUALS(g.get_component_or<EntityName>(id, std::string{""}), "dagger");
        TS_ASSERT_EQUALS(g.get_component_or<EntityDescription>(id, std::string{""}), "Stabby stabby.");
        TS_ASSERT(g.get_component_or<NeedsUpdate>(id, false));
        TS_ASSERT(g.get_component<Position>(id) != nullptr);
        TS_ASSERT(vec3_equal(g.get_component_or<Position>(id, vec3{-1, -1, -1}), loc));

        auto* damage_value = g.get_component<DamageDice>(id);
        TS_ASSERT(damage_value != nullptr);
        TS_ASSERT_EQUALS(damage_value->value.x, 1);
        TS_ASSERT_EQUALS(damage_value->value.y, 4);
        TS_ASSERT_EQUALS(damage_value->value.z, 0);

        tile_t& tile = g.d.get_floor(0)->tile_at(loc);
        TS_ASSERT_EQUALS(tile.get_cached_item(), id);
    }

    void testCreateShieldAtWithSetsExpectedShieldComponents() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_shield_at_with(loc, g.shield_init());

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS((g.get_component<EntityTypeTag>(id) ? g.get_component<EntityTypeTag>(id)->type : entitytype_t::NONE), entitytype_t::ITEM);
        TS_ASSERT_EQUALS(g.get_component_or<ItemSubtype>(id, itemtype_t::NONE), itemtype_t::SHIELD);
        TS_ASSERT_EQUALS(g.get_component_or<ShieldSubtype>(id, shieldtype_t::NONE), shieldtype_t::KITE);
        TS_ASSERT_EQUALS(g.get_component_or<EntityName>(id, std::string{""}), "kite shield");
        TS_ASSERT_EQUALS(g.get_component_or<EntityDescription>(id, std::string{""}), "Standard knight's shield");
        TS_ASSERT_EQUALS(g.get_component_or<BlockChance>(id, 0), 90);
        TS_ASSERT(!g.get_component<NeedsUpdate>(id)->value);
        TS_ASSERT(vec3_equal(g.get_component_or<Position>(id, vec3{-1, -1, -1}), loc));

        tile_t& tile = g.d.get_floor(0)->tile_at(loc);
        TS_ASSERT_EQUALS(tile.get_cached_item(), id);
    }

    void testCreatePotionAtWithSetsExpectedPotionComponents() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_potion_at_with(loc, g.potion_init(potiontype_t::HP_SMALL));

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS((g.get_component<EntityTypeTag>(id) ? g.get_component<EntityTypeTag>(id)->type : entitytype_t::NONE), entitytype_t::ITEM);
        TS_ASSERT_EQUALS(g.get_component_or<ItemSubtype>(id, itemtype_t::NONE), itemtype_t::POTION);
        TS_ASSERT_EQUALS(g.get_component_or<PotionSubtype>(id, potiontype_t::NONE), potiontype_t::HP_SMALL);
        TS_ASSERT_EQUALS(g.get_component_or<EntityName>(id, std::string{""}), "small healing potion");
        TS_ASSERT_EQUALS(g.get_component_or<EntityDescription>(id, std::string{""}), "a small healing potion");
        TS_ASSERT(g.get_component_or<NeedsUpdate>(id, false));
        TS_ASSERT(vec3_equal(g.get_component_or<Position>(id, vec3{-1, -1, -1}), loc));

        auto* healing_value = g.get_component<HealingDice>(id);
        TS_ASSERT(healing_value != nullptr);
        TS_ASSERT_EQUALS(healing_value->value.x, 1);
        TS_ASSERT_EQUALS(healing_value->value.y, 6);
        TS_ASSERT_EQUALS(healing_value->value.z, 0);

        tile_t& tile = g.d.get_floor(0)->tile_at(loc);
        TS_ASSERT_EQUALS(tile.get_cached_item(), id);
    }

    void testCreatePropAtWithSetsExpectedPropComponents() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_prop_at_with(proptype_t::DUNGEON_STATUE_00, loc, dungeon_prop_init(proptype_t::DUNGEON_STATUE_00));

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS((g.get_component<EntityTypeTag>(id) ? g.get_component<EntityTypeTag>(id)->type : entitytype_t::NONE), entitytype_t::PROP);
        TS_ASSERT_EQUALS(g.get_component_or<PropTypeComponent>(id, proptype_t::NONE), proptype_t::DUNGEON_STATUE_00);
        TS_ASSERT_EQUALS(g.get_component_or<EntityName>(id, std::string{""}), "statue");
        TS_ASSERT_EQUALS(g.get_component_or<EntityDescription>(id, std::string{""}), "A heavy carved statue worn smooth by years of damp air and passing hands.");
        TS_ASSERT(g.get_component_or<SolidTag>(id, false));
        TS_ASSERT(g.get_component_or<PushableTag>(id, false));
        TS_ASSERT(g.get_component_or<NeedsUpdate>(id, false));
        TS_ASSERT(vec3_equal(g.get_component_or<Position>(id, vec3{-1, -1, -1}), loc));

        tile_t& tile = g.d.get_floor(0)->tile_at(loc);
        TS_ASSERT_EQUALS(tile.get_cached_prop(), id);
    }

    void testCreateChestAtWithSetsExpectedChestComponents() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_chest_at_with(loc, [](gamestate&, const entityid) {});

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS((g.get_component<EntityTypeTag>(id) ? g.get_component<EntityTypeTag>(id)->type : entitytype_t::NONE), entitytype_t::CHEST);
        TS_ASSERT_EQUALS(g.get_component_or<EntityName>(id, std::string{""}), "treasure chest");
        TS_ASSERT_EQUALS(g.get_component_or<EntityDescription>(id, std::string{""}), "A stout treasure chest reinforced with iron bands and built to survive rough handling.");
        TS_ASSERT(g.get_component_or<PushableTag>(id, false));
        TS_ASSERT(g.get_component_or<PullableTag>(id, false));
        TS_ASSERT(g.get_component_or<SolidTag>(id, false));
        TS_ASSERT(!g.get_component<DoorOpenFlag>(id)->value);
        TS_ASSERT(g.get_component<Inventory>(id) != nullptr);
        TS_ASSERT_EQUALS(g.get_component<Inventory>(id)->value.size(), 0U);
        TS_ASSERT(vec3_equal(g.get_component_or<Position>(id, vec3{-1, -1, -1}), loc));

        tile_t& tile = g.d.get_floor(0)->tile_at(loc);
        TS_ASSERT_EQUALS(tile.get_cached_chest(), id);
    }

    void testCreateOrcAtWithSetsExpectedNpcComponents() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_orc_at_with(loc, [](gamestate&, const entityid) {});

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS((g.get_component<EntityTypeTag>(id) ? g.get_component<EntityTypeTag>(id)->type : entitytype_t::NONE), entitytype_t::NPC);
        TS_ASSERT_EQUALS((g.get_component<ActorKind>(id) ? g.get_component<ActorKind>(id)->race : race_t::NONE), race_t::ORC);
        TS_ASSERT_EQUALS(g.get_component_or<AlignmentComponent>(id, alignment_t::NONE), alignment_t::EVIL_CHAOTIC);
        TS_ASSERT(g.get_component<EntityName>(id) != nullptr);
        TS_ASSERT(!g.get_component_or<EntityName>(id, std::string{""}).empty());
        TS_ASSERT_EQUALS(g.get_component_or<DialogueLine>(id, std::string{""}), "They give you a guarded look but say nothing.");
        TS_ASSERT(!g.get_component_or<DeadFlag>(id, true));
        TS_ASSERT(g.get_component_or<AggroFlag>(id, false));
        TS_ASSERT(g.get_component_or<NeedsUpdate>(id, false));
        TS_ASSERT(g.get_component<Inventory>(id) != nullptr);
        TS_ASSERT_EQUALS(g.get_component<Inventory>(id)->value.size(), 0U);
        const vec2 hp_value = g.get_component_or<HitPoints>(id, vec2{0, 0});
        TS_ASSERT(hp_value.x >= 1);
        TS_ASSERT(hp_value.y >= 1);
        TS_ASSERT(vec3_equal(g.get_component_or<Position>(id, vec3{-1, -1, -1}), loc));

        tile_t& tile = g.d.get_floor(0)->tile_at(loc);
        TS_ASSERT_EQUALS(tile.get_cached_live_npc(), id);
    }

    void testCreateNpcAtWithUsesRaceDefaultsForNameAlignmentAndAggro() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_npc_at_with(race_t::DWARF, loc, [](gamestate&, const entityid) {});

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.get_component_or<EntityName>(id, std::string{""}), "dwarf");
        TS_ASSERT_EQUALS(g.get_component_or<DialogueLine>(id, std::string{""}), "They give you a guarded look but say nothing.");
        TS_ASSERT_EQUALS(g.get_component_or<AlignmentComponent>(id, alignment_t::NONE), alignment_t::GOOD_LAWFUL);
        TS_ASSERT(!g.get_component<AggroFlag>(id)->value);
    }

    void testCreateGreenSlimeUsesPassiveDefaultsAndRacialModifiers() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);

        const entityid id = g.create_npc_at_with(race_t::GREEN_SLIME, loc, [](gamestate&, const entityid) {});

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.get_component_or<EntityName>(id, std::string{""}), "green slime");
        TS_ASSERT_EQUALS(g.get_component_or<AlignmentComponent>(id, alignment_t::NONE), alignment_t::NEUTRAL_NEUTRAL);
        TS_ASSERT(!g.get_component<AggroFlag>(id)->value);
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
        TS_ASSERT_EQUALS((g.get_component<EntityTypeTag>(id) ? g.get_component<EntityTypeTag>(id)->type : entitytype_t::NONE), entitytype_t::PLAYER);
        TS_ASSERT_EQUALS(g.get_component_or<AlignmentComponent>(id, alignment_t::NONE), alignment_t::GOOD_LAWFUL);
    }

    void testApplyPermanentAttributeIncreaseCanBeUsedOutsideLevelUp() {
        gamestate g;
        const vec3 loc = add_initialized_floor(g);
        const entityid id = g.create_player_at_with(loc, "hero", g.player_init(12));

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        const int old_int = g.get_component_or<IntelligenceAttr>(id, 0);

        TS_ASSERT(g.apply_permanent_attribute_increase(id, 3, 2));
        TS_ASSERT_EQUALS(g.get_component_or<IntelligenceAttr>(id, 0), old_int + 2);
        TS_ASSERT_EQUALS(g.get_component_or<EntityLevel>(id, 0), 1);
    }

    void testCreateBoxWithSetsDefaultNameAndDescription() {
        gamestate g;

        const entityid id = g.create_box_with();

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.get_component_or<EntityName>(id, std::string{""}), "box");
        TS_ASSERT_EQUALS(g.get_component_or<EntityDescription>(id, std::string{""}), "A plain wooden box with rough handles cut into the sides for hauling.");
    }

    void testCreateDoorWithSetsDefaultNameAndDescription() {
        gamestate g;

        const entityid id = g.create_door_with([](gamestate&, const entityid) {});

        TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.get_component_or<EntityName>(id, std::string{""}), "door");
        TS_ASSERT_EQUALS(g.get_component_or<EntityDescription>(id, std::string{""}), "A heavy wooden door bound with iron straps and swollen from the dungeon damp.");
    }
};
