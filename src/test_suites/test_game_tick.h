#pragma once

#include "../ecs_gameplay_components.h"
#include "../gamestate.h"
#include <cxxtest/TestSuite.h>

class GameTickTestSuite : public CxxTest::TestSuite {
private:
    void add_floor(gamestate& g, int width = 8, int height = 8) {
        auto df = g.d.create_floor(biome_t::STONE, width, height);
        df->df_set_all_tiles(tiletype_t::FLOOR_STONE_00);
        g.d.add_floor(df);
        g.d.is_initialized = true;
    }

public:
    void testAdvanceAnimationPhasePlayerAnimToNpcTurn() {
        gamestate g;
        g.flag = gamestate_flag_t::PLAYER_ANIM;

        g.advance_animation_phase();

        TS_ASSERT_EQUALS(g.flag, gamestate_flag_t::NPC_TURN);
    }

    void testAdvanceAnimationPhaseNpcAnimToPlayerInput() {
        gamestate g;
        g.flag = gamestate_flag_t::NPC_ANIM;
        const unsigned int before = g.turn_count;

        g.advance_animation_phase();

        TS_ASSERT_EQUALS(g.flag, gamestate_flag_t::PLAYER_INPUT);
        TS_ASSERT_EQUALS(g.turn_count, before + 1);
    }

    void testAdvanceAnimationPhasePlayerInputNoChange() {
        gamestate g;
        g.flag = gamestate_flag_t::PLAYER_INPUT;

        g.advance_animation_phase();

        TS_ASSERT_EQUALS(g.flag, gamestate_flag_t::PLAYER_INPUT);
    }

    void testAdvanceAnimationPhaseNpcTurnNoChange() {
        gamestate g;
        g.flag = gamestate_flag_t::NPC_TURN;

        g.advance_animation_phase();

        TS_ASSERT_EQUALS(g.flag, gamestate_flag_t::NPC_TURN);
    }

    void testSetGamestateFlagForAttackAnimationPlayer() {
        gamestate g;
        g.flag = gamestate_flag_t::PLAYER_INPUT;

        g.set_gamestate_flag_for_attack_animation(entitytype_t::PLAYER);

        TS_ASSERT_EQUALS(g.flag, gamestate_flag_t::PLAYER_ANIM);
    }

    void testSetGamestateFlagForAttackAnimationNpc() {
        gamestate g;
        g.flag = gamestate_flag_t::NPC_TURN;

        g.set_gamestate_flag_for_attack_animation(entitytype_t::NPC);

        TS_ASSERT_EQUALS(g.flag, gamestate_flag_t::NPC_ANIM);
    }
};
