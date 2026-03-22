# Phase 2 — Entity Class Architecture

## Context

Phase 1 cleanup is complete (branch merged to master). We removed ~50 dead files,
consolidated 6 headers into 3, moved .frag shaders to `src/shaders/`, and removed
all build-stats infrastructure. The codebase is now 120 headers + 48 .cpp files,
~18,800 lines.

Phase 2 tackles the core architectural problem: each entity type (actor, item, door,
chest, box, prop) has its logic scattered across 8-12 files. The goal is to group
entity concerns (definition, components, creation, interaction, sprites, drawing)
into cohesive entity classes.

## Current Entity Architecture

### Entity Types (entitytype_t)
`PLAYER`, `NPC`, `ITEM` (weapon/shield/potion), `BOX`, `CHEST`, `DOOR`, `PROP` (15 subtypes)

### The Scattering Problem

A Door's code is spread across:
- `entitytype.h` — type enum
- `world_object_definitions.h/.cpp` — `kWoodenDoorDefinition` data
- `ecs_world_object_components.h` — `OpenState`, `SolidTag` components
- `ecs_gameplay_components.h` — `DoorOpenFlag` component
- `gamestate_world.cpp` — `create_door_with()`, `place_doors()` factory
- `gamestate_world_interaction.cpp` — `try_entity_open_door()`, pressure plate linkage
- `gamestate_lifecycle.cpp` — `path_blocked()` door checks
- `create_sg_byid.cpp` — `create_door_sg_byid()` sprite creation
- `libdraw_update_sprites.cpp` — door open/close sprite swap (line 110-115)
- `draw_dungeon_floor.cpp` — rendering (currently disabled per TODO)

Every entity type has the same scattering pattern.

### Dual-ID System
- Legacy `entityid` (int, incrementing) — used by tile cache, dungeon floor, game logic
- EnTT `entt::entity` — created lazily via `gamestate::ensure_registry_entity(id)`
- `LegacyEntityId` component bridges them
- `gamestate` owns the registry and entity ID allocation

### Data Flow Duplication
Definition tables store sprite/name data → mirrored into EnTT components at creation
time (`mirror_item_common`, `attach_static_world_definition`) → read back during
sprite creation. A class could own the definition pointer directly.

### Current File Layout (post Phase 1)

```
src/
  gamestate.h                    — Central class (~1450 lines), owns registry
  gamestate_entity_factory.cpp   — NPC/item factory methods (547 lines)
  gamestate_world.cpp            — Door/chest/box/prop creation + dungeon gen (704 lines)
  gamestate_world_interaction.cpp— Movement, interaction, pickup, stairs (1362 lines)
  gamestate_lifecycle.cpp        — Per-tick updates, visibility, NPC state
  gamestate_npc_combat.cpp       — Combat resolution, AI, pathfinding
  gamestate_inventory.cpp        — Inventory, chest menu, equip/use/drop
  create_sg_byid.cpp             — Sprite group creation dispatch (174 lines)
  libdraw_update_sprites.cpp     — Per-frame sprite updates
  draw_dungeon_floor.cpp         — Floor/entity rendering (209 lines)
  actor_definitions.h/.cpp       — ActorDefinition structs, 13 race definitions
  item_definitions.h/.cpp        — ItemDefinition structs (weapons/shields/potions)
  world_object_definitions.h/.cpp— StaticWorldDefinition (door/chest/box/prop)
  prop_definitions.h/.cpp        — PropDefinition alias for StaticWorldDefinition
  ecs_core_components.h          — LegacyEntityId, NpcTag, EntityTypeTag
  ecs_actor_components.h         — ActorKind, ActorVisual, ActorText
  ecs_item_components.h          — ItemKind, WeaponKind, ShieldKind, PotionKind, ItemVisual, ItemText
  ecs_world_object_components.h  — StaticVisual, OpenState, SolidTag, PushableTag, etc.
  ecs_gameplay_components.h      — 35+ gameplay state components (261 lines)
  dungeon_tile.h                 — tile_t with cached entity IDs per tile
  dungeon_floor.h                — Spatial floor, df_add_at/df_remove_at
  spritegroup.h                  — Sprite group system
  libdraw_context.h              — rpg::Renderer class
```

### Key Patterns to Preserve
- `gamestate` owns the EnTT registry and entity ID allocation
- Tile cache system (`tile_t` cached entity fields) for spatial queries
- `dungeon_floor::df_add_at/df_remove_at` manages spatial indexing
- `with_fun` callback pattern for entity creation customization
- `spritegroup` / `create_sg` sprite system
- Event queue (`gameplay_event_t`, `process_gameplay_event`) for deferred actions

## Proposed Entity Class Structure

```
src/entities/
  entity_base.h          — Common interface / CRTP base
  actor.h / actor.cpp    — NPC + Player
  item.h / item.cpp      — Weapon + Shield + Potion
  door.h / door.cpp      — Open/close, pressure plates
  chest.h / chest.cpp    — Inventory container
  box.h / box.cpp        — Push/pull physics
  prop.h / prop.cpp      — 15 prop subtypes
```

### What Each Entity Class Owns

| Concern | Current Location | Moves To |
|---------|-----------------|----------|
| Definition data | `*_definitions.h/.cpp` | Entity class (or kept as data, referenced) |
| Component setup | `gamestate_entity_factory.cpp`, `gamestate_world.cpp` | `Entity::create()` |
| Init callbacks | `gamestate_world.cpp` (lambdas) | `Entity::init()` |
| Interaction logic | `gamestate_world_interaction.cpp` | `Entity::try_interact()` |
| Sprite creation | `create_sg_byid.cpp` | `Entity::create_sprite()` |
| Sprite update | `libdraw_update_sprites.cpp` | `Entity::update_sprite()` |
| Movement rules | `gamestate_world_interaction.cpp` | `Entity::on_move()` / `Entity::blocks()` |

### Entity Base Interface (Draft)

```cpp
namespace rpg::entities {

struct CreateContext {
    gamestate& g;
    entityid id;
    entt::entity e;  // registry entity (already ensured)
};

class EntityBase {
public:
    virtual ~EntityBase() = default;

    // Factory: create entity, set up EnTT components, return entityid
    virtual entityid create(gamestate& g, vec3 loc) = 0;

    // Rendering: create spritegroup for this entity
    virtual void create_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) = 0;

    // Per-frame: update sprite state (animation, direction, open/close swap)
    virtual void update_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) = 0;

    // Interaction: handle player interacting with this entity
    virtual bool try_interact(gamestate& g, entityid actor_id, entityid target_id) = 0;

    // Movement: does this entity block movement?
    virtual bool is_solid() const = 0;
};

} // namespace rpg::entities
```

### Entity Classes

#### Actor (NPC + Player)
**Absorbs:** `actor_definitions.h/.cpp`, NPC creation from `gamestate_entity_factory.cpp`,
combat from `gamestate_npc_combat.cpp`, NPC state from `gamestate_lifecycle.cpp`,
actor sprite creation from `create_sg_byid.cpp`

- 13 races with definitions (stats, modifiers, hit dice, alignment)
- 6 AI behaviors (`entity_actions.h`: NONE, RANDOM_MOVE, MOVE_TO_TARGET, etc.)
- Combat system (attack roll, damage, block, death, XP)
- Pathfinding (A* in `pathfinder.h`)
- Stat rolling, leveling

**Key files consumed:**
- `actor_definitions.h/.cpp` → definition data stays, class wraps access
- `gamestate_entity_factory.cpp` lines 60-200+ (create_npc_with, create_player_at_with)
- `gamestate_npc_combat.cpp` (combat resolution, AI dispatch)
- `gamestate_lifecycle.cpp` (update_npcs_state, update_npc_behavior)
- `create_sg_byid.cpp` lines 14-27 (create_actor_sg_from_registry)

#### Item (Weapon + Shield + Potion)
**Absorbs:** `item_definitions.h/.cpp`, item creation from `gamestate_entity_factory.cpp`,
inventory logic from `gamestate_inventory.cpp`, item sprite creation from `create_sg_byid.cpp`

- 3 sub-classes: Weapon (damage dice), Shield (block chance, durability), Potion (healing dice)
- Equip/unequip, use, drop, pickup, transfer
- Durability system

**Key files consumed:**
- `item_definitions.h/.cpp` → definition data stays
- `gamestate_entity_factory.cpp` lines 53-150 (create_weapon_with, create_shield_with, create_potion_with)
- `gamestate_inventory.cpp` (equip, use, drop, chest transfer)
- `create_sg_byid.cpp` lines 29-80 (create_item_sg_from_registry and sub-dispatches)

#### Door
**Absorbs:** door parts of `world_object_definitions.h/.cpp`, `gamestate_world.cpp`,
`gamestate_world_interaction.cpp`, `create_sg_byid.cpp`, `libdraw_update_sprites.cpp`

- Open/close toggle
- Pressure plate linkage (auto-open/close)
- Movement blocking when closed
- Sprite swap on state change

**Key files consumed:**
- `world_object_definitions.h/.cpp` (`kWoodenDoorDefinition`)
- `gamestate_world.cpp` (create_door_with, place_doors, pressure plate setup)
- `gamestate_world_interaction.cpp` (try_entity_open_door, pressure plate refresh)
- `create_sg_byid.cpp` (create_door_sg_byid)
- `libdraw_update_sprites.cpp` lines 110-115 (door open/close sprite swap)

#### Chest
**Absorbs:** chest parts of same files + chest inventory UI

- Open/close toggle
- Inventory container
- Chest menu UI trigger
- Item transfer

**Key files consumed:**
- `world_object_definitions.h/.cpp` (`kTreasureChestDefinition`)
- `gamestate_world.cpp` (create_chest_with, place_first_floor_chest)
- `gamestate_world_interaction.cpp` (try_entity_open_chest)
- `gamestate_inventory.cpp` (open_chest_menu, close_chest_menu, transfer_inventory_item)
- `create_sg_byid.cpp` (create_chest_sg_byid)

#### Box
**Absorbs:** box parts of same files

- Push/pull physics
- Movement blocking

**Key files consumed:**
- `gamestate_entity_factory.cpp` (create_box_with, create_box_at_with)
- `gamestate_world_interaction.cpp` (handle_box_push, try_entity_push, try_entity_pull)
- `create_sg_byid.cpp` (create_box_sg_byid)

#### Prop
**Absorbs:** `prop_definitions.h/.cpp`, prop parts of same files

- 15 subtypes (banner, statue, torch, candle, jar, barrel, chair, table, sign, etc.)
- Interact text
- Some push/pull-able

**Key files consumed:**
- `prop_definitions.h/.cpp`
- `world_object_definitions.h/.cpp` (prop definitions)
- `gamestate_world.cpp` (create_prop_with, place_props, dungeon_prop_init)
- `gamestate_world_interaction.cpp` (try_entity_interact for props)
- `create_sg_byid.cpp` (create_prop_sg_byid)

## Implementation Plan

### Step 0: Create `src/entities/` directory and base class
- Create `src/entities/entity_base.h` with the interface above
- Add `src/entities/` to include path in Makefile
- No existing code changes yet — just the foundation

### Step 1: Box (simplest entity — proof of concept)
- Create `src/entities/box.h` and `src/entities/box.cpp`
- Move `create_box_with`/`create_box_at_with` from `gamestate_entity_factory.cpp`
- Move `handle_box_push`, `try_entity_push`, `try_entity_pull` from `gamestate_world_interaction.cpp`
- Move `create_box_sg_byid` from `create_sg_byid.cpp`
- Wire `gamestate` to delegate box operations to `rpg::entities::Box`
- Update Makefile to compile new .cpp
- Verify build + tests

### Step 2: Door (moderate complexity, clear boundaries)
- Create `src/entities/door.h` and `src/entities/door.cpp`
- Move `create_door_with`/`create_door_at_with` from `gamestate_world.cpp`
- Move `try_entity_open_door` from `gamestate_world_interaction.cpp`
- Move `create_door_sg_byid` from `create_sg_byid.cpp`
- Move door sprite swap logic from `libdraw_update_sprites.cpp`
- Handle pressure plate linkage (either keep in world_interaction or delegate)
- Verify build + tests

### Step 3: Chest (adds inventory layer)
- Create `src/entities/chest.h` and `src/entities/chest.cpp`
- Move chest creation, interaction, menu, transfer logic
- Chest menu UI can stay in `draw_chest_menu.cpp` but chest class provides the data
- Verify build + tests

### Step 4: Prop (many subtypes, thin logic)
- Create `src/entities/prop.h` and `src/entities/prop.cpp`
- Move `prop_definitions.h/.cpp` content
- Move prop creation, interaction, sprite creation
- Verify build + tests

### Step 5: Item (introduces sub-classes)
- Create `src/entities/item.h` and `src/entities/item.cpp`
- Move item creation, mirror logic, sprite creation
- Keep inventory UI in `draw_inventory_menu.cpp` but item class provides data
- Consider Weapon/Shield/Potion as sub-classes or separate files
- Verify build + tests

### Step 6: Actor (most complex — last)
- Create `src/entities/actor.h` and `src/entities/actor.cpp`
- Move NPC/player creation, combat, AI dispatch
- Keep combat UI (damage numbers, action menu) in existing draw files
- Verify build + tests

### Step 7: Cleanup
- Remove now-empty source files
- Clean up `gamestate.h` method surface (delegate to entity classes)
- Remove redundant `mirror_*` functions
- Remove `create_sg_byid.cpp` dispatch (each entity handles its own sprite creation)
- Verify build + tests

## Files That Stay As-Shared-Infrastructure

These systems cut across all entity types and remain unchanged:

| File | Role |
|------|------|
| `gamestate.h` | EnTT registry, entity ID management, tile cache coordination |
| `dungeon_floor.h` / `dungeon_tile.h` | Spatial index, tile cache |
| `spritegroup.h` / `sprite_def.h` | Sprite system |
| `libdraw_context.h` | Renderer state |
| `draw_dungeon_floor.cpp` | Render loop (calls entity draw methods) |
| `libdraw_gameplay_scene.cpp` | Scene composition |
| `ecs_*.h` | Component structs (entity classes write to them) |
| `pathfinder.h` | A* pathfinding (used by Actor) |

## Build Commands

```bash
# Clean release build
make clean && make game

# Debug build with all NPCs
make clean && CXXFLAGS="-DDEBUG=1 -DDEBUG_ASSERT=1 -DNPCS_ALL_AT_ONCE -DMUSIC_OFF" make game

# Run
./game

# Unit tests (note: may need cxxtest include path on this machine)
make clean && CXXFLAGS="-DDEBUG=1 -DDEBUG_ASSERT=1 -DNPCS_ALL_AT_ONCE -DMUSIC_OFF -I/opt/homebrew/Cellar/cxxtest/4.4_3/include" make tests && ./tests
```

## Naming Conventions (from AGENTS.md)

- Files: `lowercase_snake_case.h` / `.cpp`
- Classes: `snake_case` for game types, `PascalCase` for ECS tags
- Functions: `snake_case`
- Namespaces: project uses `rpg::` for some subsystems
- Entity classes should live in `rpg::entities` namespace

## Notes

- The `with_fun` callback pattern (`std::function<void(gamestate&, entityid)>`) is used
  extensively for entity creation customization. Entity classes should support this pattern
  or replace it with a builder/options pattern.
- The tile cache system (`tile_t::cached_door`, `cached_chest`, `cached_box`, etc.) must
  be maintained. Entity classes write to tile cache during creation.
- The event queue (`gameplay_event_t`) dispatches interactions. Entity classes should
  integrate with this system rather than replacing it.
- `gamestate_world_interaction.cpp` is 1362 lines and contains ALL interaction logic.
  This is the biggest file to decompose — each entity class takes its slice.
- `gamestate.h` is ~1457 lines with 180+ methods. After entity class migration,
  many methods can be removed or delegated.
