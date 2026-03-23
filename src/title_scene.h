/** @file title_scene.h
 *  @brief Title screen scene.
 */

#pragma once

#include "scene.h"

class TitleScene : public Scene {
public:
    void handle_input(gamestate& g, inputstate& is) override;
};
