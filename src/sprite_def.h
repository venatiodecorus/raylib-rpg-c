#pragma once
#include <string>

namespace rpg {
enum texture_src_type {
    TXSRC_FILE,
    TXSRC_TILEMAP,
    TXSRC_PLACEHOLDER
};

struct SpriteDef {
    texture_src_type src_type;
    std::string path;
    int contexts;
    int frames;
    int src_x = 0;
    int src_y = 0;
    int src_w = 0;
    int src_h = 0;
};
}