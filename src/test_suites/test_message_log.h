#pragma once

#include "../message_log.h"
#include <cxxtest/TestSuite.h>

class MessageLogTestSuite : public CxxTest::TestSuite {
public:
    void testAddSetsIsActive() {
        rpg::MessageLog log;
        TS_ASSERT(!log.is_active);
        log.add("hello");
        TS_ASSERT(log.is_active);
        TS_ASSERT_EQUALS(log.system.size(), 1U);
    }

    void testAddFormatsCorrectly() {
        rpg::MessageLog log;
        log.add("val=%d", 42);
        TS_ASSERT_EQUALS(log.system.front(), "val=42");
    }

    void testAddHistoryDoesNotSetIsActive() {
        rpg::MessageLog log;
        log.add_history("x");
        TS_ASSERT(!log.is_active);
        TS_ASSERT_EQUALS(log.history.size(), 1U);
    }

    void testAddHistoryFormatsCorrectly() {
        rpg::MessageLog log;
        log.add_history("%s world", "hello");
        TS_ASSERT_EQUALS(log.history.front(), "hello world");
    }

    void testMultipleAddsQueueInOrder() {
        rpg::MessageLog log;
        log.add("a");
        log.add("b");
        TS_ASSERT_EQUALS(log.system.size(), 2U);
        TS_ASSERT_EQUALS(log.system[0], "a");
        TS_ASSERT_EQUALS(log.system[1], "b");
    }

    void testResetClearsAll() {
        rpg::MessageLog log;
        log.add("x");
        log.add_history("y");
        TS_ASSERT_EQUALS(log.system.size(), 1U);
        TS_ASSERT_EQUALS(log.history.size(), 1U);

        // A fresh MessageLog should be empty.
        rpg::MessageLog fresh;
        TS_ASSERT_EQUALS(fresh.system.size(), 0U);
        TS_ASSERT_EQUALS(fresh.history.size(), 0U);
        TS_ASSERT(!fresh.is_active);
    }
};
