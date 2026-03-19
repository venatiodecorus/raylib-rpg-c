/** @file scene.h
 *  @brief High-level scene identifiers for top-level game flow.
 */

#pragma once

/// @brief Primary scenes that the game loop can present.
enum class scene_t {
    COMPANY = 0,
    TITLE,
    MAIN_MENU,
    CHARACTER_CREATION,
    GAMEPLAY,
};
