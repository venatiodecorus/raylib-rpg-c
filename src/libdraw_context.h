/** @file libdraw_context.h
 *  @brief Shared renderer-global assets and frame targets used by the libdraw subsystem.
 */

#pragma once

#include "entityid.h"
#include "libgame_defines.h"
#include "sprite_def.h"
#include "spritegroup.h"
#include "textureinfo.h"
#include <memory>
#include <raylib.h>
#include <string>
#include <unordered_map>

namespace rpg
{

/**
 * @brief Process-lifetime renderer state shared across the libdraw subsystem.
 *
 * This groups the renderer's mutable state into one object so libdraw
 * callers depend on a single shared context instead of many unrelated globals.
 */
class Renderer {
public:
    /// @brief Active spritegroup instances keyed by owning entity id.
    std::unordered_map<entityid, std::unique_ptr<spritegroup>> spritegroups;
    /// @brief Texture metadata table indexed by generated texture id (legacy).
    textureinfo txinfo[GAMESTATE_SIZEOFTEXINFOARRAY];
    /// @brief On-demand texture cache keyed by cache key string.
    std::unordered_map<std::string, Texture2D> texture_cache;
    /// @brief Loaded shaders keyed by project-specific shader identifiers.
    std::unordered_map<int, Shader> shaders;
    /// @brief Off-screen target used to compose the title screen.
    RenderTexture2D title_target_texture = {};
    /// @brief Off-screen target used to compose the character-creation scene.
    RenderTexture2D char_creation_target_texture = {};
    /// @brief Off-screen target used to compose the gameplay world scene.
    RenderTexture2D main_game_target_texture = {};
    /// @brief Off-screen target used to compose the HUD overlay.
    RenderTexture2D hud_target_texture = {};
    /// @brief Final compositing target presented to the window.
    RenderTexture2D target = {};
    /// @brief Source rectangle used when blitting the final render target.
    Rectangle target_src = {0, 0, DEFAULT_TARGET_WIDTH, DEFAULT_TARGET_HEIGHT};
    /// @brief Destination rectangle used when scaling the final render target.
    Rectangle target_dest = {0, 0, DEFAULT_TARGET_WIDTH, DEFAULT_TARGET_HEIGHT};
    /// @brief Window-space destination rectangle for direct presentation.
    Rectangle win_dest = {0, 0, DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT};
    /// @brief Global animation speed divisor used by sprite update code.
    int anim_speed = DEFAULT_ANIM_SPEED;
    /// @brief Accumulated time since last animation frame advance.
    float anim_elapsed = 0.0f;

    /** @brief Load (or retrieve from cache) a Texture2D for a SpriteDef. */
    Texture2D load_sprite_texture(const rpg::SpriteDef& def);

    /** @brief Unload all textures held in texture_cache. */
    void unload_texture_cache();
};

} // namespace rpg
