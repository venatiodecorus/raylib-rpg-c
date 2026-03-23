/** @file test_stubs.cpp
 *  @brief Stub implementations of rendering functions required by entity .o files
 *         but not needed for unit test logic. Linked only into test binaries.
 */

#include "libdraw_create_spritegroup.h"

bool create_spritegroup(gamestate&, rpg::Renderer&, entityid, const rpg::SpriteDef*, int, int, int) {
    return false;
}

bool create_sg(gamestate&, rpg::Renderer&, entityid, const rpg::SpriteDef*, int) {
    return false;
}
