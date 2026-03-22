/** @file libgame_defines.h
 *  @brief Shared compile-time constants for versioning, windowing, UI, and gameplay defaults.
 */

#pragma once

// Game info
constexpr const char* GAME_VERSION = "v0.0.6.26";
constexpr const char* GAME_VERSION_DATE = "March 17, 2026";
constexpr const char* WINDOW_TITLE = "@evildojo666 presents: project.rpg";

// Window
constexpr int DEFAULT_WIN_WIDTH = 1920;
constexpr int DEFAULT_WIN_HEIGHT = 1080;
constexpr int DEFAULT_TARGET_WIDTH = 1920;
constexpr int DEFAULT_TARGET_HEIGHT = 1080;

// Alternative window sizes if needed (commented out)
// constexpr int DEFAULT_WIN_WIDTH      = 360;
// constexpr int DEFAULT_WIN_HEIGHT     = 640;
// constexpr int DEFAULT_TARGET_WIDTH   = 360;
// constexpr int DEFAULT_TARGET_HEIGHT  = 640;

// Audio
constexpr float DEFAULT_MASTER_VOLUME = 1.0f;
constexpr float DEFAULT_MUSIC_VOLUME = 0.5f;

// UI
constexpr int DEFAULT_MSG_HISTORY_FONT_SIZE = 20;
constexpr int DEFAULT_MSG_WINDOW_FONT_SIZE = 20;
constexpr int DEFAULT_LOOK_PANEL_FONT_SIZE = 20;
constexpr int DEFAULT_HUD_FONT_SIZE = 20;
constexpr int DEFAULT_HUD_BUFFER_LINE_SIZE = 1024;
constexpr int DEFAULT_HUD_LINE_COUNT = 4;

// Graphics
constexpr int DEFAULT_TILE_SIZE_SCALED = 32;
constexpr float DEFAULT_TILE_SIZE_FLOAT = 32.0f;
constexpr int DEFAULT_OFFSET = -12;
constexpr int DEFAULT_TARGET_FPS = 60;
constexpr int DEFAULT_LOCK = 30;
constexpr int DEFAULT_TILE_SIZE = 8;
constexpr float DEFAULT_ZOOM_INCR = 1.0f;
constexpr float DEFAULT_ZOOM_LEVEL = 4.0f;
constexpr int FRAMEINTERVAL = 10;

// Gameplay
constexpr int DEFAULT_DUNGEONFLOOR_WIDTH = 8;
constexpr int DEFAULT_DUNGEONFLOOR_HEIGHT = 8;
constexpr int DEFAULT_ANIM_SPEED = 10;
constexpr float ANIM_FPS = 8.0f;
constexpr float ANIM_FRAME_TIME = 1.0f / ANIM_FPS;
constexpr float DEFAULT_CAMERA_ZOOM = 10.0f;

// Memory
constexpr int DEFAULT_SPRITEGROUPS_SIZE = 128;
constexpr int SPRITEGROUP_DEFAULT_SIZE = 32;
constexpr int GAMESTATE_SIZEOFTEXINFOARRAY = 2048;

// UI padding
constexpr int DEFAULT_PAD = 20;
