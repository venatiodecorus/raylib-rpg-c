#pragma once

#include "../alignment.h"
#include "../direction.h"
#include "../option_menu.h"
#include "../roll.h"
#include "../stat_bonus.h"
#include <cstdlib>
#include <cxxtest/TestSuite.h>

class UtilityHelpersExpandedTestSuite : public CxxTest::TestSuite {
public:
    void testDoRollReturnsWithinExpectedRange() {
        srand(12345);
        const vec3 dice = {1, 6, 0};
        for (int i = 0; i < 100; i++) {
            const int result = do_roll(dice);
            TS_ASSERT(result >= 1);
            TS_ASSERT(result <= 6);
        }
    }

    void testDoRollWithModifier() {
        const vec3 dice = {1, 1, 5};
        for (int i = 0; i < 10; i++) {
            TS_ASSERT_EQUALS(do_roll(dice), 6);
        }
    }

    void testDoRollBestOfThreeReturnsHighest() {
        srand(99999);
        const vec3 dice = {2, 6, 0};
        for (int i = 0; i < 50; i++) {
            const unsigned int seed = static_cast<unsigned int>(i + 1);
            srand(seed);
            const int best = do_roll_best_of_3(dice);
            srand(seed);
            const int single = do_roll(dice);
            TS_ASSERT(best >= single);
        }
    }

    void testGetStatBonusTable() {
        TS_ASSERT_EQUALS(get_stat_bonus(1), -5);
        TS_ASSERT_EQUALS(get_stat_bonus(8), -1);
        TS_ASSERT_EQUALS(get_stat_bonus(9), -1);
        TS_ASSERT_EQUALS(get_stat_bonus(10), 0);
        TS_ASSERT_EQUALS(get_stat_bonus(11), 0);
        TS_ASSERT_EQUALS(get_stat_bonus(12), 1);
        TS_ASSERT_EQUALS(get_stat_bonus(20), 5);
    }

    void testGetStatBonusEdgeCases() {
        TS_ASSERT_EQUALS(get_stat_bonus(0), -5);
        TS_ASSERT_EQUALS(get_stat_bonus(-10), -10);
    }

    void testDirectionGetOpposite() {
        TS_ASSERT_EQUALS(get_opposite_dir(direction_t::UP), direction_t::DOWN);
        TS_ASSERT_EQUALS(get_opposite_dir(direction_t::DOWN), direction_t::UP);
        TS_ASSERT_EQUALS(get_opposite_dir(direction_t::LEFT), direction_t::RIGHT);
        TS_ASSERT_EQUALS(get_opposite_dir(direction_t::RIGHT), direction_t::LEFT);
        TS_ASSERT_EQUALS(get_opposite_dir(direction_t::UP_LEFT), direction_t::DOWN_RIGHT);
        TS_ASSERT_EQUALS(get_opposite_dir(direction_t::UP_RIGHT), direction_t::DOWN_LEFT);
        TS_ASSERT_EQUALS(get_opposite_dir(direction_t::DOWN_LEFT), direction_t::UP_RIGHT);
        TS_ASSERT_EQUALS(get_opposite_dir(direction_t::DOWN_RIGHT), direction_t::UP_LEFT);

        // Roundtrip: opposite of opposite is original.
        TS_ASSERT_EQUALS(get_opposite_dir(get_opposite_dir(direction_t::UP)), direction_t::UP);
        TS_ASSERT_EQUALS(get_opposite_dir(get_opposite_dir(direction_t::DOWN_LEFT)), direction_t::DOWN_LEFT);
    }

    void testDirectionGetLocFromDir() {
        TS_ASSERT(vec3_equal(get_loc_from_dir(direction_t::UP), vec3{0, -1, 0}));
        TS_ASSERT(vec3_equal(get_loc_from_dir(direction_t::DOWN), vec3{0, 1, 0}));
        TS_ASSERT(vec3_equal(get_loc_from_dir(direction_t::LEFT), vec3{-1, 0, 0}));
        TS_ASSERT(vec3_equal(get_loc_from_dir(direction_t::RIGHT), vec3{1, 0, 0}));
        TS_ASSERT(vec3_equal(get_loc_from_dir(direction_t::NONE), vec3{-1, -1, -1}));
        TS_ASSERT(vec3_equal(get_loc_from_dir(direction_t::UNKNOWN), vec3{-1, -1, -1}));
    }

    void testDirectionRoundtrip() {
        const direction_t dirs[] = {
            direction_t::UP, direction_t::DOWN, direction_t::LEFT, direction_t::RIGHT,
            direction_t::UP_LEFT, direction_t::UP_RIGHT, direction_t::DOWN_LEFT, direction_t::DOWN_RIGHT};
        for (const direction_t d : dirs) {
            const int dx = get_x_from_dir(d);
            const int dy = get_y_from_dir(d);
            TS_ASSERT_EQUALS(get_dir_from_xy(dx, dy), d);
        }
    }

    void testDirectionFromXyEdgeCases() {
        TS_ASSERT_EQUALS(get_dir_from_xy(0, 0), direction_t::NONE);
        TS_ASSERT_EQUALS(get_dir_from_xy(2, 0), direction_t::UNKNOWN);
    }

    void testAlignmentNextCyclesCorrectly() {
        TS_ASSERT_EQUALS(alignment_next(alignment_t::GOOD_LAWFUL), alignment_t::GOOD_NEUTRAL);
        TS_ASSERT_EQUALS(alignment_next(alignment_t::EVIL_CHAOTIC), alignment_t::GOOD_LAWFUL);
    }

    void testAlignmentPrevCyclesCorrectly() {
        TS_ASSERT_EQUALS(alignment_prev(alignment_t::GOOD_LAWFUL), alignment_t::EVIL_CHAOTIC);
        TS_ASSERT_EQUALS(alignment_prev(alignment_t::GOOD_NEUTRAL), alignment_t::GOOD_LAWFUL);
    }

    void testAlignmentFullCycle() {
        alignment_t a = alignment_t::GOOD_LAWFUL;
        for (int i = 0; i < 9; i++) {
            a = alignment_next(a);
        }
        TS_ASSERT_EQUALS(a, alignment_t::GOOD_LAWFUL);
    }

    void testAlignmentToStr() {
        TS_ASSERT_EQUALS(alignment_to_str(alignment_t::GOOD_LAWFUL), "lawful good");
        TS_ASSERT_EQUALS(alignment_to_str(alignment_t::NEUTRAL_NEUTRAL), "true neutral");
        TS_ASSERT_EQUALS(alignment_to_str(alignment_t::EVIL_CHAOTIC), "chaotic evil");
        TS_ASSERT_EQUALS(alignment_to_str(alignment_t::NONE), "none");
    }

    void testOptionMenuDefaultState() {
        option_menu menu;
        TS_ASSERT_EQUALS(menu.get_option_count(), 4U);
        TS_ASSERT_EQUALS(menu.get_selection(), 0U);
        TS_ASSERT_EQUALS(menu.get_option(0), option_type::SOUND);
        TS_ASSERT_EQUALS(menu.get_option(3), option_type::INVENTORY_MENU);
    }

    void testOptionMenuIncrDecrClamping() {
        option_menu menu;

        // Decrement at 0 stays at 0.
        menu.decr_selection();
        TS_ASSERT_EQUALS(menu.get_selection(), 0U);

        // Increment to max (3 for 4 options).
        menu.incr_selection();
        menu.incr_selection();
        menu.incr_selection();
        TS_ASSERT_EQUALS(menu.get_selection(), 3U);

        // Increment at max stays at max.
        menu.incr_selection();
        TS_ASSERT_EQUALS(menu.get_selection(), 3U);
    }
};
