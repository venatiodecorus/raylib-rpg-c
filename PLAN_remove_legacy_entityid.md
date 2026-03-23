# Plan: Remove Legacy Entity System (`entityid` → `entt::entity`)

## Background

The codebase completed Phase 3 of the EnTT migration (removing `ComponentTable`), but the
legacy `entityid` integer type (`typedef int entityid`) remains as the primary entity
identifier everywhere. A bridge layer in `gamestate.h` converts between `entityid` and
`entt::entity` on every component access via `lookup_registry_entity()` and the
`legacy_to_entt` unordered map.

**Scale:** ~812 occurrences of `entityid` across 52 files (non-test). Zero places use
`entt::entity` as a primary identifier outside the bridge.

## Phase 1: Replace dirty-entity range with vector

The dirty-entity range (`new_entityid_begin` to `new_entityid_end`) is used to iterate
newly created entities for sprite creation. This integer-range iteration is incompatible
with `entt::entity`. Replace with `std::vector<entt::entity> new_entities`.

| File | Change |
|------|--------|
| `gamestate.h` | Replace `new_entityid_begin`, `new_entityid_end`, `dirty_entities` with `std::vector<entt::entity> new_entities` |
| `gamestate.h` `add_entity()` | Push the created entity onto `new_entities` |
| `gamestate.h` `init()` / `reset()` | Clear `new_entities` instead of setting sentinels |
| `libdraw_update_sprites.cpp:130-140` | Iterate `g.new_entities` instead of integer range |
| `gamestate_lifecycle.cpp:262-264` | Clear `new_entities` instead of resetting sentinels |
| `character_creation_scene.cpp:65-67` | Populate `new_entities` from all existing entities |
| `test_gamestate_lifecycle.h` | Update assertions for new_entities vector |

## Phase 2: Delete `#ifndef NPCS_ALL_AT_ONCE` code paths and `entity_turn`

Since `NPCS_ALL_AT_ONCE` is always defined, the `#ifndef` paths are dead code.

| File | Change |
|------|--------|
| `gamestate_lifecycle.cpp:216-221, 225-236` | Delete the `#ifndef` blocks, keep only `#else` content |
| `gamestate_npc_combat.cpp:480-489` | Delete the `#ifndef` block, keep lines 491-502 |
| `gamestate.h` | Remove `entity_turn` and `next_entityid` members |
| `character_creation_scene.cpp:61` | Assign directly to `hero_id` instead of `entity_turn` |
| Test files | Update references to `entity_turn` and `next_entityid` |

## Phase 3: Alias swap — `entityid` becomes `entt::entity`

The keystone change. After Phases 1-2 remove all integer arithmetic on entityid:

| File | Change |
|------|--------|
| `entityid.h` | `typedef int entityid` → `using entityid = entt::entity` |
| `entityid.h` | `ENTITYID_INVALID` → `entt::null`, `INVALID` → `entt::null` |
| `gamestate.h` `add_entity()` | Use `registry.create()` instead of incrementing integer |

**Expected cascading fixes:**
- `%d` format specifiers printing entityids → cast via `entt::to_integral()`
- Literal `-1` comparisons → `entt::null` / `ENTITYID_INVALID`
- Tile cache sentinels
- Test assertions comparing entityid to integers

## Phase 4: Remove the bridge layer

Now that `entityid` IS `entt::entity`, the bridge is identity:

| Component | Change |
|-----------|--------|
| `legacy_to_entt` map | Delete |
| `lookup_registry_entity()` | Replace with `registry.valid(id) ? id : entt::null` or inline |
| `ensure_registry_entity()` | Delete — `add_entity()` already creates the registry entity |
| `has_registry_entity()` | Replace with `registry.valid(id)` |
| `get_component<T>(entityid)` | Simplify to `registry.try_get<T>(id)` |
| `has_component<T>(entityid)` | Simplify to `registry.valid(id) && registry.all_of<T>(id)` |
| `LegacyEntityId` component | Delete from `ecs_core_components.h` |
| `for_entities_of_type()` | Iterate `view<EntityTypeTag>` directly |

## Phase 5: Clean up `LegacyEntityId` usage in views

All `registry.view<LegacyEntityId, ...>()` patterns become simpler since entity IS entityid:

```cpp
// Before:
auto view = g.registry.view<LegacyEntityId, NpcTag>();
for (auto entity : view) {
    entityid id = view.get<LegacyEntityId>(entity).id;
    handle_npc(id);
}

// After:
auto view = g.registry.view<NpcTag>();
for (auto entity : view) {
    handle_npc(entity);
}
```

Files: `gamestate_scene.cpp`, `gamestate_lifecycle.cpp`, `gamestate_npc_combat.cpp`,
`libdraw_update_sprites.cpp`, test suites.

## Phase 6: Update factory callbacks and entity creation

- Factory methods: remove redundant `ensure_registry_entity()` calls after `add_entity()`
- `with_fun` signature works as-is since `entityid = entt::entity`
- Remove stale `AGENTS.md` reference to deleted `ComponentTable`
- Update `TODO.md` line 194 to mark done

## Phase 7: Test and validate

1. `make clean && make tests && ./tests`
2. `make clean && make tests_heavy && ./tests_heavy`
3. `make clean && CXXFLAGS="-DDEBUG_ASSERT=1 -DNPCS_ALL_AT_ONCE -DDEBUG=1 -DMASTER_VOLUME=0.25f" make game`
4. Run game, create character, verify gameplay

## Summary

| Phase | Description | Files | Risk |
|-------|-------------|-------|------|
| 1 | Replace dirty-entity range with vector | 4 | Low |
| 2 | Delete `#ifndef NPCS_ALL_AT_ONCE` paths | 3-5 | Low |
| 3 | Alias swap: `entityid` = `entt::entity` | 2 + cascading | Medium |
| 4 | Remove bridge layer | ~5 | Medium |
| 5 | Clean up `LegacyEntityId` views | ~8 | Low |
| 6 | Simplify factory/creation flow | ~4 | Low |
| 7 | Test & validate | — | — |
