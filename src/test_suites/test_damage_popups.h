#pragma once

#include "../damage_popups.h"
#include <cxxtest/TestSuite.h>
#include <random>

class DamagePopupsTestSuite : public CxxTest::TestSuite {
public:
    void testAddZeroAmountIsNoOp() {
        rpg::DamagePopups dp;
        std::mt19937 rng(42);

        dp.add(2, 3, 0, 0, false, rng);

        TS_ASSERT(dp.popups.empty());
    }

    void testAddNegativeAmountIsNoOp() {
        rpg::DamagePopups dp;
        std::mt19937 rng(42);

        dp.add(2, 3, 0, -1, false, rng);

        TS_ASSERT(dp.popups.empty());
    }

    void testAddCreatesPopupWithCorrectFields() {
        rpg::DamagePopups dp;
        std::mt19937 rng(42);

        dp.add(2, 3, 0, 5, false, rng);

        TS_ASSERT_EQUALS(dp.popups.size(), 1U);
        TS_ASSERT_EQUALS(dp.popups[0].amount, 5);
        TS_ASSERT_EQUALS(dp.popups[0].floor, 0);
        TS_ASSERT_DELTA(dp.popups[0].age_seconds, 0.0f, 0.001f);
        TS_ASSERT(!dp.popups[0].critical);
    }

    void testAddStacksMultipleOnSameTile() {
        rpg::DamagePopups dp;
        std::mt19937 rng(42);

        dp.add(2, 3, 0, 5, false, rng);
        dp.add(2, 3, 0, 7, false, rng);

        TS_ASSERT_EQUALS(dp.popups.size(), 2U);
        TS_ASSERT_DIFFERS(dp.popups[0].rise_distance, dp.popups[1].rise_distance);
    }

    void testAddCapsAtFortyEight() {
        rpg::DamagePopups dp;
        std::mt19937 rng(42);

        for (int i = 0; i < 49; ++i) {
            dp.add(i % 8, i / 8, 0, 1 + i, false, rng);
        }

        TS_ASSERT_EQUALS(dp.popups.size(), 48U);
    }

    void testUpdateExpiresPopupsAtLifetime() {
        rpg::DamagePopups dp;
        std::mt19937 rng(42);

        dp.add(1, 1, 0, 3, false, rng);
        TS_ASSERT_EQUALS(dp.popups.size(), 1U);

        dp.update(0.3f);
        TS_ASSERT_EQUALS(dp.popups.size(), 1U);

        dp.update(0.4f);
        TS_ASSERT(dp.popups.empty());
    }

    void testUpdateNegativeDtClampsToZero() {
        rpg::DamagePopups dp;
        std::mt19937 rng(42);

        dp.add(1, 1, 0, 3, false, rng);
        dp.dirty = false;

        dp.update(-1.0f);

        TS_ASSERT_EQUALS(dp.popups.size(), 1U);
        TS_ASSERT_DELTA(dp.popups[0].age_seconds, 0.0f, 0.001f);
    }

    void testUpdateSetsDirtyFlag() {
        rpg::DamagePopups dp;
        std::mt19937 rng(42);

        dp.add(1, 1, 0, 3, false, rng);
        dp.dirty = false;

        dp.update(1.0f);

        TS_ASSERT(dp.dirty);
    }
};
