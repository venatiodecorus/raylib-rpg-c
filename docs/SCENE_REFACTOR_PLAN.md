# Scene Refactor Plan

## Problem

The game has 5 scenes (`COMPANY`, `TITLE`, `MAIN_MENU`, `CHARACTER_CREATION`, `GAMEPLAY`)
represented by a simple enum `scene_t`. Scene dispatch is scattered across 4 separate
switch/if-chain sites:

- `gamestate_input.cpp:749-771` — `handle_input()` if-chain
- `libdraw.cpp:160-175` — `libdraw_render_current_scene_to_scene_texture()` switch
- `libdraw.cpp:177-185` — `libdraw_draw_current_scene_from_scene_texture()` switch
- `libdraw_update_sprites.cpp:142-153` — `libdraw_update_sprites_pre()` guard

The `gamestate` class (1455 lines) is a god object mixing data, input, game logic,
entity factory, and rendering preparation. Scene-specific methods live directly on it.

Additionally, entities are created in `logic_init()` before `InitWindow()` creates
the OpenGL context, so sprite creation must be deferred via dirty flags. A scene
system where scenes own entity lifecycle can solve this: entities are created in
`on_enter()` when the renderer is already ready.

`COMPANY` enum value is dead code (never referenced).

## Recommended Approach

Virtual `Scene` base class with concrete implementations. Scenes own entity lifecycle.
3-phase refactor to keep each phase independently testable.

## Phase 1: Scene Interface + Simple Scenes

Introduce the scene system alongside existing code. No behavior changes.

### New files

- `src/scene.h` — Replace enum with `Scene` base class
- `src/title_scene.h/cpp` — Title screen scene
- `src/main_menu_scene.h/cpp` — Main menu scene
- `src/character_creation_scene.h/cpp` — Character creation scene
- `src/gameplay_scene.h/cpp` — Gameplay scene (initially delegates to existing code)

### Base class

```cpp
class Scene {
public:
    virtual ~Scene() = default;
    virtual void on_enter(gamestate& g) {}
    virtual void on_exit(gamestate& g) {}
    virtual void handle_input(gamestate& g, inputstate& is) = 0;
    virtual void update(gamestate& g, inputstate& is) {}
    virtual void render_to_texture(gamestate& g, rpg::Renderer& renderer) = 0;
    virtual void render_from_texture(gamestate& g, rpg::Renderer& renderer) = 0;
    virtual void update_sprites_pre(gamestate& g, rpg::Renderer& renderer) {}
    virtual void update_sprites_post(gamestate& g, rpg::Renderer& renderer) {}
};
```

### Changes

- `gamestate.h` — Add `std::unique_ptr<Scene> scene;` and `void set_scene(Scene*)`.
  Remove migrated scene-specific methods.
- `main.cpp` — `gameloop()` delegates to `g.scene->handle_input()` / `g.scene->update()`.
- `libdraw.cpp` — `drawframe()` delegates to `g.scene->render_to_texture()` / etc.
- `gamestate_scene.cpp` — Remove migrated methods. Character creation transition
  calls `set_scene(gameplay_scene)`.
- `gamestate_input.cpp` — `handle_input()` delegates to `g.scene->handle_input()`.
- `libdraw_update_sprites.cpp` — Delegate to `g.scene->update_sprites_pre/post()`.
- `Makefile` — Add new source files.

## Phase 2: Gameplay Scene (Solves Sprite Deferral)

Move gameplay entity lifecycle into `GameplayScene`. Entities created in `on_enter()`
after OpenGL exists — eliminates bulk deferred sprite creation.

### GameplayScene responsibilities

- `on_enter()` — Calls `g.logic_init()`, then synchronously creates sprites for all
  entities (renderer is ready). No dirty flag needed for initial population.
- `on_exit()` — Calls `g.logic_close()`, clears sprite groups.
- `handle_input()` — Moves `handle_input_gameplay_scene()` and all control mode
  dispatch from `gamestate_input.cpp`.
- `update()` — Moves `tick()` core logic: FOV, player state, NPC state, NPC turns,
  damage popups.
- `render_to_texture()` — World renderer, HUD, menus, inventory.
- `update_sprites_pre/post()` — Per-entity sprite updates and animation.

### Key change

`gamestate_scene.cpp:116-118` bulk dirty reset eliminated. Character creation
transition becomes `set_scene(gameplay_scene)` which triggers `on_enter()`.

The `dirty_entities` mechanism stays for mid-gameplay entity spawning.

### Files modified

- `gamestate_lifecycle.cpp` — `tick()` content moves to `GameplayScene::update()`.
- `gamestate_input.cpp` — Gameplay handlers move to `GameplayScene`.
- `gamestate_scene.cpp` — Character creation transition simplified.
- `gameplay_scene.h/cpp` — Full implementation.

## Phase 3: Cleanup

- Remove `scene_t` enum from `scene.h` entirely
- Remove `current_scene` field from `gamestate`
- Remove old scene-specific method declarations from `gamestate.h`
- Delete `gamestate_scene.cpp`
- Remove dead `COMPANY` enum value
- Move `libdraw_update_sprites_pre/post` guards into `GameplayScene`

## Testing

- All existing tests should pass after each phase (no logic changes, just relocation)
- Smoke test: `make clean && CXXFLAGS="-DDEBUG=1 -DNPCS_ALL_AT_ONCE -DDEBUG_ASSERT=1" make game && ./game`
- Unit tests: `make tests && ./tests`
- Verify transitions: TITLE -> MAIN_MENU -> CHARACTER_CREATION -> GAMEPLAY -> restart
