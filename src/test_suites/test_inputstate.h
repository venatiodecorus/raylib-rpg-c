#pragma once

#include "../inputstate.h"
#include <cxxtest/TestSuite.h>

class InputstateTestSuite : public CxxTest::TestSuite {
private:
    void set_pressed(inputstate& is, int key) {
        const int idx = key / BITS_PER_LONG;
        const int bit = key % BITS_PER_LONG;
        is.pressed[idx] |= (1ULL << bit);
    }

    void set_held(inputstate& is, int key) {
        const int idx = key / BITS_PER_LONG;
        const int bit = key % BITS_PER_LONG;
        is.held[idx] |= (1ULL << bit);
    }

public:
    void testResetClearsAllBits() {
        inputstate is = {};
        set_pressed(is, 65);
        set_pressed(is, 200);
        set_held(is, 100);
        is.mouse_pressed[0] = true;

        inputstate_reset(is);

        for (int i = 0; i < NUM_LONGS; i++) {
            TS_ASSERT_EQUALS(is.pressed[i], 0ULL);
            TS_ASSERT_EQUALS(is.held[i], 0ULL);
        }
        for (int i = 0; i < 3; i++) {
            TS_ASSERT(!is.mouse_pressed[i]);
            TS_ASSERT(!is.mouse_held[i]);
            TS_ASSERT(!is.mouse_released[i]);
        }
    }

    void testIsPressedReturnsTrueForSetBit() {
        inputstate is = {};
        inputstate_reset(is);
        set_pressed(is, 65); // KEY_A
        TS_ASSERT(inputstate_is_pressed(is, 65));
    }

    void testIsPressedReturnsFalseForUnsetBit() {
        inputstate is = {};
        inputstate_reset(is);
        TS_ASSERT(!inputstate_is_pressed(is, 65));
    }

    void testIsPressedBoundaryKeys() {
        inputstate is = {};
        inputstate_reset(is);

        set_pressed(is, 0);
        TS_ASSERT(inputstate_is_pressed(is, 0));

        set_pressed(is, 63);
        TS_ASSERT(inputstate_is_pressed(is, 63));

        set_pressed(is, 64);
        TS_ASSERT(inputstate_is_pressed(is, 64));

        set_pressed(is, 399);
        TS_ASSERT(inputstate_is_pressed(is, 399));
    }

    void testIsPressedOutOfRange() {
        inputstate is = {};
        inputstate_reset(is);
        TS_ASSERT(!inputstate_is_pressed(is, -1));
        TS_ASSERT(!inputstate_is_pressed(is, 400));
    }

    void testAnyPressedDetectsAnySingleKey() {
        inputstate is = {};
        inputstate_reset(is);
        set_pressed(is, 42);
        TS_ASSERT(inputstate_any_pressed(is));
    }

    void testAnyPressedReturnsFalseWhenEmpty() {
        inputstate is = {};
        inputstate_reset(is);
        TS_ASSERT(!inputstate_any_pressed(is));
    }

    void testIsHeldWorks() {
        inputstate is = {};
        inputstate_reset(is);
        set_held(is, 75);
        TS_ASSERT(inputstate_is_held(is, 75));
        TS_ASSERT(!inputstate_is_held(is, 76));
    }

    void testGetPressedKeyReturnsLowest() {
        inputstate is = {};
        inputstate_reset(is);
        set_pressed(is, 100);
        set_pressed(is, 50);
        TS_ASSERT_EQUALS(inputstate_get_pressed_key(is), 50);
    }

    void testGetPressedKeyReturnsNegativeOneWhenEmpty() {
        inputstate is = {};
        inputstate_reset(is);
        TS_ASSERT_EQUALS(inputstate_get_pressed_key(is), -1);
    }

    void testIsShiftHeldCombinations() {
        inputstate is = {};

        // Left shift only.
        inputstate_reset(is);
        set_held(is, KEY_LEFT_SHIFT);
        TS_ASSERT(inputstate_is_shift_held(is));
        TS_ASSERT(inputstate_is_left_shift_held(is));
        TS_ASSERT(!inputstate_is_right_shift_held(is));

        // Right shift only.
        inputstate_reset(is);
        set_held(is, KEY_RIGHT_SHIFT);
        TS_ASSERT(inputstate_is_shift_held(is));
        TS_ASSERT(!inputstate_is_left_shift_held(is));
        TS_ASSERT(inputstate_is_right_shift_held(is));

        // Both shifts.
        inputstate_reset(is);
        set_held(is, KEY_LEFT_SHIFT);
        set_held(is, KEY_RIGHT_SHIFT);
        TS_ASSERT(inputstate_is_shift_held(is));

        // Neither shift.
        inputstate_reset(is);
        TS_ASSERT(!inputstate_is_shift_held(is));
    }
};
