#pragma once

/** @file pathfinder.h
 *  @brief Reusable A* pathfinder with flat-array storage and generation counters.
 *
 *  Designed for small tile grids (dungeon floors). All internal buffers are
 *  retained between calls so repeated pathfinding avoids per-call allocation.
 */

#include "vec3.h"
#include "dungeon_floor.h"
#include "dungeon_tile_type.h"
#include "entityid.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <vector>

class Pathfinder {
public:
    using PushableCheck = std::function<entityid(int, int, int)>;

    /** @brief Ensure internal buffers are large enough for a grid of the given size. */
    void resize(int w, int h) {
        if (w == width_ && h == height_) {
            return;
        }
        width_ = w;
        height_ = h;
        capacity_ = w * h;
        gen_.assign(capacity_, 0);
        g_score_.resize(capacity_);
        came_from_.resize(capacity_);
        current_gen_ = 0;
    }

    /**
     * @brief Run A* from @p start to @p goal on the given floor.
     *
     * Returns a reference to an internal buffer containing the path (excluding
     * the start position, including the goal). The buffer is valid until the
     * next call to find_path().
     *
     * @param pushable_check  Returns ENTITYID_INVALID when the tile is clear,
     *                        otherwise the blocking entity id.
     */
    const std::vector<vec3>& find_path(
        vec3 start,
        vec3 goal,
        const dungeon_floor& df,
        PushableCheck pushable_check)
    {
        result_.clear();

        if (vec3_equal(start, goal)) {
            return result_;
        }

        const int w = df.get_width();
        const int h = df.get_height();
        if (w != width_ || h != height_) {
            resize(w, h);
        }

        // Bump generation counter (O(1) "clear" of all per-cell state).
        if (++current_gen_ == 0) {
            // Wrapped — full reset.
            std::fill(gen_.begin(), gen_.end(), 0u);
            current_gen_ = 1;
        }

        const int floor_z = start.z;
        const int start_idx = to_idx(start);
        const int goal_idx = to_idx(goal);

        // Seed the start node.
        touch(start_idx);
        g_score_[start_idx] = 0.0f;
        came_from_[start_idx] = -1;

        pq_.clear();
        pq_.push_back({octile(start.x, start.y, goal.x, goal.y), start_idx});
        // Single-element heap is already valid.

        bool found = false;

        while (!pq_.empty()) {
            std::pop_heap(pq_.begin(), pq_.end(), PQGreater{});
            PQEntry current = pq_.back();
            pq_.pop_back();

            const int ci = current.idx;

            if (ci == goal_idx) {
                found = true;
                break;
            }

            // Lazy deletion: skip if this entry is stale (a better path to
            // this cell was already found and a newer PQ entry was pushed).
            if (!is_valid(ci)) {
                continue;
            }
            const float current_g = g_score_[ci];
            if (current.f - octile_from_idx(ci, goal_idx) > current_g + 1e-5f) {
                continue;
            }

            const int cx = ci % width_;
            const int cy = ci / width_;

            static constexpr int dx[] = {1, -1, 0, 0, 1, -1, 1, -1};
            static constexpr int dy[] = {0, 0, 1, -1, 1, 1, -1, -1};

            for (int d = 0; d < 8; ++d) {
                const int nx = cx + dx[d];
                const int ny = cy + dy[d];

                if (nx < 0 || nx >= w || ny < 0 || ny >= h) {
                    continue;
                }

                const int ni = ny * width_ + nx;
                const vec3 nv{nx, ny, floor_z};

                // Goal tile is always reachable (it is occupied by the target entity).
                if (ni != goal_idx) {
                    const tile_t& tile = df.tile_at(nv);
                    if (!tile_is_walkable(tile.get_type())) {
                        continue;
                    }
                    if (pushable_check(nx, ny, floor_z) != ENTITYID_INVALID) {
                        continue;
                    }
                }

                const bool diagonal = (dx[d] != 0 && dy[d] != 0);
                const float step_cost = diagonal ? 1.414f : 1.0f;
                const float tentative_g = current_g + step_cost;

                if (is_valid(ni) && tentative_g >= g_score_[ni]) {
                    continue;
                }

                touch(ni);
                g_score_[ni] = tentative_g;
                came_from_[ni] = ci;

                const float f = tentative_g + octile(nx, ny, goal.x, goal.y);
                pq_.push_back({f, ni});
                std::push_heap(pq_.begin(), pq_.end(), PQGreater{});
            }
        }

        if (found) {
            // Reconstruct path (goal → start), then reverse.
            int idx = goal_idx;
            while (idx != start_idx && idx != -1) {
                result_.push_back(from_idx(idx, floor_z));
                idx = came_from_[idx];
            }
            std::reverse(result_.begin(), result_.end());
        }

        return result_;
    }

private:
    int width_ = 0;
    int height_ = 0;
    int capacity_ = 0;

    uint32_t current_gen_ = 0;
    std::vector<uint32_t> gen_;
    std::vector<float>    g_score_;
    std::vector<int>      came_from_;

    struct PQEntry {
        float f;
        int   idx;
    };

    struct PQGreater {
        bool operator()(const PQEntry& a, const PQEntry& b) const {
            return a.f > b.f;
        }
    };

    std::vector<PQEntry> pq_;
    std::vector<vec3>    result_;

    /** @brief Mark cell as touched in the current generation. */
    void touch(int idx) {
        gen_[idx] = current_gen_;
    }

    /** @brief Return whether the cell has been visited in the current generation. */
    bool is_valid(int idx) const {
        return gen_[idx] == current_gen_;
    }

    int to_idx(vec3 v) const {
        return v.y * width_ + v.x;
    }

    vec3 from_idx(int idx, int z) const {
        return vec3{idx % width_, idx / width_, z};
    }

    /** @brief Octile distance heuristic — admissible for 8-directional movement. */
    static float octile(int x0, int y0, int x1, int y1) {
        const int adx = std::abs(x0 - x1);
        const int ady = std::abs(y0 - y1);
        const int mn = adx < ady ? adx : ady;
        const int mx = adx > ady ? adx : ady;
        return static_cast<float>(mx) + 0.414f * static_cast<float>(mn);
    }

    float octile_from_idx(int idx, int goal_idx) const {
        return octile(idx % width_, idx / width_, goal_idx % width_, goal_idx / width_);
    }
};
