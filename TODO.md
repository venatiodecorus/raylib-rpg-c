# TODO

## High Priority
- [ ] Map placeholder textures to actual tilemap sub-regions and sprite files
- [ ] Fix runtime issues preventing normal gameplay
- [ ] Add setup instructions for acquiring Minifantasy asset packs (gfx/, sfx/)

## Texture / Asset Mapping
- [ ] Map dungeon tile textures (TX_TILES_STONE_*, TX_TILES_WALL_*, etc.) to Tileset.png sub-regions
- [ ] Map outdoor tile textures (TX_TILES_GRASS_*, TX_TILES_DIRT_*) to ForgottenPlainsTiles.png
- [ ] Map prop textures (TX_PROP_*) to Props.png sub-regions
- [ ] Map door textures (TX_DOOR_*) to dungeon props
- [ ] Map item/weapon/shield/chest textures to appropriate tilemap regions
- [ ] Map GUI textures (TX_GUI_*) to asset pack UI elements
- [ ] Create or source rat sprites (TX_MONSTER_RAT_*)
- [ ] Map action overlay textures (TX_ACTIONS_*)
- [ ] Map character guard/thrust/shot/mining animations (currently placeholders)

## Audio
- [ ] Source additional SFX for alchemy, bag, equip, material sounds (currently aliased)
- [ ] Source additional music tracks

## Gameplay
- [ ] Fix font/shader loading for full runtime functionality
- [ ] Verify all character race animations display correctly
- [ ] Verify all monster animations display correctly
- [ ] Test combat system with new SFX mappings

## Build / Infrastructure
- [ ] Test Linux build with platform detection in Makefile
- [ ] Update web build configuration for new asset paths
- [ ] Add asset pack download/setup script for developer onboarding
