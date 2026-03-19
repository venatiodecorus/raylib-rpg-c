/** @file libdraw_create_spritegroup.h
 *  @brief Helpers for creating renderer spritegroups from SpriteDef arrays.
 */

#pragma once

#include "gamestate.h"
#include "libdraw_context.h"
#include "sprite_def.h"

/**
 * @brief Create and register a spritegroup for an entity.
 *
 * The spritegroup is initialized from the provided SpriteDef array and snapped
 * to the entity location when a location component exists.
 */
bool create_spritegroup(gamestate& g, rpg::Renderer& renderer, entityid id, const rpg::SpriteDef* sprites, int num_sprites, int offset_x, int offset_y);

/** @brief Create a spritegroup using the project's default sprite offsets. */
bool create_sg(gamestate& g, rpg::Renderer& renderer, entityid id, const rpg::SpriteDef* sprites, int num_sprites);
