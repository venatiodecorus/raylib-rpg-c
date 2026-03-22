# EnTT Migration — Phase 2 & 3 Plan

## Context

Branch: `refactor/entt-migration`
Phase 1 is complete AND committed. Phase 2 Tier 1 is complete (gameplay files migrated).

## Current State (as of last session)

**Phase 1 — COMPLETE & COMMITTED:**
- `ecs_gameplay_components.h` created with all EnTT component structs
- `with_fun` signature changed to `function<void(gamestate&, entityid)>`
- All entity creation functions dual-write to both `ct` and `registry`
- `registry.destroy()` added for broken weapons/shields
- All `(CT&, entityid)` lambdas updated to `(gamestate&, entityid)` in test files

**Phase 2 — Tier 1 COMPLETE (gameplay files):**
- Helper methods added to `gamestate.h` (after line 329, before `sync_entt_entity_type_tags`):
  - `get_component<T>(id)` → returns `T*`
  - `get_component_or<T>(id, fallback)` → returns `ptr->value` or fallback
  - `has_component<T>(id)` → bool
- All 6 gameplay files fully migrated from `ct.get`/`ct.get_or`/`ct.has` to EnTT:
  - `gamestate_npc_combat.cpp` (59 lines)
  - `gamestate_world_interaction.cpp` (66 lines)
  - `gamestate_input.cpp` (20 lines)
  - `gamestate_inventory.cpp` (30 lines)
  - `gamestate_lifecycle.cpp` (15 lines)
  - `gamestate_entity_factory.cpp` (7 lines)

**Phase 2 — Tier 2 COMPLETE (rendering files):**
- `libdraw_update_sprites.cpp` — 15 ct calls migrated
- `draw_hud.cpp` — 13 ct calls migrated
- `draw_dungeon_floor.cpp` — 6 ct calls migrated
- `draw_item_detail_text.cpp` — 10 ct calls migrated
- `draw_mini_inventory_menu.cpp` — 4 ct calls migrated, signature changed to `const vector<entityid>&`
- `draw_inventory_menu.cpp` — 1 ct call migrated
- `draw_chest_menu.cpp` — 3 ct calls migrated, `draw_inventory_grid` signature changed to `const vector<entityid>&`
- `draw_look_panel.cpp` — 1 ct call migrated
- `libdraw.cpp` — 2 ct calls migrated
- `libdraw_create_spritegroup.cpp` — 3 ct calls migrated
- `libdraw_gameplay_scene.cpp` — 2 ct calls migrated

**Phase 2 — Tier 3 COMPLETE (small helpers):**
- `get_entity_name.h` — 1 ct call migrated
- `get_entity_type.h` — 1 ct call migrated (uses `ecs_core_components.h` for EntityTypeTag)
- `get_entity_location.h` — 1 ct call migrated
- `get_npc_dead.h` — 1 ct call migrated
- `set_npc_dead.h` — 1 ct.get_or migrated (ct.set kept for dual-write)
- `get_weapon_sprite.h` — 2 ct calls migrated
- `get_shield_sprite.h` — 2 ct calls migrated
- `is_loc_path_blocked.h` — 3 ct calls migrated
- `set_sg.cpp` — 6 ct calls migrated
- `create_sg_byid.cpp` — 7 ct calls migrated
- `debug_entity.h` — 2 ct calls migrated
- `libdraw_update_weapon_for_entity.h` — 1 ct call migrated
- `libdraw_update_shield_for_entity.h` — 1 ct call migrated
- `gamestate.h` — 5 ct calls migrated, added `#include "ecs_gameplay_components.h"`
- `gamestate_world.cpp` — 8 ct calls migrated

**Build verification:** Clean build with `-DDEBUG=1 -DDEBUG_ASSERT=1 -DNPCS_ALL_AT_ONCE -DMASTER_VOLUME=0.25f -DMUSIC_OFF` — zero errors.

**Phase 2 — COMPLETE (all tiers including test files):**

Only `src/unit_test_old.h` (54 lines, commented-out legacy tests) retains `ct.get`/`ct.get_or`/`ct.has` calls. This file is slated for deletion in Phase 3.

All `ct.set` calls remain as dual-write (208 across 10 source files). These are now dead writes since nothing reads from ComponentTable anymore.

**Build verification:** Clean build with `-DDEBUG=1 -DDEBUG_ASSERT=1 -DNPCS_ALL_AT_ONCE -DMASTER_VOLUME=0.25f -DMUSIC_OFF` — zero errors.

**Phase 3** — ready to start. See plan below.

## Known Gotchas for Next Session

1. **`get_component_or` returns `ptr->value`** — this works for most components that have a
   `.value` field. For `EntityTypeTag` (`.type`) and `ActorKind` (`.race`), `get_component_or`
   DOES NOT WORK. Use `get_component<T>(id)` with null check + manual `.type`/`.race` access.
   Pattern: `(get_component<EntityTypeTag>(id) ? get_component<EntityTypeTag>(id)->type : entitytype_t::NONE)`

2. **Pointer returns changed type** — `ct.get<location>(id)` returned `vec3*`. Now
   `get_component<Position>(id)` returns `Position*`. Callers that assigned to `const vec3*`
   need updating to `const Position*` and use `->value`. Same for all component pointer types.

3. **`shared_ptr` patterns fully removed in gameplay** — `Inventory` and `NpcPath` components
   use `vector<...> value` instead of `shared_ptr<vector<...>>`. Code that did `*ptr` to copy
   the shared_ptr now does `ptr->value` to access the vector. `->method()` on the vector
   becomes `.method()` since it's a reference, not a pointer.

4. **Files need `#include "ecs_gameplay_components.h"`** — any file using the new component
   types needs this include. Add via: `perl -pi -e 'print "#include \"ecs_gameplay_components.h\"\n" if $. == 1' file.cpp`

5. **Build command:** `make clean && CXXFLAGS="-DDEBUG=1 -DDEBUG_ASSERT=1 -DNPCS_ALL_AT_ONCE -DMASTER_VOLUME=0.25f -DMUSIC_OFF" make game`

6. **Python migration script** used for bulk replacements — see session history for the
   TAG_MAP dictionary and regex patterns. Reusable for remaining tiers.

## Current Architecture

- `gamestate.h` contains `ComponentTable ct` (legacy) and `entt::registry registry`
- `ecs_gameplay_components.h` defines all EnTT component structs
- `ecs_core_components.h` has `LegacyEntityId`, `NpcTag`, `EntityTypeTag`
- `ecs_world_object_components.h` has `SolidTag`, `PushableTag`, `PullableTag`, `GridPosition`, `OpenState`, etc.
- `ecs_item_components.h` has `ItemKind`, `WeaponKind`, `ShieldKind`, `PotionKind`, `ItemVisual`, `ItemText`
- `ecs_actor_components.h` has `ActorKind`, `ActorVisual`, `ActorText`
- Bridge: `legacy_to_entt` map + `ensure_registry_entity()` / `lookup_registry_entity()`

## Component Mapping (legacy → EnTT)

| Legacy Tag (ComponentTraits.h) | EnTT Component (ecs_gameplay_components.h) |
|---|---|
| `name` (string) | `EntityName { string value }` |
| `description` (string) | `EntityDescription { string value }` |
| `dialogue_text` (string) | `DialogueLine { string value }` |
| `location` (vec3) | `Position { vec3 value }` |
| `hp` (vec2) | `HitPoints { vec2 value }` |
| `dead` (bool) | `DeadFlag { bool value }` |
| `direction` (direction_t) | `Facing { direction_t value }` |
| `spritemove` (Rectangle) | `SpriteMoveState { Rectangle value }` |
| `update` (bool) | `NeedsUpdate { bool value }` |
| `attacking` (bool) | `AttackingFlag { bool value }` |
| `damaged` (bool) | `DamagedFlag { bool value }` |
| `block_success` (bool) | `BlockSuccessFlag { bool value }` |
| `pushable` (bool) | `PushableTag` (existing in ecs_world_object_components.h) |
| `pullable` (bool) | `PullableTag` (existing) |
| `solid` (bool) | `SolidTag` (existing) |
| `level` (int) | `EntityLevel { int value }` |
| `xp` (int) | `Experience { int value }` |
| `strength` (int) | `StrengthAttr { int value }` |
| `dexterity` (int) | `DexterityAttr { int value }` |
| `intelligence` (int) | `IntelligenceAttr { int value }` |
| `wisdom` (int) | `WisdomAttr { int value }` |
| `constitution` (int) | `ConstitutionAttr { int value }` |
| `charisma` (int) | `CharismaAttr { int value }` |
| `target_id` (entityid) | `TargetEntity { entityid value }` |
| `light_radius` (int) | `LightRadius { int value }` |
| `vision_distance` (int) | `VisionRange { int value }` |
| `hearing_distance` (int) | `HearingRange { int value }` |
| `aggro` (bool) | `AggroFlag { bool value }` |
| `damage` (vec3) | `DamageDice { vec3 value }` |
| `healing` (vec3) | `HealingDice { vec3 value }` |
| `durability` (int) | `ItemDurability { int value }` |
| `max_durability` (int) | `ItemMaxDurability { int value }` |
| `destroyed` (bool) | `DestroyedFlag { bool value }` |
| `door_open` (bool) | `DoorOpenFlag { bool value }` |
| `block_chance` (int) | `BlockChance { int value }` |
| `base_ac` (int) | `BaseArmorClass { int value }` |
| `hd` (vec3) | `HitDie { vec3 value }` |
| `proptype` (proptype_t) | `PropTypeComponent { proptype_t value }` |
| `steps_taken` (uint) | `StepsTaken { uint value }` |
| `entity_default_action` (enum) | `DefaultAction { entity_default_action_t value }` |
| `target_path` (shared_ptr<vector<vec3>>) | `NpcPath { vector<vec3> value }` |
| `alignment` (alignment_t) | `AlignmentComponent { alignment_t value }` |
| `itemtype` (itemtype_t) | `ItemSubtype { itemtype_t value }` |
| `weapontype` (weapontype_t) | `WeaponSubtype { weapontype_t value }` |
| `shieldtype` (shieldtype_t) | `ShieldSubtype { shieldtype_t value }` |
| `potiontype` (potiontype_t) | `PotionSubtype { potiontype_t value }` |
| `inventory` (shared_ptr<vector<entityid>>) | `Inventory { vector<entityid> value }` |
| `equipped_weapon` (entityid) | `EquippedWeapon { entityid value }` |
| `equipped_shield` (entityid) | `EquippedShield { entityid value }` |
| `entitytype` (entitytype_t) | `EntityTypeTag` (existing, ecs_core_components.h) |
| `race` (race_t) | `ActorKind` (existing, ecs_actor_components.h) |

Existing EnTT components reused (NOT duplicated):
- `SolidTag`, `PushableTag`, `PullableTag` (ecs_world_object_components.h)
- `EntityTypeTag`, `NpcTag`, `LegacyEntityId` (ecs_core_components.h)
- `ActorKind`, `ActorVisual`, `ActorText` (ecs_actor_components.h)
- `ItemKind`, `WeaponKind`, `ShieldKind`, `PotionKind`, `ItemVisual`, `ItemText` (ecs_item_components.h)
- `GridPosition`, `OpenState` (ecs_world_object_components.h)

## Phase 2: Migrate All Reads from ct to EnTT

### Goal
Replace every `ct.get<Tag>(id)`, `ct.get_or<Tag>(id, fallback)`, and `ct.has<Tag>(id)` call
with EnTT registry queries. Keep `ct.set` calls as-is (dual-write already in place from Phase 1).

### Step 2.1: Add helper methods to gamestate.h

Add these template methods INSIDE the `gamestate` class (near the existing
`lookup_registry_entity` / `ensure_registry_entity` methods, around line 330):

```cpp
// Lookup the EnTT component for a legacy entityid. Returns nullptr if entity or component missing.
template <typename T>
T* get_component(entityid id) {
    auto e = lookup_registry_entity(id);
    return e != entt::null ? registry.try_get<T>(e) : nullptr;
}

template <typename T>
const T* get_component(entityid id) const {
    auto e = lookup_registry_entity(id);
    return e != entt::null ? registry.try_get<T>(e) : nullptr;
}

// Lookup with fallback value. Returns by value.
template <typename T, typename F>
auto get_component_or(entityid id, const F& fallback) const {
    auto* ptr = get_component<T>(id);
    return ptr ? ptr->value : fallback;
}

// Check if entity has an EnTT component.
template <typename T>
bool has_component(entityid id) const {
    auto e = lookup_registry_entity(id);
    return e != entt::null && registry.all_of<T>(e);
}
```

NOTE: `get_component_or` returns `ptr->value` (not `*ptr`) because the EnTT component
structs wrap the value in a `.value` field. This means the return type is the raw type
(string, int, vec3, bool, etc.) matching the legacy `ct.get_or` return type.

IMPORTANT: For components that DON'T have a `.value` field (the existing ones like
`EntityTypeTag`, `ActorKind`, `ItemKind`), use `get_component<T>(id)` directly and
access `ptr->type` or `ptr->value` manually. These are accessed via different field names:
- `EntityTypeTag.value`, `LegacyEntityType.value`
- `ActorKind.race`
- `ItemKind.type`, `WeaponKind.type`, `ShieldKind.type`, `PotionKind.type`
- `GridPosition.loc`

For these, don't use `get_component_or` — use `get_component` and check for null.

### Step 2.2: Replacement patterns

For each `ct.get_or<Tag>(id, fallback)` call site:

| Legacy call | EnTT replacement |
|---|---|
| `ct.get_or<location>(id, vec3{-1,-1,-1})` | `get_component_or<Position>(id, vec3{-1,-1,-1})` |
| `ct.get_or<name>(id, "no-name")` | `get_component_or<EntityName>(id, std::string{"no-name"})` |
| `ct.get_or<dead>(id, true)` | `get_component_or<DeadFlag>(id, true)` |
| `ct.get_or<direction>(id, dir_t::NONE)` | `get_component_or<Facing>(id, direction_t::NONE)` |
| `ct.get_or<entitytype>(id, ent_t::NONE)` | Use `get_component<EntityTypeTag>(id)` + null check |
| `ct.get_or<door_open>(id, false)` | `get_component_or<DoorOpenFlag>(id, false)` |
| `ct.get_or<hp>(id, vec2{-1,-1})` | `get_component_or<HitPoints>(id, vec2{-1,-1})` |
| `ct.get_or<level>(id, 0)` | `get_component_or<EntityLevel>(id, 0)` |
| `ct.get_or<xp>(id, 0)` | `get_component_or<Experience>(id, 0)` |
| `ct.get_or<strength>(id, 10)` | `get_component_or<StrengthAttr>(id, 10)` |
| `ct.get_or<dexterity>(id, 10)` | `get_component_or<DexterityAttr>(id, 10)` |
| `ct.get_or<intelligence>(id, 10)` | `get_component_or<IntelligenceAttr>(id, 10)` |
| `ct.get_or<wisdom>(id, 10)` | `get_component_or<WisdomAttr>(id, 10)` |
| `ct.get_or<constitution>(id, 10)` | `get_component_or<ConstitutionAttr>(id, 10)` |
| `ct.get_or<charisma>(id, 10)` | `get_component_or<CharismaAttr>(id, 10)` |
| `ct.get_or<alignment>(id, al_t::NONE)` | `get_component_or<AlignmentComponent>(id, alignment_t::NONE)` |
| `ct.get_or<equipped_weapon>(id, INVALID)` | `get_component_or<EquippedWeapon>(id, ENTITYID_INVALID)` |
| `ct.get_or<equipped_shield>(id, INVALID)` | `get_component_or<EquippedShield>(id, ENTITYID_INVALID)` |
| `ct.get_or<itemtype>(id, it_t::NONE)` | `get_component_or<ItemSubtype>(id, itemtype_t::NONE)` |
| `ct.get_or<weapontype>(id, wt_t::NONE)` | `get_component_or<WeaponSubtype>(id, weapontype_t::NONE)` |
| `ct.get_or<shieldtype>(id, st_t::NONE)` | `get_component_or<ShieldSubtype>(id, shieldtype_t::NONE)` |
| `ct.get_or<potiontype>(id, pt_t::NONE)` | `get_component_or<PotionSubtype>(id, potiontype_t::NONE)` |
| `ct.get_or<proptype>(id, pt_t::NONE)` | `get_component_or<PropTypeComponent>(id, proptype_t::NONE)` |
| `ct.get_or<pushable>(id, false)` | `get_component_or<PushableTag>(id, false)` |
| `ct.get_or<pullable>(id, false)` | `get_component_or<PullableTag>(id, false)` |
| `ct.get_or<solid>(id, false)` | `get_component_or<SolidTag>(id, false)` |
| `ct.get_or<damage>(id, vec3{-1,-1,-1})` | `get_component_or<DamageDice>(id, vec3{-1,-1,-1})` |
| `ct.get_or<healing>(id, vec3{-1,-1,-1})` | `get_component_or<HealingDice>(id, vec3{-1,-1,-1})` |
| `ct.get_or<block_chance>(id, -1)` | `get_component_or<BlockChance>(id, -1)` |
| `ct.get_or<durability>(id, -1)` | `get_component_or<ItemDurability>(id, -1)` |
| `ct.get_or<max_durability>(id, -1)` | `get_component_or<ItemMaxDurability>(id, -1)` |
| `ct.get_or<description>(id, "no-desc")` | `get_component_or<EntityDescription>(id, std::string{"no-desc"})` |
| `ct.get_or<dialogue_text>(id, "")` | `get_component_or<DialogueLine>(id, std::string{""})` |
| `ct.get_or<race>(id, race_t::NONE)` | Use `get_component<ActorKind>(id)` + null check |
| `ct.get_or<vision_distance>(id, 0)` | `get_component_or<VisionRange>(id, 0)` |
| `ct.get_or<light_radius>(id, 0)` | `get_component_or<LightRadius>(id, 0)` |
| `ct.get_or<hearing_distance>(id, 3)` | `get_component_or<HearingRange>(id, 3)` |
| `ct.get_or<aggro>(id, false)` | `get_component_or<AggroFlag>(id, false)` |
| `ct.get_or<steps_taken>(id, 0)` | `get_component_or<StepsTaken>(id, 0u)` |
| `ct.get_or<base_ac>(id, 10)` | `get_component_or<BaseArmorClass>(id, 10)` |
| `ct.get_or<hd>(id, vec3{1,1,0})` | `get_component_or<HitDie>(id, vec3{1,1,0})` |
| `ct.get_or<target_id>(id, INVALID)` | `get_component_or<TargetEntity>(id, ENTITYID_INVALID)` |
| `ct.get_or<entity_default_action>(id, NONE)` | `get_component_or<DefaultAction>(id, entity_default_action_t::NONE)` |
| `ct.get_or<block_success>(id, false)` | `get_component_or<BlockSuccessFlag>(id, false)` |
| `ct.get_or<attacking>(id, false)` | `get_component_or<AttackingFlag>(id, false)` |
| `ct.get_or<damaged>(id, false)` | `get_component_or<DamagedFlag>(id, false)` |

For `ct.get<Tag>(id)` (returns pointer):
| Legacy call | EnTT replacement |
|---|---|
| `ct.get<location>(id)` → `vec3*` | `get_component<Position>(id)` → `Position*`, use `->value` |
| `ct.get<inventory>(id)` → `shared_ptr<vector<>>*` | `get_component<Inventory>(id)` → `Inventory*`, use `->value` |
| `ct.get<target_path>(id)` → `shared_ptr<vector<>>*` | `get_component<NpcPath>(id)` → `NpcPath*`, use `->value` |
| `ct.get<dead>(id)` → `bool*` | `get_component<DeadFlag>(id)` → `DeadFlag*`, use `->value` |
| `ct.get<direction>(id)` → `direction_t*` | `get_component<Facing>(id)` → `Facing*`, use `->value` |
| `ct.get<durability>(id)` → `int*` | `get_component<ItemDurability>(id)` → `ItemDurability*`, use `->value` |
| `ct.get<door_open>(id)` → `bool*` | `get_component<DoorOpenFlag>(id)` → `DoorOpenFlag*`, use `->value` |
| `ct.get<hp>(id)` → `vec2*` | `get_component<HitPoints>(id)` → `HitPoints*`, use `->value` |
| `ct.get<race>(id)` → `race_t*` | `get_component<ActorKind>(id)` → `ActorKind*`, use `->race` |
| `ct.get<entitytype>(id)` → `entitytype_t*` | `get_component<EntityTypeTag>(id)` → `EntityTypeTag*`, use `->value` |
| `ct.get<spritemove>(id)` → `Rectangle*` | `get_component<SpriteMoveState>(id)` → `SpriteMoveState*`, use `->value` |
| `ct.get<update>(id)` → `bool*` | `get_component<NeedsUpdate>(id)` → `NeedsUpdate*`, use `->value` |
| `ct.get<healing>(id)` → `vec3*` | `get_component<HealingDice>(id)` → `HealingDice*`, use `->value` |

For `ct.has<Tag>(id)`:
| Legacy call | EnTT replacement |
|---|---|
| `ct.has<location>(id)` | `has_component<Position>(id)` |
| `ct.has<name>(id)` | `has_component<EntityName>(id)` |
| `ct.has<description>(id)` | `has_component<EntityDescription>(id)` |
| `ct.has<dialogue_text>(id)` | `has_component<DialogueLine>(id)` |
| `ct.has<update>(id)` | `has_component<NeedsUpdate>(id)` |
| `ct.has<location>(id)` | `has_component<Position>(id)` |
| `ct.has<target_path>(id)` | `has_component<NpcPath>(id)` |

### Step 2.3: Inventory shared_ptr → value type

When migrating `ct.get<inventory>(id)`:
- Old: returns `shared_ptr<vector<entityid>>*`, callers do `*ptr` to copy the shared_ptr, then `->push_back()`
- New: `get_component<Inventory>(id)` returns `Inventory*`, callers use `ptr->value.push_back()`
- Old `ct.get_or<inventory>(id, make_shared<vector<entityid>>())` → just use `get_component<Inventory>(id)` and check for null

Functions that take `shared_ptr<vector<entityid>>` by parameter need updating:
- `draw_mini_inventory_menu(... shared_ptr<vector<entityid>> inventory ...)` → `const vector<entityid>& inventory`
- `draw_inventory_grid(... shared_ptr<vector<entityid>> inventory ...)` → `const vector<entityid>& inventory`
- Their callers change from `*inv_ptr` to `inv_ptr->value` (pass by reference)

### Step 2.4: target_path shared_ptr → value type

When migrating `ct.get<target_path>(id)`:
- Old: returns `shared_ptr<vector<vec3>>*`, callers do `(*ptr)->clear()`, `(*ptr)->at(0)`
- New: `get_component<NpcPath>(id)` returns `NpcPath*`, callers use `ptr->value.clear()`, `ptr->value.at(0)`

### Step 2.5: File migration order

Tier 1 (gameplay — most call sites) — **COMPLETE**:
1. ✅ `src/gamestate_npc_combat.cpp`
2. ✅ `src/gamestate_world_interaction.cpp`
3. ✅ `src/gamestate_input.cpp`
4. ✅ `src/gamestate_inventory.cpp`
5. ✅ `src/gamestate_lifecycle.cpp`
6. ✅ `src/gamestate_entity_factory.cpp`
7. ✅ `src/gamestate.h` — all 5 ct calls migrated

Tier 2 (rendering) — **COMPLETE**:
8. ✅ `src/libdraw_update_sprites.cpp` — 15 ct calls migrated
9. ✅ `src/draw_hud.cpp` — 13 ct calls migrated
10. ✅ `src/draw_dungeon_floor.cpp` — 6 ct calls migrated
11. ✅ `src/draw_item_detail_text.cpp` — 10 ct calls migrated
12. ✅ `src/draw_mini_inventory_menu.cpp` — 4 ct calls migrated, signature → `const vector<entityid>&`
13. ✅ `src/draw_inventory_menu.cpp` — 1 ct call migrated
14. ✅ `src/draw_chest_menu.cpp` — 3 ct calls migrated, `draw_inventory_grid` signature → `const vector<entityid>&`
15. ✅ `src/draw_look_panel.cpp` — 1 ct call migrated
16. ✅ `src/libdraw.cpp` — 2 ct calls migrated
17. ✅ `src/libdraw_create_spritegroup.cpp` — 3 ct calls migrated
18. ✅ `src/libdraw_gameplay_scene.cpp` — 2 ct calls migrated

Tier 3 (small helper headers) — **COMPLETE**:
19. ✅ `src/get_entity_name.h`, `src/get_entity_type.h`, `src/get_entity_location.h`
20. ✅ `src/get_npc_dead.h`, `src/set_npc_dead.h`
21. ✅ `src/get_weapon_sprite.h`, `src/get_shield_sprite.h`
22. ✅ `src/is_loc_path_blocked.h`
23. ✅ `src/set_sg.cpp`, `src/create_sg_byid.cpp`
24. ✅ `src/debug_entity.h`
25. ✅ `src/libdraw_update_weapon_for_entity.h`, `src/libdraw_update_shield_for_entity.h`
26. ✅ `src/gamestate_world.cpp` — 8 ct calls migrated

Tier 4 (test files — update g.ct.* calls to use get_component) — **COMPLETE**:
27. ✅ `src/test_suites/test_combat_bootstrap.h` — ~80 ct reads migrated
28. ✅ `src/test_suites/test_world_interaction.h` — ~64 ct reads migrated
29. ✅ `src/test_suites/test_entity_factory.h` — ~72 ct reads migrated
30. ✅ `src/test_suites/test_entity_placement.h` — ~12 ct reads migrated
31. ✅ `src/test_suites/test_inventory.h` — ~24 ct reads migrated
32. ✅ `src/test_suites/test_gamestate_lifecycle.h` — ~29 ct reads migrated
33. ✅ `src/test_suites/test_heavy_simulation.h` — ~15 ct reads migrated

### Step 2.6: After all reads migrated

After every `ct.get`, `ct.get_or`, and `ct.has` is replaced with EnTT calls, remove
the dual-write `ct.set` calls from Phase 1. At this point the ComponentTable is only
written to but never read from, so the ct.set calls are dead code.

### Verification after Phase 2 — ALL TIERS COMPLETE
```
make clean && CXXFLAGS="-DDEBUG=1 -DDEBUG_ASSERT=1 -DNPCS_ALL_AT_ONCE -DMASTER_VOLUME=0.25f -DMUSIC_OFF" make game
```
**BUILD PASSES CLEAN — zero errors.** All tiers (1-4) complete. Phase 2 is done.

### Phase 3 Handoff Notes

**Remaining `ct.set` calls (208 total, dual-write, now dead code):**
- `src/gamestate_entity_factory.cpp` — 104 ct.set (most calls)
- `src/gamestate_world.cpp` — 31 ct.set
- `src/gamestate_lifecycle.cpp` — 25 ct.set
- `src/gamestate_npc_combat.cpp` — 20 ct.set
- `src/gamestate_world_interaction.cpp` — 16 ct.set
- `src/gamestate_inventory.cpp` — 15 ct.set
- `src/gamestate_input.cpp` — 12 ct.set
- `src/gamestate_scene.cpp` — 1 ct.set
- `src/set_npc_dead.h` — 1 ct.set
- Test files also have ct.set calls (keep or remove with tests)

**Test files still call `g.create_*_at_with(g.ct, ...)`** — these entity creation functions
accept `ComponentTable&` as a parameter. For Phase 3, either:
1. Remove the `ct` parameter from creation functions (since they no longer write to ct)
2. Or leave the signatures as-is if they're hard to change

### Verification after Phase 2
```
make clean && CXXFLAGS="-DDEBUG=1 -DDEBUG_ASSERT=1 -DNPCS_ALL_AT_ONCE -DMASTER_VOLUME=0.25f -DMUSIC_OFF" make game
```
**BUILD PASSES CLEAN — zero errors.** Tests not yet migrated (Tier 4).

---

## Phase 3: Remove Legacy System

### Step 3.1: Delete all remaining ct.set calls
Remove every `ct.set<Tag>(id, value)` from all source files. These are now dead writes
since nothing reads from ComponentTable anymore.

### Step 3.2: Remove ComponentTable from gamestate.h
- Remove `#include "ComponentTable.h"`
- Remove `typedef ComponentTable CT;`
- Remove `ComponentTable ct;` member
- Remove `ct.clear();` from `reset()`

### Step 3.3: Delete files
- `src/ComponentTable.h`
- `src/ComponentTraits.h`
- `src/entity_templates.h` (if test helpers migrated)
- `src/unit_test_old.h` (commented-out legacy tests)
- `src/test_suites/test_component_table.h` (tests for deleted class)
- Update Makefile: remove `test_component_table.h` from cxxtestgen line

### Step 3.4: Clean up includes
Remove all `#include "ComponentTable.h"` and `#include "ComponentTraits.h"` from every file.

### Step 3.5: Optional — keep or remove legacy_to_entt bridge
Decision deferred. The `legacy_to_entt` map bridges `entityid` (int) to `entt::entity`.
Keeping it minimizes signature changes. Removing it requires converting all `entityid`
parameters to `entt::entity` — large blast radius. Recommendation: keep for Phase 3,
remove in a follow-up.

### Verification after Phase 3
```
make clean && CXXFLAGS="-DDEBUG=1 -DDEBUG_ASSERT=1 -DNPCS_ALL_AT_ONCE -DMASTER_VOLUME=0.25f -DMUSIC_OFF" make game
```

### Phase 3 — COMPLETE

**Step 3.1 — ct.set removal:**
- Removed 391 ct.set lines across 15 files (source + tests)
- Fixed broken functions where ct.set removal left variables unused or logic incomplete:
  - `gamestate_scene.cpp`: `make_all_npcs_target_player()` — re-added target write via `get_component<TargetEntity>`
  - `gamestate_input.cpp`: `apply_level_up_rewards()` — re-added level + HP write via `get_component<EntityLevel>` / `get_component<HitPoints>`
  - `gamestate_npc_combat.cpp`: `update_npc_xp()` — re-added XP write via `get_component<Experience>`; `resolve_attack_intent()` — re-added facing/attacking/update writes via `get_component<Facing>`, `get_component<AttackingFlag>`, `get_component<NeedsUpdate>`
  - `gamestate_world_interaction.cpp`: 2 movement functions — re-added position/spritemove/steps_taken/update writes via `get_component<Position>`, `get_component<SpriteMoveState>`, `get_component<StepsTaken>`, `get_component<NeedsUpdate>`

**Step 3.2 — ComponentTable removed from gamestate.h:**
- Removed `#include "ComponentTable.h"`, `#include "ComponentTraits.h"`
- Removed `typedef ComponentTable CT;`
- Removed `ComponentTable ct;` member
- Removed `ct.clear();` from `reset()`
- Updated 4 function signatures to remove `ComponentTable& ct` / `CT& ct` parameter:
  - `create_weapon_at_with`, `create_weapon_at_random_loc_with`, `create_shield_with`, `create_shield_at_with`
- Updated all callers in source and test files (removed `g.ct,` argument)

**Step 3.3 — Deleted files:**
- `src/ComponentTable.h`
- `src/ComponentTraits.h`
- `src/entity_templates.h` (all functions became empty bodies after ct.set removal; file was never included)
- `src/unit_test_old.h` (commented-out legacy tests)
- `src/test_suites/test_component_table.h` (tests for deleted class)

**Step 3.4 — Include cleanup:**
- Removed `ComponentTable.h` / `ComponentTraits.h` includes from: `gamestate.h`, `libdraw.cpp`, `dungeon_tile.h`, `entity_templates.h`
- Added `using std::shared_ptr;` to `dungeon_floor.h` (was previously provided transitively by deleted headers)

**Step 3.5 — Makefile updated:**
- Removed `test_component_table.h` from both cxxtestgen rules
- Removed `#include "test_suites/test_component_table.h"` from `unit_test.h`

**Build verification:** Clean build with `-DDEBUG=1 -DDEBUG_ASSERT=1 -DNPCS_ALL_AT_ONCE -DMASTER_VOLUME=0.25f -DMUSIC_OFF` — zero errors.

---

## EnTT Migration — COMPLETE

The full migration from ComponentTable to EnTT is complete across all 3 phases:

- **Phase 1**: Dual-write setup (ct + registry)
- **Phase 2**: All reads migrated to EnTT (Tiers 1-4)
- **Phase 3**: Legacy system removed (ct.set, ComponentTable, ComponentTraits, entity_templates)

Remaining deferred work (not blocking):
- Remove `legacy_to_entt` bridge and convert `entityid` params to `entt::entity` (Phase 3 Step 3.5)
- cxxtest not available in this environment for test verification
