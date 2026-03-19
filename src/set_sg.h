/** @file set_sg.h
 *  @brief Helpers for switching spritegroups into state-driven animations.
 */

#pragma once

#include "gamestate.h"
#include "libdraw_context.h"
#include "spritegroup.h"

void libdraw_set_sg_block_success(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* const sg);
void libdraw_set_sg_is_damaged(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* const sg);
void libdraw_set_sg_is_dead(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* const sg);
void libdraw_set_sg_is_attacking(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* const sg);
