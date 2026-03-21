/** @file ecs_gameplay_components.h
 *  @brief EnTT component structs for all gameplay state migrated from ComponentTraits.
 */

#pragma once

#include "alignment.h"
#include "direction.h"
#include "entity_actions.h"
#include "entityid.h"
#include "entitytype.h"
#include "item.h"
#include "potion.h"
#include "proptype.h"
#include "race.h"
#include "shield.h"
#include "vec2.h"
#include "vec3.h"
#include "weapon.h"
#include <raylib.h>
#include <string>
#include <vector>

/// @brief Entity display name.
struct EntityName {
    std::string value;
};

/// @brief Entity description text.
struct EntityDescription {
    std::string value;
};

/// @brief NPC dialogue text.
struct DialogueLine {
    std::string value;
};

/// @brief Dungeon-space entity position (x, y = tile coords, z = floor).
struct Position {
    vec3 value;
};

/// @brief Hit points (x = current, y = max).
struct HitPoints {
    vec2 value;
};

/// @brief Whether the entity is dead.
struct DeadFlag {
    bool value;
};

/// @brief Actor facing direction.
struct Facing {
    direction_t value;
};

/// @brief Sprite movement/interpolation state.
struct SpriteMoveState {
    Rectangle value;
};

/// @brief Per-frame update eligibility flag.
struct NeedsUpdate {
    bool value;
};

/// @brief Attack animation/action state.
struct AttackingFlag {
    bool value;
};

/// @brief Recent damage feedback state.
struct DamagedFlag {
    bool value;
};

/// @brief Successful block resolution flag.
struct BlockSuccessFlag {
    bool value;
};

/// @brief Whether the entity can be pushed (reuse PushableTag from ecs_world_object_components.h).
// PushableFlag — use PushableTag instead

/// @brief Whether the entity can be pulled (reuse PullableTag from ecs_world_object_components.h).
// PullableFlag — use PullableTag instead

/// @brief Actor level.
struct EntityLevel {
    int value;
};

/// @brief Accumulated experience points.
struct Experience {
    int value;
};

/// @brief Strength attribute.
struct StrengthAttr {
    int value;
};

/// @brief Dexterity attribute.
struct DexterityAttr {
    int value;
};

/// @brief Intelligence attribute.
struct IntelligenceAttr {
    int value;
};

/// @brief Wisdom attribute.
struct WisdomAttr {
    int value;
};

/// @brief Constitution attribute.
struct ConstitutionAttr {
    int value;
};

/// @brief Charisma attribute.
struct CharismaAttr {
    int value;
};

/// @brief Current AI target entity id.
struct TargetEntity {
    entityid value;
};

/// @brief Emitted light radius in tiles.
struct LightRadius {
    int value;
};

/// @brief Vision range in tiles.
struct VisionRange {
    int value;
};

/// @brief Hearing range in tiles.
struct HearingRange {
    int value;
};

/// @brief Whether the NPC is in an aggressive state.
struct AggroFlag {
    bool value;
};

/// @brief Damage dice payload (x=count, y=sides, z=bonus).
struct DamageDice {
    vec3 value;
};

/// @brief Healing dice payload (x=count, y=sides, z=bonus).
struct HealingDice {
    vec3 value;
};

/// @brief Current item durability.
struct ItemDurability {
    int value;
};

/// @brief Maximum item durability.
struct ItemMaxDurability {
    int value;
};

/// @brief Whether an item has been destroyed/broken.
struct DestroyedFlag {
    bool value;
};

/// @brief Door/chest open/closed state.
struct DoorOpenFlag {
    bool value;
};

/// @brief Block chance percentage or score.
struct BlockChance {
    int value;
};

/// @brief Base armor class.
struct BaseArmorClass {
    int value;
};

/// @brief Hit-die style values (x=count, y=sides, z=modifier).
struct HitDie {
    vec3 value;
};

/// @brief Prop type selection.
struct PropTypeComponent {
    proptype_t value;
};

/// @brief Whether the entity blocks movement (reuse SolidTag from ecs_world_object_components.h).
// SolidFlag — use SolidTag instead

/// @brief Counted movement steps.
struct StepsTaken {
    unsigned int value;
};

/// @brief Default NPC AI action.
struct DefaultAction {
    entity_default_action_t value;
};

/// @brief NPC path toward current target.
struct NpcPath {
    std::vector<vec3> value;
};

/// @brief Entity alignment.
struct AlignmentComponent {
    alignment_t value;
};

/// @brief Item sub-category.
struct ItemSubtype {
    itemtype_t value;
};

/// @brief Weapon sub-category.
struct WeaponSubtype {
    weapontype_t value;
};

/// @brief Shield sub-category.
struct ShieldSubtype {
    shieldtype_t value;
};

/// @brief Potion sub-category.
struct PotionSubtype {
    potiontype_t value;
};

/// @brief Entity inventory (owned item ids).
struct Inventory {
    std::vector<entityid> value;
};

/// @brief Equipped weapon entity id.
struct EquippedWeapon {
    entityid value;
};

/// @brief Equipped shield entity id.
struct EquippedShield {
    entityid value;
};
