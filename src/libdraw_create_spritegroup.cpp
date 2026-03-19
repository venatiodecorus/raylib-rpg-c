#include "libdraw_create_spritegroup.h"

#include "entitytype.h"
#include "libdraw.h"

#include <memory>

bool create_spritegroup(gamestate& g, rpg::Renderer& renderer, entityid id, const rpg::SpriteDef* sprites, int num_sprites, int offset_x, int offset_y) {
    minfo("BEGIN create_spritegroup");
    auto group = std::make_unique<spritegroup>(SPRITEGROUP_DEFAULT_SIZE);

    massert(group, "spritegroup is NULL");
    auto maybe_loc = g.ct.get<location>(id);

    minfo("checking if has location");
    if (maybe_loc.has_value()) {
        minfo("it DOES have a location");
        const vec3 loc = maybe_loc.value();
        massert(loc.z >= 0 && static_cast<size_t>(loc.z) < g.d.get_floor_count(), "location z out of bounds: %d", loc.z);
        auto df = g.d.get_floor(loc.z);
        massert(df, "dungeon floor is NULL");
        massert(loc.x >= 0 && loc.x < df->get_width(), "location x out of bounds: %d", loc.x);
        massert(loc.y >= 0 && loc.y < df->get_height(), "location y out of bounds: %d", loc.y);

        minfo2("creating spritegroups...");
        minfo2("num_sprites: %d", num_sprites);
        int count = 0;
        for (int i = 0; i < num_sprites; i++) {
            const rpg::SpriteDef& def = sprites[i];
            renderer.load_sprite_texture(def);
            std::string cache_key =
                def.src_type == rpg::TXSRC_FILE ? def.path :
                def.src_type == rpg::TXSRC_PLACEHOLDER ?
                    ("__placeholder_" + std::to_string(def.src_w > 0 ? def.src_w : 32) + "x" + std::to_string(def.src_h > 0 ? def.src_h : 32)) :
                    (def.path + "@" + std::to_string(def.src_x) + "," + std::to_string(def.src_y) + "," + std::to_string(def.src_w) + "," + std::to_string(def.src_h));
            auto s = make_shared<sprite>(&renderer.texture_cache.at(cache_key), def.contexts, def.frames);
            massert(s, "s is NULL for some reason!");
            group->add(s);
            count++;
        }
        msuccess2("spritegroups created");
        minfo2("count: %d", count);
        minfo2("setting id: %d", id);
        group->id = id;

        string n = g.ct.get<name>(id).value_or("no-name");
        minfo2("name: %s", n.c_str());
        entitytype_t t = g.ct.get<entitytype>(id).value_or(entitytype_t::NONE);
        string t_s = entitytype_to_str(t);
        minfo2("type: %s", t_s.c_str());
        minfo2("group->get(0)");
        minfo2("group->count(): %lu", group->count());

        auto s = group->get(0);
        massert(s, "sprite is NULL");

        minfo2("group->current = 0");
        group->current = 0;

        float x = loc.x * DEFAULT_TILE_SIZE + offset_x;
        float y = loc.y * DEFAULT_TILE_SIZE + offset_y;
        minfo("getting width");
        float w = s->get_width();
        minfo("getting height");
        float h = s->get_height();

        minfo("setting destination...");
        group->dest = Rectangle{x, y, w, h};
        group->off_x = offset_x;
        group->off_y = offset_y;
        renderer.spritegroups[id] = std::move(group);

        msuccess("END create spritegroup");
        return true;
    }

    minfo("it does NOT have a location");
    for (int i = 0; i < num_sprites; i++) {
        const rpg::SpriteDef& def = sprites[i];
        renderer.load_sprite_texture(def);
        std::string cache_key2 =
            def.src_type == rpg::TXSRC_FILE ? def.path :
            def.src_type == rpg::TXSRC_PLACEHOLDER ?
                ("__placeholder_" + std::to_string(def.src_w > 0 ? def.src_w : 32) + "x" + std::to_string(def.src_h > 0 ? def.src_h : 32)) :
                (def.path + "@" + std::to_string(def.src_x) + "," + std::to_string(def.src_y) + "," + std::to_string(def.src_w) + "," + std::to_string(def.src_h));
        auto s = make_shared<sprite>(&renderer.texture_cache.at(cache_key2), def.contexts, def.frames);
        group->add(s);
    }
    group->id = id;
    group->current = 0;
    auto s = group->get(0);
    massert(s, "sprite is NULL");
    float x = -DEFAULT_TILE_SIZE + offset_x;
    float y = -DEFAULT_TILE_SIZE + offset_y;
    float w = s->get_width();
    float h = s->get_height();
    group->dest = Rectangle{x, y, w, h};
    group->off_x = offset_x;
    group->off_y = offset_y;
    renderer.spritegroups[id] = std::move(group);
    msuccess("END create spritegroup");
    return true;
}

bool create_sg(gamestate& g, rpg::Renderer& renderer, entityid id, const rpg::SpriteDef* sprites, int num_sprites) {
    return create_spritegroup(g, renderer, id, sprites, num_sprites, -12, -12);
}
