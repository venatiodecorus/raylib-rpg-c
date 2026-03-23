/** @file actor.cpp
 *  @brief Actor entity class implementation.
 */

#include "actor.h"

#include "../ecs_actor_components.h"
#include "../ecs_gameplay_components.h"
#include "../ecs_world_object_components.h"
#include "../gamestate.h"
#include "../actor_definitions.h"
#include "../get_equipped_sprite.h"
#include "../libdraw_create_spritegroup.h"
#include "../mprint.h"
#include "../massert.h"

namespace rpg::entities {

namespace {

void set_npc_alignment(gamestate& g, entityid id, alignment_t alignment_value) {
    g.registry.emplace_or_replace<AlignmentComponent>(id, AlignmentComponent{alignment_value});
}

} // namespace

void Actor::set_defaults(gamestate& g, entityid id) {
    g.sync_entt_entity_type_tags(id, entitytype_t::NPC);
    const auto e = id;
    g.registry.emplace_or_replace<SpriteMoveState>(e, SpriteMoveState{Rectangle{0, 0, 0, 0}});
    g.registry.emplace_or_replace<DeadFlag>(e, DeadFlag{false});
    g.registry.emplace_or_replace<NeedsUpdate>(e, NeedsUpdate{true});
    g.registry.emplace_or_replace<Facing>(e, Facing{direction_t::DOWN_RIGHT});
    g.registry.emplace_or_replace<AttackingFlag>(e, AttackingFlag{false});
    g.registry.emplace_or_replace<BlockSuccessFlag>(e, BlockSuccessFlag{false});
    g.registry.emplace_or_replace<DamagedFlag>(e, DamagedFlag{false});
    g.registry.emplace_or_replace<Inventory>(e, Inventory{});
    g.registry.emplace_or_replace<EquippedWeapon>(e, EquippedWeapon{ENTITYID_INVALID});
    g.registry.emplace_or_replace<AggroFlag>(e, AggroFlag{false});
    g.registry.emplace_or_replace<VisionRange>(e, VisionRange{3});
    g.registry.emplace_or_replace<HearingRange>(e, HearingRange{3});
    g.registry.emplace_or_replace<EntityLevel>(e, EntityLevel{1});
    g.registry.emplace_or_replace<Experience>(e, Experience{0});
    g.registry.emplace_or_replace<DefaultAction>(e, DefaultAction{entity_default_action_t::NONE});
    g.registry.emplace_or_replace<NpcPath>(e, NpcPath{});
}

void Actor::set_starting_stats(gamestate& g, entityid id) {
    race_t rt = (g.get_component<ActorKind>(id) ? g.get_component<ActorKind>(id)->race : race_t::NONE);
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
    int strength_ = gen(g.mt) + str_m;
    int dexterity_ = gen(g.mt) + dex_m;
    int intelligence_ = gen(g.mt) + int_m;
    int wisdom_ = gen(g.mt) + wis_m;
    int constitution_ = gen(g.mt) + con_m;
    int charisma_ = gen(g.mt) + cha_m;
    const auto e = id;
    g.registry.emplace_or_replace<StrengthAttr>(e, StrengthAttr{strength_});
    g.registry.emplace_or_replace<DexterityAttr>(e, DexterityAttr{dexterity_});
    g.registry.emplace_or_replace<IntelligenceAttr>(e, IntelligenceAttr{intelligence_});
    g.registry.emplace_or_replace<WisdomAttr>(e, WisdomAttr{wisdom_});
    g.registry.emplace_or_replace<ConstitutionAttr>(e, ConstitutionAttr{constitution_});
    g.registry.emplace_or_replace<CharismaAttr>(e, CharismaAttr{charisma_});
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
    int my_maxhp = std::max(1, gen2(g.mt) + get_stat_bonus(constitution_));
    int my_hp = my_maxhp;
    g.registry.emplace_or_replace<HitPoints>(e, HitPoints{vec2{my_hp, my_maxhp}});
    g.registry.emplace_or_replace<BaseArmorClass>(e, BaseArmorClass{10});
    g.registry.emplace_or_replace<HitDie>(e, HitDie{hitdie});
}

alignment_t Actor::default_alignment_for_race(race_t rt) {
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

bool Actor::alignment_is_aggressive(alignment_t alignment_value) {
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

entityid Actor::create_npc(gamestate& g, race_t rt) {
    entityid id = g.add_entity();
    set_defaults(g, id);
    g.registry.emplace_or_replace<ActorKind>(id, ActorKind{rt});

    const ActorDefinition* def = get_actor_definition(rt);
    const alignment_t default_alignment = def ? def->default_alignment : default_alignment_for_race(rt);

    set_npc_alignment(g, id, default_alignment);
    g.registry.emplace_or_replace<AggroFlag>(id, AggroFlag{alignment_is_aggressive(default_alignment)});
    set_starting_stats(g, id);
    if (!g.has_component<EntityName>(id)) {
        const string npc_name = def ? def->default_name : race2str(rt);
        g.registry.emplace_or_replace<EntityName>(id, EntityName{npc_name});
    }
    if (!g.has_component<DialogueLine>(id)) {
        const string npc_dialogue = def ? def->default_description : "They give you a guarded look but say nothing.";
        g.registry.emplace_or_replace<DialogueLine>(id, DialogueLine{npc_dialogue});
    }

    if (def) {
        const auto registry_entity = id;
        g.registry.emplace_or_replace<ActorKind>(registry_entity, ActorKind{rt});
        g.registry.emplace_or_replace<ActorVisual>(registry_entity, ActorVisual{def->sprites, def->sprite_count});
        g.registry.emplace_or_replace<ActorText>(registry_entity, ActorText{def->default_name, def->default_description});
    }

    return id;
}

entityid Actor::create_npc_at(gamestate& g, race_t rt, vec3 loc) {
    minfo2("create npc at with: (%d, %d, %d)", loc.x, loc.y, loc.z);
    auto df = g.d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    if (!tile_is_walkable(tile.get_type())) {
        merror2("cannot create entity on non-walkable tile: tile.type: %s", tiletype2str(tile.get_type()).c_str());
        return INVALID;
    }
    if (g.tile_has_box(loc.x, loc.y, loc.z) != ENTITYID_INVALID) {
        merror2("cannot create entity on tile with box");
        return INVALID;
    }
    if (g.tile_has_pushable(loc.x, loc.y, loc.z) != ENTITYID_INVALID) {
        merror2("cannot create entity on tile with pushable");
        return INVALID;
    }
    if (tile.get_cached_live_npc() != INVALID) {
        merror2("cannot create entity on tile with live npc");
        return INVALID;
    }
    entityid id = create_npc(g, rt);
    if (id == INVALID) {
        merror("failed to create npc");
        return INVALID;
    }
    if (df->df_add_at(id, entitytype_t::NPC, loc) == INVALID) {
        merror("failed to add npc %d to %d, %d", id, loc.x, loc.y);
        return INVALID;
    }
    minfo2("setting location for %d", id);
    g.registry.emplace_or_replace<Position>(id, Position{loc});
    g.sync_registry_grid_position(id, loc);
    msuccess2("created npc %d", id);
    return id;
}

entityid Actor::create_player_at(gamestate& g, vec3 loc, const std::string& n) {
    minfo2("create player with: loc=(%d, %d, %d), n=%s", loc.x, loc.y, loc.z, n.c_str());
    massert(n != "", "name is empty string");
    race_t rt = g.chara_creation.race;
    entityid id = create_npc_at(g, rt, loc);
    if (id == ENTITYID_INVALID && !vec3_invalid(loc)) {
        auto spawn_floor = g.d.get_floor(loc.z);
        const vec3 fallback_loc = spawn_floor->get_random_loc();
        if (!vec3_invalid(fallback_loc)) {
            minfo2("retrying player creation at fallback loc=(%d, %d, %d)", fallback_loc.x, fallback_loc.y, fallback_loc.z);
            loc = fallback_loc;
            id = create_npc_at(g, rt, loc);
        }
    }
    massert(id != ENTITYID_INVALID, "id is invalid");
    constexpr int hp_ = 10;
    constexpr int maxhp_ = 10;
    constexpr int vis_dist = 20;
    constexpr int light_rad = 3;
    constexpr int hear_dist = 3;
    g.set_hero_id(id);
    g.sync_entt_entity_type_tags(id, entitytype_t::PLAYER);

    auto df = g.d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    tile.set_cached_player_present(true);
    tile.set_cached_live_npc(id);

    const auto e = id;
    g.registry.emplace_or_replace<HitPoints>(e, HitPoints{vec2{hp_, maxhp_}});
    g.registry.emplace_or_replace<VisionRange>(e, VisionRange{vis_dist});
    g.registry.emplace_or_replace<LightRadius>(e, LightRadius{light_rad});
    g.registry.emplace_or_replace<HearingRange>(e, HearingRange{hear_dist});
    g.registry.emplace_or_replace<EntityName>(e, EntityName{n});
    g.registry.emplace_or_replace<DeadFlag>(e, DeadFlag{false});
    return id;
}

void Actor::create_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    const auto registry_entity = id;
    if (registry_entity != entt::null && g.registry.all_of<ActorVisual>(registry_entity)) {
        const ActorVisual& visual = g.registry.get<ActorVisual>(registry_entity);
        if (visual.sprites != nullptr && visual.sprite_count > 0) {
            create_sg(g, renderer, id, visual.sprites, visual.sprite_count);
            return;
        }
    }
    const race_t r = (g.get_component<ActorKind>(id) ? g.get_component<ActorKind>(id)->race : race_t::NONE);
    massert(r != race_t::NONE, "race is none for id %d", id);
    const ActorDefinition* def = get_actor_definition(r);
    massert(def != nullptr, "no actor definition for race %d", r);
    create_sg(g, renderer, id, def->sprites, def->sprite_count);
}

void Actor::update_sprite(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    massert(sg, "spritegroup is NULL");
    if (sg->update_dest()) {
        g.frame_dirty = true;
    }
    const Position* loc_ptr = g.get_component<Position>(id);
    if (!loc_ptr) {
        return;
    }
    const vec3 loc = loc_ptr->value;
    sg->snap_dest(loc.x, loc.y);
}

void Actor::draw(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    massert(sg, "spritegroup is NULL");
    Rectangle dest = {sg->dest.x, sg->dest.y, sg->dest.width, sg->dest.height};
    auto shield_back_s = get_shield_back_sprite(g, renderer, id, sg);
    if (shield_back_s) {
        DrawTexturePro(*shield_back_s->get_texture(), shield_back_s->get_src(), dest, Vector2{0, 0}, 0, WHITE);
    }
    auto weapon_back_s = get_weapon_back_sprite(g, renderer, id, sg);
    if (weapon_back_s) {
        DrawTexturePro(*weapon_back_s->get_texture(), weapon_back_s->get_src(), dest, Vector2{0, 0}, 0, WHITE);
    }
    shared_ptr<sprite> s = sg->get_current();
    massert(s, "sprite is NULL");
    DrawTexturePro(*s->get_texture(), s->get_src(), dest, Vector2{0, 0}, 0, WHITE);
    auto shield_front_s = get_shield_front_sprite(g, renderer, id, sg);
    if (shield_front_s) {
        DrawTexturePro(*shield_front_s->get_texture(), shield_front_s->get_src(), dest, Vector2{0, 0}, 0, WHITE);
    }
    auto weapon_front_s = get_weapon_front_sprite(g, renderer, id, sg);
    if (weapon_front_s) {
        DrawTexturePro(*weapon_front_s->get_texture(), weapon_front_s->get_src(), dest, Vector2{0, 0}, 0, WHITE);
    }
}

} // namespace rpg::entities
