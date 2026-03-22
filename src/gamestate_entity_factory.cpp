#include "gamestate.h"

#include "ecs_actor_components.h"
#include "ecs_gameplay_components.h"
#include "ecs_item_components.h"
#include "actor_definitions.h"
#include "entities/actor.h"
#include "entities/box.h"
#include "entities/item.h"
#include "item_definitions.h"

namespace {
void mirror_item_common(gamestate& g, entityid id, const ItemDefinition* definition) {
    const entt::entity registry_entity = g.ensure_registry_entity(id);
    if (definition == nullptr) {
        return;
    }

    g.registry.emplace_or_replace<ItemVisual>(registry_entity, ItemVisual{definition->sprites, definition->sprite_count});
    g.registry.emplace_or_replace<ItemText>(registry_entity, ItemText{definition->name, definition->description});
}

void mirror_weapon_item(gamestate& g, entityid id, weapontype_t type) {
    const ItemDefinition* definition = find_weapon_definition(type);
    mirror_item_common(g, id, definition);

    const entt::entity registry_entity = g.ensure_registry_entity(id);
    g.registry.emplace_or_replace<ItemKind>(registry_entity, ItemKind{itemtype_t::WEAPON});
    g.registry.emplace_or_replace<WeaponKind>(registry_entity, WeaponKind{type});
}

void mirror_shield_item(gamestate& g, entityid id, shieldtype_t type) {
    const ItemDefinition* definition = find_shield_definition(type);
    mirror_item_common(g, id, definition);

    const entt::entity registry_entity = g.ensure_registry_entity(id);
    g.registry.emplace_or_replace<ItemKind>(registry_entity, ItemKind{itemtype_t::SHIELD});
    g.registry.emplace_or_replace<ShieldKind>(registry_entity, ShieldKind{type});
}

void mirror_potion_item(gamestate& g, entityid id, potiontype_t type) {
    const ItemDefinition* definition = find_potion_definition(type);
    mirror_item_common(g, id, definition);

    const entt::entity registry_entity = g.ensure_registry_entity(id);
    g.registry.emplace_or_replace<ItemKind>(registry_entity, ItemKind{itemtype_t::POTION});
    g.registry.emplace_or_replace<PotionKind>(registry_entity, PotionKind{type});
}
}

/** @file gamestate_entity_factory.cpp
 *  @brief Entity/item/NPC factory helpers implemented on `gamestate`.
 */


entityid gamestate::create_weapon_with(with_fun weaponInitFunction) {
    entityid id = rpg::entities::Item::create_weapon(*this);
    weaponInitFunction(*this, id);
    mirror_weapon_item(*this, id, get_component_or<WeaponSubtype>(id, weapontype_t::NONE));
    return id;
}

with_fun gamestate::dagger_init() {
    return [](gamestate& g, const entityid id) {
        auto e = g.ensure_registry_entity(id);
        g.registry.emplace_or_replace<EntityName>(e, EntityName{"dagger"});
        g.registry.emplace_or_replace<EntityDescription>(e, EntityDescription{"Stabby stabby."});
        g.registry.emplace_or_replace<WeaponSubtype>(e, WeaponSubtype{weapontype_t::DAGGER});
        g.registry.emplace_or_replace<DamageDice>(e, DamageDice{vec3{1, 4, 0}});
        g.registry.emplace_or_replace<ItemDurability>(e, ItemDurability{100});
        g.registry.emplace_or_replace<ItemMaxDurability>(e, ItemMaxDurability{100});
    };
}

with_fun gamestate::axe_init() {
    return [](gamestate& g, const entityid id) {
        auto e = g.ensure_registry_entity(id);
        g.registry.emplace_or_replace<EntityName>(e, EntityName{"axe"});
        g.registry.emplace_or_replace<EntityDescription>(e, EntityDescription{"Choppy choppy"});
        g.registry.emplace_or_replace<WeaponSubtype>(e, WeaponSubtype{weapontype_t::AXE});
        g.registry.emplace_or_replace<DamageDice>(e, DamageDice{vec3{1, 8, 0}});
        g.registry.emplace_or_replace<ItemDurability>(e, ItemDurability{100});
        g.registry.emplace_or_replace<ItemMaxDurability>(e, ItemMaxDurability{100});
    };
}

with_fun gamestate::sword_init() {
    return [](gamestate& g, const entityid id) {
        auto e = g.ensure_registry_entity(id);
        g.registry.emplace_or_replace<EntityName>(e, EntityName{"short sword"});
        g.registry.emplace_or_replace<EntityDescription>(e, EntityDescription{"your basic soldier's short sword"});
        g.registry.emplace_or_replace<WeaponSubtype>(e, WeaponSubtype{weapontype_t::SHORT_SWORD});
        g.registry.emplace_or_replace<DamageDice>(e, DamageDice{vec3{1, 6, 0}});
        g.registry.emplace_or_replace<ItemDurability>(e, ItemDurability{100});
        g.registry.emplace_or_replace<ItemMaxDurability>(e, ItemMaxDurability{100});
    };
}

with_fun gamestate::shield_init() {
    return [](gamestate& g, const entityid id) {
        auto e = g.ensure_registry_entity(id);
        g.registry.emplace_or_replace<EntityName>(e, EntityName{"kite shield"});
        g.registry.emplace_or_replace<EntityDescription>(e, EntityDescription{"Standard knight's shield"});
        g.registry.emplace_or_replace<ShieldSubtype>(e, ShieldSubtype{shieldtype_t::KITE});
        g.registry.emplace_or_replace<BlockChance>(e, BlockChance{90});
        g.registry.emplace_or_replace<ItemDurability>(e, ItemDurability{100});
        g.registry.emplace_or_replace<ItemMaxDurability>(e, ItemMaxDurability{100});
    };
}

with_fun gamestate::potion_init(potiontype_t pt) {
    return [pt](gamestate& g, const entityid id) {
        g.registry.emplace_or_replace<PotionSubtype>(g.ensure_registry_entity(id), PotionSubtype{pt});
        if (pt == potiontype_t::HP_SMALL) {
            auto e = g.ensure_registry_entity(id);
            g.registry.emplace_or_replace<EntityName>(e, EntityName{"small healing potion"});
            g.registry.emplace_or_replace<EntityDescription>(e, EntityDescription{"a small healing potion"});
            g.registry.emplace_or_replace<HealingDice>(e, HealingDice{vec3{1, 6, 0}});
        }
    };
}

with_fun gamestate::player_init(int maxhp_roll) {
    return [this, maxhp_roll](gamestate& g, const entityid id) {
        auto e = g.ensure_registry_entity(id);
        g.registry.emplace_or_replace<StrengthAttr>(e, StrengthAttr{chara_creation.strength});
        g.registry.emplace_or_replace<DexterityAttr>(e, DexterityAttr{chara_creation.dexterity});
        g.registry.emplace_or_replace<ConstitutionAttr>(e, ConstitutionAttr{chara_creation.constitution});
        g.registry.emplace_or_replace<IntelligenceAttr>(e, IntelligenceAttr{chara_creation.intelligence});
        g.registry.emplace_or_replace<WisdomAttr>(e, WisdomAttr{chara_creation.wisdom});
        g.registry.emplace_or_replace<CharismaAttr>(e, CharismaAttr{chara_creation.charisma});
        g.registry.emplace_or_replace<HitDie>(e, HitDie{vec3{1, chara_creation.hitdie, 0}});
        g.registry.emplace_or_replace<HitPoints>(e, HitPoints{vec2{maxhp_roll, maxhp_roll}});
        g.registry.emplace_or_replace<AlignmentComponent>(e, AlignmentComponent{chara_creation.alignment});
    };
}

alignment_t gamestate::default_alignment_for_race(race_t rt) {
    return rpg::entities::Actor::default_alignment_for_race(rt);
}

bool gamestate::alignment_is_aggressive(alignment_t alignment_value) {
    return rpg::entities::Actor::alignment_is_aggressive(alignment_value);
}

with_fun gamestate::npc_alignment_init(alignment_t alignment_value) {
    return [alignment_value](gamestate& g, const entityid id) {
        g.registry.emplace_or_replace<AlignmentComponent>(g.ensure_registry_entity(id), AlignmentComponent{alignment_value});
    };
}

entityid gamestate::create_weapon_at_with(vec3 loc, with_fun weaponInitFunction) {
    minfo2("create weapon at with: %d %d %d", loc.x, loc.y, loc.z);
    if (d.floors.size() == 0) {
        merror2("dungeon floors size is 0");
        return INVALID;
    }
    if (!d.is_initialized) {
        merror2("dungeon is_initialized flag not set");
        return INVALID;
    }
    if (vec3_invalid(loc)) {
        merror2("loc is invalid");
        return INVALID;
    }
    shared_ptr<dungeon_floor> df = d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    if (!tile_is_walkable(tile.get_type())) {
        merror2("cannot create entity on non-walkable tile");
        return INVALID;
    }
    entityid id = create_weapon_with(weaponInitFunction);
    if (id == ENTITYID_INVALID) {
        minfo2("failed to create weapon");
        return INVALID;
    }
    if (df->df_add_at(id, entitytype_t::ITEM, loc) == ENTITYID_INVALID) {
        minfo2("failed to add weapon to df");
        return INVALID;
    }
    registry.emplace_or_replace<Position>(ensure_registry_entity(id), Position{loc});
    sync_registry_grid_position(id, loc);
    return id;
}

entityid gamestate::create_weapon_at_random_loc_with(with_fun weaponInitFunction) {
    vec3 loc = d.floors[d.current_floor]->get_random_loc();
    if (vec3_invalid(loc)) {
        merror("loc is invalid");
        return INVALID;
    }
    return create_weapon_at_with(loc, dagger_init());
}

entityid gamestate::create_shield_with(with_fun shieldInitFunction) {
    entityid id = rpg::entities::Item::create_shield(*this);
    shieldInitFunction(*this, id);
    mirror_shield_item(*this, id, get_component_or<ShieldSubtype>(id, shieldtype_t::NONE));
    return id;
}

entityid gamestate::create_shield_at_with(vec3 loc, with_fun shieldInitFunction) {
    if (d.floors.size() == 0) {
        return INVALID;
    }
    entityid id = rpg::entities::Item::create_shield_at(*this, loc);
    shieldInitFunction(*this, id);
    mirror_shield_item(*this, id, get_component_or<ShieldSubtype>(id, shieldtype_t::NONE));
    return id;
}

entityid gamestate::create_potion_with(with_fun potionInitFunction) {
    entityid id = rpg::entities::Item::create_potion(*this);
    potionInitFunction(*this, id);
    mirror_potion_item(*this, id, get_component_or<PotionSubtype>(id, potiontype_t::NONE));
    return id;
}

entityid gamestate::create_potion_at_with(vec3 loc, with_fun potionInitFunction) {
    entityid id = rpg::entities::Item::create_potion_at(*this, loc);
    if (id == INVALID) {
        return INVALID;
    }
    potionInitFunction(*this, id);
    mirror_potion_item(*this, id, get_component_or<PotionSubtype>(id, potiontype_t::NONE));
    return id;
}

race_t gamestate::random_monster_type() {
    static const vector<race_t> monster_races = {race_t::GOBLIN, race_t::ORC, race_t::BAT, race_t::WOLF, race_t::WARG, race_t::ZOMBIE, race_t::SKELETON, race_t::RAT, race_t::GREEN_SLIME};
    uniform_int_distribution<int> gen(0, monster_races.size() - 1);
    int random_index = gen(mt);
    return monster_races[random_index];
}

void gamestate::set_npc_starting_stats(entityid id) {
    rpg::entities::Actor::set_starting_stats(*this, id);
}

void gamestate::set_npc_defaults(entityid id) {
    rpg::entities::Actor::set_defaults(*this, id);
}

entityid gamestate::create_npc_with(race_t rt, with_fun npcInitFunction) {
    entityid id = rpg::entities::Actor::create_npc(*this, rt);
    npcInitFunction(*this, id);
    return id;
}

entityid gamestate::create_npc_at_with(race_t rt, vec3 loc, with_fun npcInitFunction) {
    entityid id = rpg::entities::Actor::create_npc_at(*this, rt, loc);
    if (id == INVALID) {
        return INVALID;
    }
    npcInitFunction(*this, id);
    return id;
}

bool gamestate::add_to_inventory(entityid actor_id, entityid item_id) {
    minfo2("adding %d to %d's inventory", actor_id, item_id);
    auto* inventory_ptr = get_component<Inventory>(actor_id);
    if (!inventory_ptr) {
        merror2("%d has no inventory component", actor_id);
        return false;
    }
    inventory_ptr->value.push_back(item_id);
    msuccess2("added %d to %d's inventory", actor_id, item_id);
    return true;
}

entityid gamestate::create_orc_with(with_fun monsterInitFunction) {
    constexpr race_t r = race_t::ORC;
    entityid id = create_npc_with(r, monsterInitFunction);
    const string orc_name = get_random_orc_name();
    registry.emplace_or_replace<EntityName>(ensure_registry_entity(id), EntityName{orc_name});
    return id;
}

entityid gamestate::create_orc_at_with(vec3 loc, with_fun monsterInitFunction) {
    if (vec3_invalid(loc)) {
        return ENTITYID_INVALID;
    }
    auto df = d.get_floor(loc.z);
    tile_t& t = df->tile_at(loc);
    if (!tile_is_walkable(t.get_type())) {
        return ENTITYID_INVALID;
    }
    if (t.get_cached_live_npc() != INVALID) {
        return INVALID;
    }
    entityid id = create_orc_with(monsterInitFunction);
    if (id == ENTITYID_INVALID) {
        return ENTITYID_INVALID;
    }
    if (!df->df_add_at(id, entitytype_t::NPC, loc)) {
        return ENTITYID_INVALID;
    }
    auto e = ensure_registry_entity(id);
    registry.emplace_or_replace<Position>(e, Position{loc});
    registry.emplace_or_replace<NeedsUpdate>(e, NeedsUpdate{true});
    return id;
}

entityid gamestate::create_player_at_with(vec3 loc, string n, with_fun playerInitFunction) {
    minfo2("create player with: loc=(%d, %d, %d), n=%s", loc.x, loc.y, loc.z, n.c_str());
    massert(n != "", "name is empty string");
    race_t rt = chara_creation.race;
    entityid id = create_npc_at_with(rt, loc, playerInitFunction);
    if (id == ENTITYID_INVALID && !vec3_invalid(loc)) {
        auto spawn_floor = d.get_floor(loc.z);
        const vec3 fallback_loc = spawn_floor->get_random_loc();
        if (!vec3_invalid(fallback_loc)) {
            minfo2("retrying player creation at fallback loc=(%d, %d, %d)", fallback_loc.x, fallback_loc.y, fallback_loc.z);
            loc = fallback_loc;
            id = create_npc_at_with(rt, loc, playerInitFunction);
        }
    }
    massert(id != ENTITYID_INVALID, "id is invalid");
    constexpr int hp_ = 10;
    constexpr int maxhp_ = 10;
    constexpr int vis_dist = 20;
    constexpr int light_rad = 3;
    constexpr int hear_dist = 3;
    set_hero_id(id);
    sync_entt_entity_type_tags(id, entitytype_t::PLAYER);

    auto df = d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    tile.set_cached_player_present(true);
    tile.set_cached_live_npc(id);

    auto e = ensure_registry_entity(id);
    registry.emplace_or_replace<HitPoints>(e, HitPoints{vec2{hp_, maxhp_}});
    registry.emplace_or_replace<VisionRange>(e, VisionRange{vis_dist});
    registry.emplace_or_replace<LightRadius>(e, LightRadius{light_rad});
    registry.emplace_or_replace<HearingRange>(e, HearingRange{hear_dist});
    registry.emplace_or_replace<EntityName>(e, EntityName{n});
    registry.emplace_or_replace<DeadFlag>(e, DeadFlag{false});
    return id;
}

entityid gamestate::create_box_with() {
    return rpg::entities::Box::create(*this);
}

entityid gamestate::create_box_at_with(vec3 loc) {
    return rpg::entities::Box::create_at(*this, loc);
}
