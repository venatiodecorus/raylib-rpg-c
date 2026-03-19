/** @file direction.h
 *  @brief Direction enums and helpers for movement and facing vectors.
 */

#pragma once

#include "vec3.h"

/// @brief Cardinal and diagonal movement/facing directions used by gameplay logic.
enum class direction_t {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT,
    UNKNOWN,
    COUNT
};

/** @brief Return the opposite of the provided direction, or `direction_t::UNKNOWN`. */
constexpr static inline direction_t get_opposite_dir(direction_t dir) {
    switch (dir) {
    case direction_t::UP: return direction_t::DOWN;
    case direction_t::DOWN: return direction_t::UP;
    case direction_t::LEFT: return direction_t::RIGHT;
    case direction_t::RIGHT: return direction_t::LEFT;
    case direction_t::UP_LEFT: return direction_t::DOWN_RIGHT;
    case direction_t::UP_RIGHT: return direction_t::DOWN_LEFT;
    case direction_t::DOWN_LEFT: return direction_t::UP_RIGHT;
    case direction_t::DOWN_RIGHT: return direction_t::UP_LEFT;
    default: break;
    }
    return direction_t::UNKNOWN;
}

/** @brief Return the x-axis delta associated with a direction. */
constexpr static inline int get_x_from_dir(direction_t dir) {
    switch (dir) {
    case direction_t::UP_LEFT:
    case direction_t::DOWN_LEFT:
    case direction_t::LEFT: return -1;
    case direction_t::UP_RIGHT:
    case direction_t::DOWN_RIGHT:
    case direction_t::RIGHT: return 1;
    case direction_t::UP:
    case direction_t::DOWN:
    case direction_t::NONE:
    case direction_t::UNKNOWN:
    default: return 0;
    }
}

/** @brief Return the y-axis delta associated with a direction. */
constexpr static inline int get_y_from_dir(direction_t dir) {
    switch (dir) {
    case direction_t::UP:
    case direction_t::UP_LEFT:
    case direction_t::UP_RIGHT: return -1;
    case direction_t::DOWN:
    case direction_t::DOWN_LEFT:
    case direction_t::DOWN_RIGHT: return 1;
    case direction_t::LEFT:
    case direction_t::RIGHT:
    case direction_t::NONE:
    case direction_t::UNKNOWN:
    default: return 0;
    }
}

/** @brief Convert a direction into a unit-ish `vec3` tile delta. */
constexpr static inline vec3 get_loc_from_dir(direction_t dir) {
    vec3 loc = {0, 0, 0};
    switch (dir) {
    case direction_t::UP: loc = (vec3){0, -1, 0}; break;
    case direction_t::DOWN: loc = (vec3){0, 1, 0}; break;
    case direction_t::LEFT: loc = (vec3){-1, 0, 0}; break;
    case direction_t::RIGHT: loc = (vec3){1, 0, 0}; break;
    case direction_t::UP_LEFT: loc = (vec3){-1, -1, 0}; break;
    case direction_t::UP_RIGHT: loc = (vec3){1, -1, 0}; break;
    case direction_t::DOWN_LEFT: loc = (vec3){-1, 1, 0}; break;
    case direction_t::DOWN_RIGHT: loc = (vec3){1, 1, 0}; break;
    case direction_t::NONE:
    case direction_t::UNKNOWN:
    default: loc = (vec3){-1, -1, -1}; break;
    }
    return loc;
}

/** @brief Convert an `(x, y)` delta pair into a direction enum. */
constexpr static inline const direction_t get_dir_from_xy(int x, int y) {
    if (x == 0 && y == 0)
        return direction_t::NONE;
    if (x == 0 && y == -1)
        return direction_t::UP;
    if (x == 0 && y == 1)
        return direction_t::DOWN;
    if (x == -1 && y == 0)
        return direction_t::LEFT;
    if (x == 1 && y == 0)
        return direction_t::RIGHT;
    // also handle diagonals
    if (x == -1 && y == -1)
        return direction_t::UP_LEFT;
    if (x == 1 && y == -1)
        return direction_t::UP_RIGHT;
    if (x == -1 && y == 1)
        return direction_t::DOWN_LEFT;
    if (x == 1 && y == 1)
        return direction_t::DOWN_RIGHT;
    return direction_t::UNKNOWN;
}

/** @brief Convert a `vec3` tile delta into a direction enum using its x/y axes. */
constexpr static inline direction_t get_dir_from_loc(vec3 loc) {
    return get_dir_from_xy(loc.x, loc.y);
}

/** @brief Return a lowercase string label for a direction enum. */
constexpr static inline const char* get_dir_as_string(direction_t dir) {
    switch (dir) {
    case direction_t::UP: return "up";
    case direction_t::DOWN: return "down";
    case direction_t::LEFT: return "left";
    case direction_t::RIGHT: return "right";
    case direction_t::UP_LEFT: return "up_left";
    case direction_t::UP_RIGHT: return "up_right";
    case direction_t::DOWN_LEFT: return "down_left";
    case direction_t::DOWN_RIGHT: return "down_right";
    default: return "unknown";
    }
}
