/** @file gameplay_scene.h
 *  @brief Gameplay scene.
 */

#pragma once

#include "scene.h"

class GameplayScene : public Scene {
public:
    void on_enter(gamestate& g) override;
    void on_exit(gamestate& g) override;
    void handle_input(gamestate& g, inputstate& is) override;
    void update(gamestate& g, inputstate& is) override;
};
