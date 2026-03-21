#include "gamestate.h"

#include "ecs_actor_components.h"
#include "ecs_gameplay_components.h"
#include "ecs_item_components.h"
#include "actor_definitions.h"
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
    entityid id = add_entity();
    ct.set<entitytype>(id, entitytype_t::ITEM);
    sync_entt_entity_type_tags(id, entitytype_t::ITEM);
    ct.set<itemtype>(id, itemtype_t::WEAPON);
    ct.set<spritemove>(id, Rectangle{0, 0, 0, 0});
    ct.set<update>(id, true);
    auto e = ensure_registry_entity(id);
    registry.emplace_or_replace<ItemSubtype>(e, ItemSubtype{itemtype_t::WEAPON});
    registry.emplace_or_replace<SpriteMoveState>(e, SpriteMoveState{Rectangle{0, 0, 0, 0}});
    registry.emplace_or_replace<NeedsUpdate>(e, NeedsUpdate{true});
    weaponInitFunction(*this, id);
    mirror_weapon_item(*this, id, get_component_or<WeaponSubtype>(id, weapontype_t::NONE));
    return id;
}

with_fun gamestate::dagger_init() {
    return [](gamestate& g, const entityid id) {
        g.ct.set<name>(id, "dagger");
        g.ct.set<description>(id, "Stabby stabby.");
        g.ct.set<weapontype>(id, weapontype_t::DAGGER);
        g.ct.set<damage>(id, vec3{1, 4, 0});
        g.ct.set<durability>(id, 100);
        g.ct.set<max_durability>(id, 100);
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
        g.ct.set<name>(id, "axe");
        g.ct.set<description>(id, "Choppy choppy");
        g.ct.set<weapontype>(id, weapontype_t::AXE);
        g.ct.set<damage>(id, vec3{1, 8, 0});
        g.ct.set<durability>(id, 100);
        g.ct.set<max_durability>(id, 100);
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
        g.ct.set<name>(id, "short sword");
        g.ct.set<description>(id, "your basic soldier's short sword");
        g.ct.set<weapontype>(id, weapontype_t::SHORT_SWORD);
        g.ct.set<damage>(id, vec3{1, 6, 0});
        g.ct.set<durability>(id, 100);
        g.ct.set<max_durability>(id, 100);
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
        g.ct.set<name>(id, "kite shield");
        g.ct.set<description>(id, "Standard knight's shield");
        g.ct.set<shieldtype>(id, shieldtype_t::KITE);
        g.ct.set<block_chance>(id, 90);
        g.ct.set<durability>(id, 100);
        g.ct.set<max_durability>(id, 100);
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
        g.ct.set<potiontype>(id, pt);
        g.registry.emplace_or_replace<PotionSubtype>(g.ensure_registry_entity(id), PotionSubtype{pt});
        if (pt == potiontype_t::HP_SMALL) {
            g.ct.set<name>(id, "small healing potion");
            g.ct.set<description>(id, "a small healing potion");
            g.ct.set<healing>(id, vec3{1, 6, 0});
            auto e = g.ensure_registry_entity(id);
            g.registry.emplace_or_replace<EntityName>(e, EntityName{"small healing potion"});
            g.registry.emplace_or_replace<EntityDescription>(e, EntityDescription{"a small healing potion"});
            g.registry.emplace_or_replace<HealingDice>(e, HealingDice{vec3{1, 6, 0}});
        }
    };
}

with_fun gamestate::player_init(int maxhp_roll) {
    return [this, maxhp_roll](gamestate& g, const entityid id) {
        g.ct.set<strength>(id, chara_creation.strength);
        g.ct.set<dexterity>(id, chara_creation.dexterity);
        g.ct.set<constitution>(id, chara_creation.constitution);
        g.ct.set<intelligence>(id, chara_creation.intelligence);
        g.ct.set<wisdom>(id, chara_creation.wisdom);
        g.ct.set<charisma>(id, chara_creation.charisma);
        g.ct.set<hd>(id, vec3{1, chara_creation.hitdie, 0});
        g.ct.set<hp>(id, vec2{maxhp_roll, maxhp_roll});
        g.ct.set<alignment>(id, chara_creation.alignment);
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
    switch (rt) {
    case race_t::HUMAN: return alignment_t::NEUTRAL_NEUTRAL;
    case race_t::ELF: return alignment_t::GOOD_CHAOTIC;
    case race_t::DWARF: return alignment_t::GOOD_LAWFUL;
    case race_t::HALFLING: return alignment_t::GOOD_NEUTRAL;
    case race_t::GOBLIN: return alignment_t::EVIL_NEUTRAL;
    case race_t::ORC: return alignment_t::EVIL_CHAOTIC;
    case race_t::BAT: return alignment_t::NEUTRAL_NEUTRAL;
    case race_t::GREEN_SLIME: return alignment_t::NEUTRAL_NEUTRAL;
    case race_t::WOLF: return alignment_t::NEUTRAL_NEUTRAL;
    case race_t::WARG: return alignment_t::EVIL_NEUTRAL;
    case race_t::RAT: return alignment_t::NEUTRAL_NEUTRAL;
    case race_t::SKELETON: return alignment_t::EVIL_LAWFUL;
    case race_t::ZOMBIE: return alignment_t::EVIL_NEUTRAL;
    case race_t::NONE:
    case race_t::COUNT:
    default: break;
    }
    return alignment_t::NONE;
}

bool gamestate::alignment_is_aggressive(alignment_t alignment_value) {
    switch (alignment_value) {
    case alignment_t::EVIL_LAWFUL:
    case alignment_t::EVIL_NEUTRAL:
    case alignment_t::EVIL_CHAOTIC:
        return true;
    default:
        break;
    }
    return false;
}

with_fun gamestate::npc_alignment_init(alignment_t alignment_value) {
    return [alignment_value](gamestate& g, const entityid id) {
        g.ct.set<alignment>(id, alignment_value);
        g.registry.emplace_or_replace<AlignmentComponent>(g.ensure_registry_entity(id), AlignmentComponent{alignment_value});
    };
}

entityid gamestate::create_weapon_at_with(ComponentTable& ct, vec3 loc, with_fun weaponInitFunction) {
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
    ct.set<location>(id, loc);
    registry.emplace_or_replace<Position>(ensure_registry_entity(id), Position{loc});
    sync_registry_grid_position(id, loc);
    return id;
}

entityid gamestate::create_weapon_at_random_loc_with(CT& ct, with_fun weaponInitFunction) {
    vec3 loc = d.floors[d.current_floor]->get_random_loc();
    if (vec3_invalid(loc)) {
        merror("loc is invalid");
        return INVALID;
    }
    return create_weapon_at_with(ct, loc, dagger_init());
}

entityid gamestate::create_shield_with(ComponentTable& ct, with_fun shieldInitFunction) {
    entityid id = add_entity();
    ct.set<entitytype>(id, entitytype_t::ITEM);
    sync_entt_entity_type_tags(id, entitytype_t::ITEM);
    ct.set<itemtype>(id, itemtype_t::SHIELD);
    ct.set<durability>(id, 100);
    ct.set<max_durability>(id, 100);
    ct.set<update>(id, false);
    auto e = ensure_registry_entity(id);
    registry.emplace_or_replace<ItemSubtype>(e, ItemSubtype{itemtype_t::SHIELD});
    registry.emplace_or_replace<ItemDurability>(e, ItemDurability{100});
    registry.emplace_or_replace<ItemMaxDurability>(e, ItemMaxDurability{100});
    registry.emplace_or_replace<NeedsUpdate>(e, NeedsUpdate{false});
    shieldInitFunction(*this, id);
    mirror_shield_item(*this, id, get_component_or<ShieldSubtype>(id, shieldtype_t::NONE));
    return id;
}

entityid gamestate::create_shield_at_with(ComponentTable& ct, vec3 loc, with_fun shieldInitFunction) {
    if (d.floors.size() == 0) {
        return INVALID;
    }
    entityid id = create_shield_with(ct, shieldInitFunction);
    shared_ptr<dungeon_floor> df = d.get_floor(loc.z);
    if (!df->df_add_at(id, entitytype_t::ITEM, loc)) {
        return INVALID;
    }
    ct.set<location>(id, loc);
    registry.emplace_or_replace<Position>(ensure_registry_entity(id), Position{loc});
    sync_registry_grid_position(id, loc);
    return id;
}

entityid gamestate::create_potion_with(with_fun potionInitFunction) {
    entityid id = add_entity();
    ct.set<entitytype>(id, entitytype_t::ITEM);
    sync_entt_entity_type_tags(id, entitytype_t::ITEM);
    ct.set<itemtype>(id, itemtype_t::POTION);
    ct.set<update>(id, true);
    auto e = ensure_registry_entity(id);
    registry.emplace_or_replace<ItemSubtype>(e, ItemSubtype{itemtype_t::POTION});
    registry.emplace_or_replace<NeedsUpdate>(e, NeedsUpdate{true});
    potionInitFunction(*this, id);
    mirror_potion_item(*this, id, get_component_or<PotionSubtype>(id, potiontype_t::NONE));
    return id;
}

entityid gamestate::create_potion_at_with(vec3 loc, with_fun potionInitFunction) {
    shared_ptr<dungeon_floor> df = d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    if (!tile_is_walkable(tile.get_type())) {
        return INVALID;
    }
    entityid id = create_potion_with(potionInitFunction);
    if (id == INVALID) {
        return INVALID;
    }
    if (!df->df_add_at(id, entitytype_t::ITEM, loc)) {
        return INVALID;
    }
    ct.set<location>(id, loc);
    ct.set<update>(id, true);
    auto e = ensure_registry_entity(id);
    registry.emplace_or_replace<Position>(e, Position{loc});
    registry.emplace_or_replace<NeedsUpdate>(e, NeedsUpdate{true});
    sync_registry_grid_position(id, loc);
    return id;
}

race_t gamestate::random_monster_type() {
    static const vector<race_t> monster_races = {race_t::GOBLIN, race_t::ORC, race_t::BAT, race_t::WOLF, race_t::WARG, race_t::ZOMBIE, race_t::SKELETON, race_t::RAT, race_t::GREEN_SLIME};
    uniform_int_distribution<int> gen(0, monster_races.size() - 1);
    int random_index = gen(mt);
    return monster_races[random_index];
}

void gamestate::set_npc_starting_stats(entityid id) {
    race_t rt = (get_component<ActorKind>(id) ? get_component<ActorKind>(id)->race : race_t::NONE);
    if (rt == race_t::NONE) {
        return;
    }
    const ActorDefinition* def = get_actor_definition(rt);
    int str_m = def ? def->modifiers[0] : get_racial_modifiers(rt, 0);
    int dex_m = def ? def->modifiers[1] : get_racial_modifiers(rt, 1);
    int int_m = def ? def->modifiers[2] : get_racial_modifiers(rt, 2);
    int wis_m = def ? def->modifiers[3] : get_racial_modifiers(rt, 3);
    int con_m = def ? def->modifiers[4] : get_racial_modifiers(rt, 4);
    int cha_m = def ? def->modifiers[5] : get_racial_modifiers(rt, 5);
    uniform_int_distribution<int> gen(3, 18);
    int strength_ = gen(mt) + str_m;
    int dexterity_ = gen(mt) + dex_m;
    int intelligence_ = gen(mt) + int_m;
    int wisdom_ = gen(mt) + wis_m;
    int constitution_ = gen(mt) + con_m;
    int charisma_ = gen(mt) + cha_m;
    ct.set<strength>(id, strength_);
    ct.set<dexterity>(id, dexterity_);
    ct.set<intelligence>(id, intelligence_);
    ct.set<wisdom>(id, wisdom_);
    ct.set<constitution>(id, constitution_);
    ct.set<charisma>(id, charisma_);
    auto e = ensure_registry_entity(id);
    registry.emplace_or_replace<StrengthAttr>(e, StrengthAttr{strength_});
    registry.emplace_or_replace<DexterityAttr>(e, DexterityAttr{dexterity_});
    registry.emplace_or_replace<IntelligenceAttr>(e, IntelligenceAttr{intelligence_});
    registry.emplace_or_replace<WisdomAttr>(e, WisdomAttr{wisdom_});
    registry.emplace_or_replace<ConstitutionAttr>(e, ConstitutionAttr{constitution_});
    registry.emplace_or_replace<CharismaAttr>(e, CharismaAttr{charisma_});
    vec3 hitdie = {1, def ? def->default_hitdie : 8, 0};
    if (!def) {
        switch (rt) {
        case race_t::HUMAN: hitdie.y = 8; break;
        case race_t::ELF: hitdie.y = 6; break;
        case race_t::DWARF: hitdie.y = 10; break;
        case race_t::HALFLING: hitdie.y = 6; break;
        case race_t::GOBLIN: hitdie.y = 6; break;
        case race_t::ORC: hitdie.y = 8; break;
        case race_t::BAT: hitdie.y = 3; break;
        case race_t::GREEN_SLIME: hitdie.y = 4; break;
        case race_t::WOLF: hitdie.y = 6; break;
        case race_t::WARG: hitdie.y = 12; break;
        case race_t::RAT: hitdie.y = 4; break;
        case race_t::SKELETON: hitdie.y = 8; break;
        case race_t::ZOMBIE: hitdie.y = 8; break;
        default: break;
        }
    }
    uniform_int_distribution<int> gen2(1, hitdie.y);
    int my_maxhp = std::max(1, gen2(mt) + get_stat_bonus(constitution_));
    int my_hp = my_maxhp;
    ct.set<hp>(id, vec2{my_hp, my_maxhp});
    ct.set<base_ac>(id, 10);
    ct.set<hd>(id, hitdie);
    registry.emplace_or_replace<HitPoints>(e, HitPoints{vec2{my_hp, my_maxhp}});
    registry.emplace_or_replace<BaseArmorClass>(e, BaseArmorClass{10});
    registry.emplace_or_replace<HitDie>(e, HitDie{hitdie});
}

void gamestate::set_npc_defaults(entityid id) {
    ct.set<entitytype>(id, entitytype_t::NPC);
    sync_entt_entity_type_tags(id, entitytype_t::NPC);
    ct.set<spritemove>(id, Rectangle{0, 0, 0, 0});
    ct.set<dead>(id, false);
    ct.set<update>(id, true);
    ct.set<direction>(id, direction_t::DOWN_RIGHT);
    ct.set<attacking>(id, false);
    ct.set<block_success>(id, false);
    ct.set<damaged>(id, false);
    ct.set<inventory>(id, make_shared<vector<entityid>>());
    ct.set<equipped_weapon>(id, ENTITYID_INVALID);
    ct.set<aggro>(id, false);
    ct.set<vision_distance>(id, 3);
    ct.set<hearing_distance>(id, 3);
    ct.set<level>(id, 1);
    ct.set<xp>(id, 0);
    ct.set<entity_default_action>(id, entity_default_action_t::NONE);
    ct.set<target_path>(id, make_shared<vector<vec3>>());
    auto e = ensure_registry_entity(id);
    registry.emplace_or_replace<SpriteMoveState>(e, SpriteMoveState{Rectangle{0, 0, 0, 0}});
    registry.emplace_or_replace<DeadFlag>(e, DeadFlag{false});
    registry.emplace_or_replace<NeedsUpdate>(e, NeedsUpdate{true});
    registry.emplace_or_replace<Facing>(e, Facing{direction_t::DOWN_RIGHT});
    registry.emplace_or_replace<AttackingFlag>(e, AttackingFlag{false});
    registry.emplace_or_replace<BlockSuccessFlag>(e, BlockSuccessFlag{false});
    registry.emplace_or_replace<DamagedFlag>(e, DamagedFlag{false});
    registry.emplace_or_replace<Inventory>(e, Inventory{});
    registry.emplace_or_replace<EquippedWeapon>(e, EquippedWeapon{ENTITYID_INVALID});
    registry.emplace_or_replace<AggroFlag>(e, AggroFlag{false});
    registry.emplace_or_replace<VisionRange>(e, VisionRange{3});
    registry.emplace_or_replace<HearingRange>(e, HearingRange{3});
    registry.emplace_or_replace<EntityLevel>(e, EntityLevel{1});
    registry.emplace_or_replace<Experience>(e, Experience{0});
    registry.emplace_or_replace<DefaultAction>(e, DefaultAction{entity_default_action_t::NONE});
    registry.emplace_or_replace<NpcPath>(e, NpcPath{});
}

entityid gamestate::create_npc_with(race_t rt, with_fun npcInitFunction) {
    entityid id = add_entity();
    set_npc_defaults(id);
    ct.set<race>(id, rt);
    registry.emplace_or_replace<ActorKind>(ensure_registry_entity(id), ActorKind{rt});

    const ActorDefinition* def = get_actor_definition(rt);
    const alignment_t default_alignment = def ? def->default_alignment : default_alignment_for_race(rt);

    npc_alignment_init(default_alignment)(*this, id);
    ct.set<aggro>(id, alignment_is_aggressive(default_alignment));
    registry.emplace_or_replace<AggroFlag>(ensure_registry_entity(id), AggroFlag{alignment_is_aggressive(default_alignment)});
    set_npc_starting_stats(id);
    npcInitFunction(*this, id);
    if (!has_component<EntityName>(id)) {
        const string npc_name = def ? def->default_name : race2str(rt);
        ct.set<name>(id, npc_name);
        registry.emplace_or_replace<EntityName>(ensure_registry_entity(id), EntityName{npc_name});
    }
    if (!has_component<DialogueLine>(id)) {
        const string npc_dialogue = def ? def->default_description : "They give you a guarded look but say nothing.";
        ct.set<dialogue_text>(id, npc_dialogue);
        registry.emplace_or_replace<DialogueLine>(ensure_registry_entity(id), DialogueLine{npc_dialogue});
    }

    if (def) {
        const entt::entity registry_entity = ensure_registry_entity(id);
        registry.emplace_or_replace<ActorKind>(registry_entity, ActorKind{rt});
        registry.emplace_or_replace<ActorVisual>(registry_entity, ActorVisual{def->sprites, def->sprite_count});
        registry.emplace_or_replace<ActorText>(registry_entity, ActorText{def->default_name, def->default_description});
    }

    return id;
}

entityid gamestate::create_npc_at_with(race_t rt, vec3 loc, with_fun npcInitFunction) {
    minfo2("create npc at with: (%d, %d, %d)", loc.x, loc.y, loc.z);
    auto df = d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    if (!tile_is_walkable(tile.get_type())) {
        merror2("cannot create entity on non-walkable tile: tile.type: %s", tiletype2str(tile.get_type()).c_str());
        return INVALID;
    }
    if (tile_has_box(loc.x, loc.y, loc.z) != ENTITYID_INVALID) {
        merror2("cannot create entity on tile with box");
        return INVALID;
    }
    if (tile_has_pushable(loc.x, loc.y, loc.z) != ENTITYID_INVALID) {
        merror2("cannot create entity on tile with pushable");
        return INVALID;
    }
    if (tile.get_cached_live_npc() != INVALID) {
        merror2("cannot create entity on tile with live npc");
        return INVALID;
    }
    entityid id = create_npc_with(rt, npcInitFunction);
    if (id == INVALID) {
        merror("failed to create npc");
        return INVALID;
    }
    if (df->df_add_at(id, entitytype_t::NPC, loc) == INVALID) {
        merror("failed to add npc %d to %d, %d", id, loc.x, loc.y);
        return INVALID;
    }
    minfo2("setting location for %d", id);
    ct.set<location>(id, loc);
    registry.emplace_or_replace<Position>(ensure_registry_entity(id), Position{loc});
    sync_registry_grid_position(id, loc);
    msuccess2("created npc %d", id);
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
    ct.set<name>(id, orc_name);
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
    ct.set<location>(id, loc);
    ct.set<update>(id, true);
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
    ct.set<entitytype>(id, entitytype_t::PLAYER);
    sync_entt_entity_type_tags(id, entitytype_t::PLAYER);

    auto df = d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    tile.set_cached_player_present(true);
    tile.set_cached_live_npc(id);

    ct.set<hp>(id, vec2{hp_, maxhp_});
    ct.set<vision_distance>(id, vis_dist);
    ct.set<light_radius>(id, light_rad);
    ct.set<hearing_distance>(id, hear_dist);
    ct.set<name>(id, n);
    ct.set<dead>(id, false);
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
    entityid id = add_entity();
    const StaticWorldDefinition& definition = get_static_world_definition(entitytype_t::BOX);
    ct.set<entitytype>(id, entitytype_t::BOX);
    sync_entt_entity_type_tags(id, entitytype_t::BOX);
    ct.set<spritemove>(id, Rectangle{0, 0, 0, 0});
    ct.set<update>(id, true);
    ct.set<pushable>(id, definition.pushable);
    ct.set<pullable>(id, definition.pullable);
    ct.set<solid>(id, definition.solid);
    ct.set<name>(id, definition.name);
    ct.set<description>(id, definition.description);
    auto e = ensure_registry_entity(id);
    registry.emplace_or_replace<SpriteMoveState>(e, SpriteMoveState{Rectangle{0, 0, 0, 0}});
    registry.emplace_or_replace<NeedsUpdate>(e, NeedsUpdate{true});
    registry.emplace_or_replace<PushableTag>(e, PushableTag{definition.pushable});
    registry.emplace_or_replace<PullableTag>(e, PullableTag{definition.pullable});
    registry.emplace_or_replace<SolidTag>(e, SolidTag{definition.solid});
    registry.emplace_or_replace<EntityName>(e, EntityName{definition.name});
    registry.emplace_or_replace<EntityDescription>(e, EntityDescription{definition.description});
    attach_static_world_definition(id, definition);
    return id;
}

entityid gamestate::create_box_at_with(vec3 loc) {
    shared_ptr<dungeon_floor> df = d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    if (!tile_is_walkable(tile.get_type())) {
        merror("cannot create entity on non-walkable tile");
        return ENTITYID_INVALID;
    }
    if (tile_has_box(loc.x, loc.y, loc.z) != ENTITYID_INVALID) {
        merror("cannot create entity on tile with box");
        return ENTITYID_INVALID;
    }
    entityid id = create_box_with();
    if (df->df_add_at(id, entitytype_t::BOX, loc) == ENTITYID_INVALID) {
        merror("failed df_add_at: %d, %d, %d", id, loc.x, loc.y);
        return ENTITYID_INVALID;
    }
    ct.set<location>(id, loc);
    registry.emplace_or_replace<Position>(ensure_registry_entity(id), Position{loc});
    sync_registry_grid_position(id, loc);
    return id;
}
