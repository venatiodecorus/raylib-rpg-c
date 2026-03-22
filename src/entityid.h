/** @file entityid.h
 *  @brief Shared entity id alias and invalid-id sentinel macros.
 */

#pragma once

/// @brief Integer identifier type used for all runtime entities.
typedef int entityid;
/// @brief Canonical invalid entity id sentinel.
#define ENTITYID_INVALID -1
/// @brief Generic invalid sentinel used throughout older gameplay code.
#define INVALID -1
