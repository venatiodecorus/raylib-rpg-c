# TODO — Refactor Roadmap

This document prioritizes the work needed to bring the codebase in line with
modern C++17 standards and best practices while retaining raylib as the
rendering/audio backend.

The codebase today is ~18,000 lines across 168 headers and 2 .cpp files. 87%
of the code lives in headers. There is a single god-object (`gamestate`, ~6,900
effective lines, 80+ public members, 180+ methods), pervasive global mutable
state, no namespaces, no smart pointer ownership, and full bidirectional
coupling between game logic and rendering.

---

## P0 — Critical / Blocking

These must be addressed first because everything else depends on them.

- [x] **Break up the `gamestate` god object**
  - The `gamestate` class (gamestate.h + 11 _impl.h files = ~6,900 lines) owns
    dungeon generation, entity creation, combat, pathfinding, inventory, input
    handling, audio playback, UI state, camera, debug, and message history.
  - Split into focused subsystems: `World`, `Combat`, `Inventory`, `InputHandler`,
    `AudioManager`, `UIState`, `Camera`, `MessageLog`, etc.
  - Each subsystem should be a separate .h/.cpp pair with a clear interface.
  - The _impl.h include-at-bottom pattern must be eliminated — move method
    bodies to .cpp files.

- [x] **Separate game logic from rendering**
  - Currently the renderer (`libdraw.cpp:532-561`) advances the game state
    machine (`g.flag`, `g.entity_turn`, `g.turn_count`), and game logic
    (`gamestate.h:379`) writes directly to the renderer context
    (`libdraw_ctx.audio.*`).
  - `PlaySound()` is called directly inside logic functions (combat, movement,
    door interaction) — audio should be event-driven instead.
  - Define a clear interface boundary: logic produces state, renderer reads it.
    No bidirectional mutation.

- [x] **Establish a proper compilation unit structure**
  - 168 headers vs 2 .cpp files means any header change rebuilds everything.
  - Move implementation out of headers into .cpp files.
  - Target: each subsystem has its own .cpp, headers contain only declarations.
  - This enables incremental builds and build parallelism.

## P1 — Architecture

- [x] **Introduce namespaces**
  - Zero named namespaces exist. All symbols are in the global namespace.
  - 55 `using std::X` declarations at file scope in headers pollute every
    includer's global namespace.
  - Add a project namespace (e.g. `rpg::`), move subsystems to sub-namespaces.
  - Remove all `using` declarations from headers; use fully-qualified names or
    confine `using` to .cpp files.

- [x] **Replace the component system with a proper ECS or clean it up**
  - The current "ECS" is a type-erased map-of-maps (`ComponentTable.h`). Every
    component read does 2 hash lookups + shared_ptr cast + value copy via
    `optional<T>`.
  - Entity iteration is a linear scan over all entity IDs
    (`for (id = 0; id < next_entityid; id++) { ct.get<entitytype>(id)... }`).
  - Entities are never destroyed — dead entities leave orphaned components in
    every store, leaking memory until full reset.
  - Options: adopt EnTT or similar, or redesign with sparse sets / archetype
    storage for cache-friendly iteration.

- [x] **Fix memory management — adopt RAII throughout**
  - `libdraw.cpp:73`: `new spritegroup(...)` stored as raw pointer in
    `unordered_map<entityid, spritegroup*>` — never deleted on restart (leak).
  - `spritegroup.h:55`: `sprites2 = new vector<...>()` — should be a direct
    member, not a heap-allocated pointer.
  - `libdraw_context.h:42`: raw pointer map should use `unique_ptr`.
  - Replace all raw `new`/`delete` with `unique_ptr` or direct member storage.

- [x] **Eliminate global mutable state**
  - Three process-lifetime globals: `inputstate is` (main.cpp:14),
    `gamestate g` (main.cpp:17), `libdraw_context_t libdraw_ctx`
    (libdraw.cpp:68).
  - Pass dependencies explicitly or use a service-locator / DI pattern.
  - The renderer context should be owned by a `Renderer` class, not a global.

- [x] **Break up `libdraw.cpp` (2,379 lines)**
  - This single file contains all rendering: sprite management, every UI screen,
    HUD, dungeon rendering, scene composition, resource loading, and init/shutdown.
  - Split into: `Renderer` (core pipeline), `UIRenderer` (menus, HUD, modals),
    `SpriteManager`, `ResourceLoader`, etc.

## P2 — Code Quality

- [x] **Replace `#define` constants with `constexpr` / `enum class`**
  - `libgame_defines.h`: ~60 `#define` numeric constants should be `constexpr`.
  - `sfx.h`: 62 SFX IDs + 100 legacy aliases — should be an `enum class`.
  - `gamestate.h`: `GAMESTATE_SIZEOFTIMEBUF`, `MAX_MESSAGES`, etc.
  - `texture_ids.h`: 501 texture IDs — should be an `enum class`.
  - Scene constants (`COMPANYSCENE`, `TITLESCENE`, `GAMEPLAYSCENE`) duplicate
    the `scene_t` enum — consolidate.

- [x] **Use `enum class` instead of C-style `typedef enum`**
  - All enums are unscoped: `scene_t`, `controlmode_t`, `race_t`, `alignment_t`,
    `entitytype_t`, `weapontype_t`, `tiletype_t`, `direction_t`, etc.
  - Convert to `enum class` for type safety.

- [x] **Add const-correctness throughout**
  - Many simple getters are not `const`: `dungeon_floor::get_upstairs_loc()`,
    `get_downstairs_loc()`, `get_biome()`, `get_height()`, `get_width()`, etc.
  - `dead_npc_cache::get_count()`, `contains()`, `top()` — all non-const.
  - `gamestate::get_avg_last_frame_time()` — non-const.
  - Audit all getters and mark const.

- [ ] **Fix type safety bugs**
  - `gamestate.h:267-269`: `targetwidth = -1; targetheight = -1;` assigned to
    `unsigned int` — implicit conversion to UINT_MAX.
  - `gamestate.h:333`: `fadealpha = 0.0` assigned to `unsigned int`.

- [ ] **Eliminate C-style casts**
  - ~40 C-style casts found across `libdraw.cpp`, `dungeon_floor.h`,
    `gamestate_scene_impl.h`, `entity_templates.h`.
  - Replace with `static_cast<>`.

- [ ] **Replace C headers with C++ equivalents**
  - `massert.h` uses `<stdio.h>` / `<stdlib.h>` — use `<cstdio>` / `<cstdlib>`.

- [ ] **Fix mprint.h logging level hierarchy**
  - `DEBUG=1` enables level-1 only. `DEBUG=2` enables level-2 only.
    `DEBUG=3` enables level-3 only. Higher levels should include lower levels.

## P3 — Performance

- [ ] **Optimize entity iteration**
  - `gamestate_lifecycle_impl.h:118-124` and `gamestate_npc_combat_impl.h:549-578`
    do `for (id = 0; id < next_entityid; id++)` scanning ALL entities to find
    NPCs. Maintain per-type entity lists or use archetype iteration.

- [ ] **Optimize component access**
  - `ComponentTable::get<>()` returns `optional<T>` by value — causes copies
    for `string`, `shared_ptr<vector<entityid>>`.
  - `ComponentTable::has()` calls `getStore<Kind>()` twice (redundant hash).
  - Consider returning `const T*` or `T*` instead of `optional<T>`.

- [ ] **Optimize A* pathfinding**
  - `gamestate_npc_combat_impl.h:357-457`: allocates three `std::map` trees +
    a `priority_queue` per NPC per turn. Uses ordered `map` (O(log n)) instead
    of `unordered_map` (O(1)). Maintain a reusable pathfinder object.

- [ ] **Optimize tile visibility updates**
  - `gamestate_lifecycle_impl.h:56-95`: clears visibility for every tile on
    the floor, then updates the light radius. Only clear/update affected tiles.

- [ ] **Optimize `get_random_loc()`**
  - `dungeon_floor.h:650-674`: builds a full `vector<vec3>` of all empty tiles
    every call. Cache valid positions or use rejection sampling.

## P4 — Assets and Runtime

- [ ] Map placeholder textures to actual tilemap sub-regions and sprite files
- [ ] Fix font/shader loading for full runtime functionality
- [ ] Source additional SFX for aliased sounds (alchemy, bag, equip, material)
- [ ] Source additional music tracks
- [ ] Add setup instructions for acquiring Minifantasy asset packs (gfx/, sfx/)
- [ ] Add asset pack download/setup script for developer onboarding

## P5 — Build and Tooling

- [ ] Test Linux build with platform detection in Makefile
- [ ] Update web build configuration for new asset paths
- [ ] Set up CI (build + test on Linux and macOS)
- [ ] Add clang-format configuration for consistent style
- [ ] Add clang-tidy configuration for automated quality checks
