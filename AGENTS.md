# AGENTS.md

Instructions for agentic coding agents operating in this repository.

## Project Overview

C++17 RPG game built with [raylib](https://www.raylib.com/) and Minifantasy asset packs.
Source is in `src/`, headers in `include/` (vendored EnTT). The project was ported from C
and retains some C-style patterns alongside C++ idioms.

## Build Commands

All commands run from the repository root (the Makefile is there, not in `src/`).

```bash
# Clean build artifacts
make clean

# Release build
make game

# Debug build (enables logging, assertions, NPC spawning)
make clean && CXXFLAGS="-DDEBUG_ASSERT=1 -DNPCS_ALL_AT_ONCE -DDEBUG=1 -DMASTER_VOLUME=0.25f" make game

# WebAssembly build (requires emcc)
make clean && CXXFLAGS="-DDEBUG_ASSERT=1 -DNPCS_ALL_AT_ONCE -DDEBUG=1 -DMASTER_VOLUME=0.25f" make index.html

# Run the game
./game
```

### Common Compiler Flags

| Flag               | Purpose                                      |
|--------------------|----------------------------------------------|
| `-DDEBUG=1`        | Enable debug logging macros (`minfo`, etc.)  |
| `-DDEBUG_ASSERT=1` | Enable debug assertions                      |
| `-DNPCS_ALL_AT_ONCE` | Spawn all NPCs at startup                  |
| `-DMUSIC_OFF`      | Disable music (useful for tests)             |
| `-DMASTER_VOLUME=` | Set master volume (float, e.g. `0.25f`)     |

## Testing

Framework: **CxxTest**. Test suites live in `src/test_suites/test_*.h`.
The aggregate include is `src/unit_test.h`. Heavy/slow tests are in
`src/unit_test_heavy.h` (includes `test_heavy_simulation.h`).

```bash
# Build and run standard unit tests
make tests && ./tests

# Build and run heavy tests (includes simulation suite)
make tests_heavy && ./tests_heavy

# Convenience script (clean + debug build + run tests)
bash src/do_unittests.sh
```

There is **no** way to run a single test suite in isolation — all test suites are
compiled into one binary. To focus on specific tests, temporarily comment out
includes in `src/unit_test.h` or `src/unit_test_heavy.h`, then rebuild.

### Writing Tests

- Create a new file `src/test_suites/test_<feature>.h`.
- Class name: `<Feature>TestSuite` inheriting `CxxTest::TestSuite`.
- Method prefix: `test` (e.g. `testAddId`).
- Use `TS_ASSERT(expr)`, `TS_ASSERT_EQUALS(a, b)`, `TS_ASSERT_DELTA(a, b, d)`.
- Add the `#include` to `src/unit_test.h` (and `src/unit_test_heavy.h` if applicable).
- Add the test header path to the `cxxtestgen` invocation in the Makefile.

## Code Style

Formatting is enforced by **clang-format** using `src/.clang-format` (WebKit-based).

```bash
# Format a file
clang-format -i src/some_file.cpp

# Check formatting without changes
clang-format --dry-run -Werror src/some_file.cpp
```

Key formatting rules:
- 4-space indentation, no tabs
- 160 column limit
- Braces: attach to control statements and functions (K&R style), but `else` on new line
- Pointer alignment: left (`int* p`, not `int *p`)
- No parameter bin-packing — one parameter per line when breaking
- Sort includes alphabetically

### Naming Conventions

- **Files**: `lowercase_snake_case.h` / `.cpp` (matching the C heritage)
- **Classes**: `snake_case` for game types (`dead_npc_cache`, `gamestate`, `ComponentTable`)
- **Structs for ECS tags**: `PascalCase` (`LegacyEntityId`, `NpcTag`)
- **Functions**: `snake_case` (`get_entity_name`, `vec3_add`, `drawframe`)
- **Constants/macros**: `UPPER_SNAKE_CASE` (`DEAD_NPC_CACHE_SIZE`, `INVALID`)
- **Member variables**: plain `snake_case` (no prefix)
- **Typedefs**: `snake_case` with `_t` suffix when used as type aliases (`entitytype_t`)
- **Template parameters**: `PascalCase` (`Kind` in `ComponentTable`)

### Includes

- Use `#pragma once` for all headers (no traditional include guards)
- Local project includes use quotes: `#include "gamestate.h"`
- System/third-party includes use angle brackets: `#include <raylib.h>`
- Prefer relative paths from `src/` for project headers
- In test files, use `../` to reference src headers: `#include "../ComponentTable.h"`

### Documentation

- Use Doxygen-style `/** @file ... */` and `/** @brief ... */` comments on public APIs
- Keep comments focused on "what" and "why", not "how"
- Use `/// @brief` for single-line member docs

### Error Handling & Debug Logging

- Use the logging macros from `mprint.h` (compile-time gated by `DEBUG` level):
  - `minfo(...)` / `mwarning(...)` / `merror(...)` / `msuccess(...)` — level 1
  - Append `2` or `3` for deeper verbosity (`minfo2`, `minfo3`)
- Macros are no-ops when `DEBUG` is not defined
- Use `massert.h` for debug assertions (gated by `DEBUG_ASSERT`)

### Architecture Notes

- Entity-Component system uses EnTT (`entt::registry`); `entityid` is an alias for `entt::entity`
- ECS component tags are defined in `ecs_*_components.h` files
- Game state is centralized in the `gamestate` struct (`src/gamestate.h`, ~1400 lines)
- Rendering is split across many `draw_*.cpp` files; the main render entry is `drawframe()` in `libdraw.cpp`
- Audio is managed by the `audio_manager` class
- Pathfinding uses a custom implementation in `pathfinder.h`

## Directory Layout

```
src/                  All source (.cpp) and header (.h) files (flat structure)
src/test_suites/      CxxTest test suite headers
src/shaders/          GLSL fragment shaders
src/docs/             Generated Doxygen output
include/entt/         Vendored EnTT ECS library
gfx/                  Game graphics assets (not distributed)
sfx/                  Sound effect assets (not distributed)
```
