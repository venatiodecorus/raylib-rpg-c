/** @file spritegroup.h
 *  @brief Grouped animation state for one entity's renderable sprites.
 */

#pragma once

#include "entityid.h"
#include "libgame_defines.h"
#include "sprite.h"
#include "vec3.h"
#include <memory>
#include <unordered_map>
#include <vector>

#define SPRITEGROUP_ANIM_QUEUE_MAX 32

/**
 * @brief Collection of sprites and movement state used to draw one entity.
 *
 * The group tracks the active animation index, screen destination, interpolation
 * offset, and visibility for a single entity's renderer-facing representation.
 */
class spritegroup {
public:
    int size;
    int capacity;
    int current; // the current animation sprite in the spritegroup
    int off_x;
    int off_y;
    int default_anim; // the default animation to return to after the timer expires
    int alpha;
    entityid id; // the id of the entity that this spritegroup belongs to
    Rectangle dest;
    Rectangle move;
    std::vector<std::shared_ptr<sprite>> sprites;
    float move_rate;
    bool visible;

    /** @brief Construct an empty spritegroup with the requested sprite capacity. */
    spritegroup(int cap) {
        massert(cap > 0, "cap must be greater than 0, got %d", cap);
        current = 0;
        capacity = cap;
        size = 0;
        off_x = 0;
        off_y = 0;
        default_anim = 0;
        alpha = 255;
        id = 0;
        sprites.reserve(cap);
        dest = move = Rectangle{0, 0, 0, 0};
        move_rate = 1.0;
        visible = true;
    }

    /** @brief Return the sprite at `index`. */
    std::shared_ptr<sprite> get(int index) {
        return sprites.at(index);
    }

    /** @brief Return the number of sprites currently stored in the group. */
    size_t count() {
        return sprites.size();
    }

    /** @brief Return the sprite for the currently selected animation. */
    std::shared_ptr<sprite> get_current() {
        return sprites.at(current);
    }

    /** @brief Append a sprite to the group. */
    void add(std::shared_ptr<sprite> s) {
        massert(s, "s is null");
        massert(size < capacity, "size %d is >= capacity %d", size, capacity);

        //if (!s || size >= capacity) {
        //    return;
        //}
        sprites.push_back(s);
        size++;
    }

    /** @brief Apply the same context row to every sprite in the group. */
    void setcontexts(int context) {
        for (int i = 0; i < size; i++) {
            auto s = sprites.at(i);
            if (!s || s->get_numcontexts() <= 0 || context < 0 || context >= s->get_numcontexts()) {
                continue;
            }
            s->set_context(context);
        }
    }

    /** @brief Configure whether the active sprite should stop on its last frame. */
    void set_stop_on_last_frame(bool do_stop) {
        auto s = get_current();
        massert(s, "sprite is NULL");
        s->set_stop_on_last_frame(do_stop);
    }

    /** @brief Set the animation index the group should return to after transient anims. */
    void set_default_anim(int anim) {
        default_anim = anim;
    }

    /**
     * @brief Snap the destination rectangle to a tile position if not already moving.
     * @return `true` when the snap succeeded, otherwise `false`.
     */
    bool snap_dest(int x, int y) {
        if (move.x != 0 || move.y != 0)
            return false;
        dest.x = x * DEFAULT_TILE_SIZE + off_x;
        dest.y = y * DEFAULT_TILE_SIZE + off_y;
        return true;
    }

    /**
     * @brief Change the active animation sprite and reset its frame state.
     * @return `true` after switching to the requested animation index.
     */
    bool set_current(int index) {
        minfo2("spritegroup set current");
        massert(index >= 0, "index is negative: %d, %d", index, size);
        massert(index < size, "index is out of bounds for id %d: %d, %d", id, index, size);
        current = index;
        // lets update the sprite's current frame and numloops to 0
        // since we prob want to start an animation at the beginning if we are changing current
        sprites.at(current)->zero_currentframe_numloops();
        return true;
    }

    /**
     * @brief Advance interpolated movement toward the destination rectangle.
     * @return `true` while movement was still in progress this call.
     */
    bool update_dest() {
        bool retval = false;
        if (move.x > 0) {
            dest.x += move_rate;
            move.x -= move_rate;
            retval = true;
        }
        else if (move.x < 0) {
            dest.x -= move_rate;
            move.x += move_rate;
            retval = true;
        }
        if (move.y > 0) {
            dest.y += move_rate;
            move.y -= move_rate;
            retval = true;
        }
        else if (move.y < 0) {
            dest.y -= move_rate;
            move.y += move_rate;
            retval = true;
        }
        return retval;
    }
};
