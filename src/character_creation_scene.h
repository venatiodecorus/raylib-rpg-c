/** @file character_creation_scene.h
 *  @brief Character creation scene.
 */

#pragma once

#include "scene.h"

class CharacterCreationScene : public Scene {
public:
    void handle_input(gamestate& g, inputstate& is) override;
};
