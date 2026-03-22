/** @file factory.cpp
 *  @brief Static factory mapping entitytype_t to entity handler singletons.
 */

#include "actor.h"
#include "box.h"
#include "chest.h"
#include "door.h"
#include "item.h"
#include "prop.h"

namespace rpg::entities
{

namespace
{

class NoopEntity : public EntityBase {
public:
    void create_sprite(gamestate&, rpg::Renderer&, entityid) override {
    }
    void draw(gamestate&, rpg::Renderer&, entityid, spritegroup*) override {
    }
};

} // namespace

static Actor actor_handler;
static Box box_handler;
static Chest chest_handler;
static Door door_handler;
static Item item_handler;
static Prop prop_handler;
static NoopEntity noop_handler;

EntityBase& EntityBase::for_type(entitytype_t type) {
    switch (type) {
    case entitytype_t::PLAYER:
    case entitytype_t::NPC: return actor_handler;
    case entitytype_t::DOOR: return door_handler;
    case entitytype_t::BOX: return box_handler;
    case entitytype_t::CHEST: return chest_handler;
    case entitytype_t::ITEM: return item_handler;
    case entitytype_t::PROP: return prop_handler;
    default: return noop_handler;
    }
}

} // namespace rpg::entities
