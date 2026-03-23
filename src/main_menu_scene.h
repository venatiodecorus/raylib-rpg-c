/** @file main_menu_scene.h
 *  @brief Main menu scene.
 */

#pragma once

#include "scene.h"

class MainMenuScene : public Scene {
public:
    void handle_input(gamestate& g, inputstate& is) override;
};
