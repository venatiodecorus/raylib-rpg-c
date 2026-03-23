/** @file scene.h
 *  @brief Scene base class and high-level scene identifiers for top-level game flow.
 */

#pragma once

#include "inputstate.h"

class gamestate;

/// @brief Primary scenes that the game loop can present.
enum class scene_t {
    COMPANY = 0,
    TITLE,
    MAIN_MENU,
    CHARACTER_CREATION,
    GAMEPLAY,
};

/// @brief Base class for game scenes. Each scene owns its input and update logic.
class Scene {
public:
    virtual ~Scene() = default;

    /** @brief Called once when this scene becomes active. */
    virtual void on_enter(gamestate& g) {
    }

    /** @brief Called once when this scene is being replaced by another. */
    virtual void on_exit(gamestate& g) {
    }

    /** @brief Handle per-frame input for this scene. */
    virtual void handle_input(gamestate& g, inputstate& is) = 0;

    /** @brief Advance per-frame game logic for this scene. */
    virtual void update(gamestate& g, inputstate& is) {
    }
};
