
CXX = clang++

WFLAGS = -Wall -Werror -Wno-unused-but-set-variable -std=c++17

SRCDIR = src

MAIN_C = $(SRCDIR)/main.cpp
LIBDRAW_C = $(SRCDIR)/libdraw.cpp
LIBDRAW_SPLIT_C = $(addprefix $(SRCDIR)/, \
	create_sg_byid.cpp \
	libdraw_create_spritegroup.cpp \
	libdraw_update_sprites.cpp \
	set_sg.cpp)
AUX_DRAW_C = $(addprefix $(SRCDIR)/, \
	draw_action_menu.cpp \
	draw_character_creation_screen.cpp \
	draw_controls_menu.cpp \
	draw_damage_numbers.cpp \
	draw_dungeon_floor.cpp \
	draw_entity_sprite.cpp \
	draw_handle_debug_panel.cpp \
	draw_item_detail_text.cpp \
	draw_help_menu.cpp \
	draw_hud.cpp \
	draw_interaction_modal.cpp \
	draw_chest_menu.cpp \
	draw_inventory_menu.cpp \
	draw_level_up_modal.cpp \
	draw_look_panel.cpp \
	draw_message_box.cpp \
	draw_message_history.cpp \
	draw_mini_inventory_menu.cpp \
	draw_option_menu.cpp \
	draw_sound_menu.cpp \
	draw_title_screen.cpp \
	libdraw_gameplay_scene.cpp \
	libdraw_frontend_scenes.cpp \
	draw_window_color_menu.cpp)
AUDIO_MANAGER_C = $(SRCDIR)/audio_manager.cpp
MESSAGE_LOG_C = $(SRCDIR)/message_log.cpp
DAMAGE_POPUPS_C = $(SRCDIR)/damage_popups.cpp
GAMESTATE_C = $(addprefix $(SRCDIR)/, \
	gamestate_lifecycle.cpp \
	gamestate_scene.cpp \
	gamestate_inventory.cpp \
	gamestate_keybinding.cpp \
	gamestate_options.cpp \
	gamestate_input.cpp \
	gamestate_npc_combat.cpp \
	gamestate_world.cpp \
	world_object_definitions.cpp \
	item_definitions.cpp \
	prop_definitions.cpp \
	actor_definitions.cpp \
	gamestate_world_interaction.cpp \
	gamestate_entity_factory.cpp)
GAMESTATE_O = $(GAMESTATE_C:.cpp=.o)
GAME_SOURCES = $(MAIN_C) $(LIBDRAW_C) $(LIBDRAW_SPLIT_C) $(AUX_DRAW_C) $(AUDIO_MANAGER_C) $(MESSAGE_LOG_C) $(DAMAGE_POPUPS_C) $(GAMESTATE_C)

LINK_MATH = -lm

# Platform detection
UNAME_S := $(shell uname -s)

# Use pkg-config for raylib include/link flags
RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib)
RAYLIB_LDFLAGS := $(shell pkg-config --libs raylib)

ifeq ($(UNAME_S),Darwin)
    # macOS needs these frameworks for raylib
    RAYLIB_LIBS = $(RAYLIB_LDFLAGS) -framework IOKit -framework Cocoa -framework OpenGL
else
    # Linux
    RAYLIB_LIBS = $(RAYLIB_LDFLAGS) -lGL -lm -lpthread -ldl -lrt -lX11
endif

CFLAGS ?= # Allow override

# Web build setup
WEB_CC = emcc
WEB_SOURCES = $(GAME_SOURCES)
WEB_INCLUDES = -I$(SRCDIR) -Iinclude -I /usr/local/include
WEB_LINKS = -L$(SRCDIR) -l:libraylib.web.a
EMCC_OPTIONS = -s USE_GLFW=3 -s EXPORTED_RUNTIME_METHODS=ccall -s ALLOW_MEMORY_GROWTH
SHELL_FILE = --shell-file $(SRCDIR)/minshell.html
PRELOAD_FILES = --preload-file $(SRCDIR)/fonts --preload-file $(SRCDIR)/img --preload-file $(SRCDIR)/audio/music --preload-file $(SRCDIR)/music.txt --preload-file $(SRCDIR)/audio/sfx --preload-file $(SRCDIR)/sfx.txt --preload-file $(SRCDIR)/shaders
WEB_OPTIONS = -DPLATFORM_WEB -DWEB -DSPAWN_MONSTERS -DNPCS_ALL_AT_ONCE -DSTART_MESSAGES


# Phony targets
.PHONY: all clean docs docs-clean game tests tests_heavy

# Targets
all: game tests

# Desktop build
game: $(SRCDIR)/main.o $(SRCDIR)/libdraw.o $(LIBDRAW_SPLIT_C:.cpp=.o) $(AUX_DRAW_C:.cpp=.o) $(SRCDIR)/audio_manager.o $(SRCDIR)/message_log.o $(SRCDIR)/damage_popups.o $(GAMESTATE_O)
	$(CXX) $(WFLAGS) $(CXXFLAGS) $(CFLAGS) $^ $(RAYLIB_LIBS) -o $@

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(WFLAGS) $(CXXFLAGS) $(CFLAGS) $(RAYLIB_CFLAGS) -I$(SRCDIR) -Iinclude -c $< -o $@

# Web build
index.html: $(GAME_SOURCES)
	$(WEB_CC) -o $@ $(GAME_SOURCES) $(WEB_INCLUDES) $(WEB_LINKS) $(EMCC_OPTIONS) $(SHELL_FILE) $(PRELOAD_FILES) $(WEB_OPTIONS) $(CFLAGS) $(CXXFLAGS)

# Unit tests
$(SRCDIR)/tests.cpp: $(SRCDIR)/unit_test.h $(wildcard $(SRCDIR)/test_suites/test_*.h)
	cxxtestgen --error-printer -o $@ $(SRCDIR)/unit_test.h $(SRCDIR)/test_suites/test_gamestate_lifecycle.h $(SRCDIR)/test_suites/test_combat_bootstrap.h $(SRCDIR)/test_suites/test_dungeon_bootstrap.h $(SRCDIR)/test_suites/test_entity_factory.h $(SRCDIR)/test_suites/test_entity_placement.h $(SRCDIR)/test_suites/test_inventory.h $(SRCDIR)/test_suites/test_pathfinder.h $(SRCDIR)/test_suites/test_renderer_seams.h $(SRCDIR)/test_suites/test_tile_cache.h $(SRCDIR)/test_suites/test_utility_helpers.h $(SRCDIR)/test_suites/test_world_interaction.h

tests: $(SRCDIR)/tests.cpp $(SRCDIR)/audio_manager.o $(SRCDIR)/message_log.o $(SRCDIR)/damage_popups.o $(GAMESTATE_O)
	$(CXX) -o $@ $^ $(RAYLIB_CFLAGS) $(RAYLIB_LIBS) -I$(SRCDIR) -Iinclude $(CXXFLAGS) $(CFLAGS)

$(SRCDIR)/tests_heavy.cpp: $(SRCDIR)/unit_test_heavy.h $(SRCDIR)/unit_test.h $(wildcard $(SRCDIR)/test_suites/test_*.h)
	cxxtestgen --error-printer -o $@ $(SRCDIR)/unit_test_heavy.h $(SRCDIR)/unit_test.h $(SRCDIR)/test_suites/test_gamestate_lifecycle.h $(SRCDIR)/test_suites/test_combat_bootstrap.h $(SRCDIR)/test_suites/test_dungeon_bootstrap.h $(SRCDIR)/test_suites/test_entity_factory.h $(SRCDIR)/test_suites/test_entity_placement.h $(SRCDIR)/test_suites/test_heavy_simulation.h $(SRCDIR)/test_suites/test_inventory.h $(SRCDIR)/test_suites/test_pathfinder.h $(SRCDIR)/test_suites/test_renderer_seams.h $(SRCDIR)/test_suites/test_tile_cache.h $(SRCDIR)/test_suites/test_utility_helpers.h $(SRCDIR)/test_suites/test_world_interaction.h

tests_heavy: $(SRCDIR)/tests_heavy.cpp $(SRCDIR)/audio_manager.o $(SRCDIR)/message_log.o $(SRCDIR)/damage_popups.o $(GAMESTATE_O)
	$(CXX) -o $@ $^ $(RAYLIB_CFLAGS) $(RAYLIB_LIBS) -I$(SRCDIR) -Iinclude $(CXXFLAGS) $(CFLAGS)

# Documentation
docs:
	doxygen $(SRCDIR)/Doxyfile

docs-clean:
	rm -rf docs/api

clean:
	rm -rfv game tests tests_heavy index.html index.js index.wasm index.data $(SRCDIR)/*.o $(SRCDIR)/*.gch $(SRCDIR)/tests.cpp $(SRCDIR)/tests_heavy.cpp
