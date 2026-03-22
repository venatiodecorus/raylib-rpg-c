/** @file item.h
 *  @brief Item entity class — weapons, shields, potions with sprite management.
 */

#pragma once

#include "entity_base.h"
#include "../weapon.h"
#include "../shield.h"
#include "../potion.h"

namespace rpg::entities {

/** @brief Item entity with weapon/shield/potion subtypes. */
class Item : public EntityBase {
public:
    /** @brief Create a weapon item entity. */
    static entityid create_weapon(gamestate& g);

    /** @brief Create a shield item entity. */
    static entityid create_shield(gamestate& g);

    /** @brief Create a potion item entity. */
    static entityid create_potion(gamestate& g);

    /** @brief Create and place a weapon at a dungeon location. */
    static entityid create_weapon_at(gamestate& g, vec3 loc);

    /** @brief Create and place a shield at a dungeon location. */
    static entityid create_shield_at(gamestate& g, vec3 loc);

    /** @brief Create and place a potion at a dungeon location. */
    static entityid create_potion_at(gamestate& g, vec3 loc);

    void create_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) override;
    void update_sprite(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) override;
    void draw(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) override;
};

} // namespace rpg::entities
