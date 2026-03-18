# TODO


- [ ] BUG sometimes it appears like the orcs are spawning without gear
- [ ] BUG game crashes on reload 
- [ ] BUG when going downstairs/upstairs, it is possible to be "on top" of another entity
    - [ ] when you walk off of the tile, the entity acts
    - [ ] unobserved what happens if you do not walk off
    - [ ] FIX NPCs should not be able to walk onto a stairs tile in general probably
- [ ] BUG NPCs appear to be able to walk through doors
- [ ] BUG shield direction context appears to be incorrect for the up-right direction
    - [ ] observed on a few races but not all
- [ ] wolf shield animation bugfix
- [ ] slime shield animation bugfix
- [ ] bat walk animation bugfix
- [ ] bat shield animation bugfix (CRASHING)
- [ ] warg shield animation bugfix (CRASHING)
- [ ] fix web shaders

- [ ] magic system
- [ ] more NPC races
- [ ] more accounting for stats etc in rolls
- [ ] armor
    - [ ] head
    - [ ] chest 
    - [ ] hands 
    - [ ] feet
    - [ ] rings
    - [ ] amulets
- [ ] critical hits
- [ ] humidity
- [ ] air quality
- [ ] poison
- [ ] trap tiles
- [ ] switches
- [ ] circuits
- [ ] stealing
- [ ] invisibility
- [ ] potions
- [ ] sneaking
- [ ] intelligent NPC action selection
- [ ] torches
- [ ] sokoban mechanics (re-introduction)
- [ ] item creation
- [ ] spell creation

- [ ] Automatic generation of texture_ids by filename similar to how sound effects works

## Placeholder Assets (magenta squares — need real tilemap/sprite mappings)

### Tiles (163 textures)
- [ ] Map all TX_TILES_* entries to dungeon/plains tilemap sub-regions
- [ ] TX_TILES_STONE_* (13 variants) — dungeon stone floor tiles
- [ ] TX_TILES_GRASS_* (20 variants) — outdoor grass tiles
- [ ] TX_TILES_DIRT_* (9 variants) — dirt tiles
- [ ] TX_TILES_STONE_DIRT_* (54 variants) — stone-to-dirt transition tiles
- [ ] TX_TILES_STONE_GRASS_* (14 variants) — stone-to-grass transition tiles
- [ ] TX_TILES_DIRT_GRASS_* (15 variants) — dirt-to-grass transition tiles
- [ ] TX_TILES_WATER_* (2 variants, animated) — water tiles
- [ ] TX_TILES_WALL_STONE_* (2 variants) — wall tiles
- [ ] TX_TILES_UPSTAIRS_00, TX_TILES_DOWNSTAIRS_00 — staircase tiles
- [ ] TX_TILES_STONE_TRAP_ON_00, TX_TILES_STONE_TRAP_OFF_00 — trap tiles
- [ ] TX_TILES_TOWN_* (all town tile variants)

### Doors (4 textures)
- [ ] TX_DOOR_WOODEN_OPEN_00/01, TX_DOOR_WOODEN_CLOSED_00/01

### Props (17 textures)
- [ ] TX_PROP_* — tables, signs, chairs, banners, barrels, torch, statue, candle, jar, plate, boxes

### Items & Equipment
- [ ] TX_ITEM_* (11 textures) — arrow, torch, key, sack, potions
- [ ] TX_WEAPON_* (10 textures) — short sword, bow, long sword, etc.
- [ ] TX_SHIELD_* (3 textures) — buckler, kite shield, tower shield
- [ ] TX_CHEST_* (2 textures) — open/closed chest
- [ ] TX_BOX_WOODEN (1 texture)

### GUI Elements
- [ ] TX_GUI_* (6 textures) — heart icon, selectors, inventory slot

### Switches & Interactive
- [ ] TX_SWITCHES_* (11 textures) — wall switches, pressure plates, levers
- [ ] TX_GEMS_* (4 textures, animated)
- [ ] TX_TILE_ETCHINGS_* (1 texture)
- [ ] TX_RING_* (3 textures, animated)

### Character Animations (missing from asset packs)
- [ ] TX_CHAR_*_GUARD / TX_CHAR_*_GUARD_SHADOW — guard stance for all races
- [ ] TX_CHAR_*_GUARD_SUCCESS / TX_CHAR_*_GUARD_SUCCESS_SHADOW — guard success for all races
- [ ] TX_CHAR_*_SHOT / TX_CHAR_*_SHOT_SHADOW — ranged shot for all races
- [ ] TX_CHAR_*_MINING / TX_CHAR_MINING_SHADOW — mining animation for all races
- [ ] TX_CHAR_*_ATTACK_THRUST / TX_CHAR_*_ATTACK_THRUST_SHADOW — thrust attack for all races

### Monsters
- [ ] TX_MONSTER_RAT_* (10 textures) — no rat sprites in current asset packs

### Action Overlays
- [ ] TX_ACTIONS_* (17 textures) — weapon slash/shot/guard overlays

### Miscellaneous
- [ ] TX_AMULETS_* (1 texture)
- [ ] TX_ARMOR_FEET_* (1 texture), TX_ARMOR_BODY_* (2 textures)
- [ ] TX_FOOD_* (3 textures)
- [ ] TX_RUNES_* (8 textures)

--------------------------------------------------------------------------------

- [x] create infrastructure for A/B testing of a specific game logic
    - [x] individual NPC turn rendering (individual NPC turns render one-at-a-time)
    - [x] mass NPC turn rendering (all NPCs' turns render simultaneously)

- [x] enemies are currently attacking each other randomly (shouldn't happen)
  - [x] was adjacency check 

- [x] elf shield animation bugfix
- [x] dwarf shield animation bugfix
- [x] orc shield animation bugfix
- [x] goblin shield animation bugfix
- [x] halfling shield animation bugfix

- [x] drawing order needs changing so that NPCs (warg etc) do not render behind door sprites when adjacent
- [x] drawing order needs changing so that player target box renders beneath if player is a warg etc

- [x] when handling light radius and visibility distance, we need to check line-of-sight so we don't render
things that might be behind doors, walls, etc.

- [x] Shader so that the target box glows
- [x] BUG death animation for player stuck on first frame

