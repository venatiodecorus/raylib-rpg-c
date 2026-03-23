/** @file entityid.h
 *  @brief Entity id alias backed by entt::entity.
 */

#pragma once

#include <entt/entt.hpp>

/// @brief Entity identifier type — alias for entt::entity.
using entityid = entt::entity;
/// @brief Canonical invalid entity id sentinel.
constexpr entityid ENTITYID_INVALID = entt::null;
/// @brief Generic invalid sentinel used throughout older gameplay code.
constexpr entityid INVALID = entt::null;
