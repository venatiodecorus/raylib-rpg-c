/** @file load_textures.h
 *  @brief Texture loading from code-defined texture definitions.
 *
 *  Supports three source types:
 *  - TXSRC_FILE:        Load an entire PNG file as a texture.
 *  - TXSRC_TILEMAP:     Extract a sub-rectangle from a tilemap PNG.
 *  - TXSRC_PLACEHOLDER: Generate a magenta placeholder texture.
 */

#pragma once

#include "massert.h"
#include "mprint.h"
#include "textureinfo.h"
#include "texture_defs.h"
#include <unordered_map>
#include <string>

/** @brief Cache of loaded tilemap images, keyed by file path. */
static std::unordered_map<std::string, Image> g_tilemap_cache;

/** @brief Unload all cached tilemap images. Call after load_textures(). */
static inline void unload_tilemap_cache() {
    for (auto& pair : g_tilemap_cache) {
        UnloadImage(pair.second);
    }
    g_tilemap_cache.clear();
}

/** @brief Get or load a tilemap image from cache. */
static inline Image get_tilemap_image(const char* path) {
    std::string key(path);
    auto it = g_tilemap_cache.find(key);
    if (it != g_tilemap_cache.end()) {
        return it->second;
    }
    Image img = LoadImage(path);
    massert(img.data != NULL, "Failed to load tilemap: %s", path);
    g_tilemap_cache[key] = img;
    return img;
}

/** @brief Generate a solid magenta placeholder image of given dimensions. */
static inline Image generate_placeholder_image(int width, int height) {
    Image img = GenImageColor(width, height, MAGENTA);
    return img;
}

/** @brief Load one texture from a texture_def entry. */
static inline bool load_texture_from_def(textureinfo* txinfo, const texture_def& def) {
    massert(txinfo != NULL, "txinfo is NULL");
    massert(def.txkey >= 0, "txkey is invalid");
    massert(def.txkey < GAMESTATE_SIZEOFTEXINFOARRAY, "txkey out of bounds: %d", def.txkey);

    Image image;
    switch (def.src_type) {
    case TXSRC_FILE: {
        massert(def.path != NULL, "TXSRC_FILE path is NULL for txkey %d", def.txkey);
        image = LoadImage(def.path);
        massert(image.data != NULL, "Failed to load image: %s (txkey %d)", def.path, def.txkey);
        break;
    }
    case TXSRC_TILEMAP: {
        massert(def.path != NULL, "TXSRC_TILEMAP path is NULL for txkey %d", def.txkey);
        Image tilemap = get_tilemap_image(def.path);
        Rectangle src = {
            static_cast<float>(def.src_x),
            static_cast<float>(def.src_y),
            static_cast<float>(def.src_w),
            static_cast<float>(def.src_h)
        };
        image = ImageFromImage(tilemap, src);
        massert(image.data != NULL, "Failed to crop tilemap %s at (%d,%d,%d,%d) for txkey %d",
                def.path, def.src_x, def.src_y, def.src_w, def.src_h, def.txkey);
        break;
    }
    case TXSRC_PLACEHOLDER: {
        int w = (def.src_w > 0) ? def.src_w : 32;
        int h = (def.src_h > 0) ? def.src_h : 32;
        image = generate_placeholder_image(w, h);
        break;
    }
    default:
        merror("Unknown src_type %d for txkey %d", def.src_type, def.txkey);
        return false;
    }

    if (def.dither)
        ImageDither(&image, 4, 4, 4, 4);

    Texture2D texture = LoadTextureFromImage(image);
    massert(texture.id != 0, "texture.id is 0 for txkey %d", def.txkey);

    // Only unload if we own the image (not tilemap cache)
    if (def.src_type != TXSRC_TILEMAP) {
        UnloadImage(image);
    } else {
        // ImageFromImage creates a copy, so we must unload it
        UnloadImage(image);
    }

    txinfo[def.txkey].texture = texture;
    txinfo[def.txkey].contexts = def.contexts;
    txinfo[def.txkey].num_frames = def.frames;
    return true;
}

/** @brief Load all textures from the compiled texture definitions table. */
static inline bool load_textures(textureinfo* txinfo) {
    massert(txinfo != NULL, "txinfo is NULL");

    for (int i = 0; i < TEXTURE_DEF_COUNT; i++) {
        const texture_def& def = TEXTURE_DEFS[i];
        bool loaded = load_texture_from_def(txinfo, def);
        if (!loaded) {
            merror("Failed to load texture txkey %d, hard-crashing!", def.txkey);
            exit(-1);
        }
    }

    // Free cached tilemap images now that all textures are on GPU
    unload_tilemap_cache();
    return true;
}
