
CXX = clang++

WFLAGS = -Wall -Werror -Wno-unused-but-set-variable -std=c++17

SRCDIR = src

MAIN_C = $(SRCDIR)/main.cpp
LIBDRAW_C = $(SRCDIR)/libdraw.cpp
LIBDRAW_SPLIT_C = $(addprefix $(SRCDIR)/, \
	libdraw_create_spritegroup.cpp \
	libdraw_update_sprites.cpp \
	set_sg.cpp)
AUX_DRAW_C = $(addprefix $(SRCDIR)/, \
	draw_handle_debug_panel.cpp \
	draw_title_screen.cpp \
	draw_character_creation_screen.cpp \
	libdraw_gameplay_scene.cpp \
	libdraw_frontend_scenes.cpp \
	world_renderer.cpp \
	menu_renderer.cpp \
	inventory_renderer.cpp \
	hud_renderer.cpp)
SCENES_C = $(addprefix $(SRCDIR)/, \
	title_scene.cpp \
	main_menu_scene.cpp \
	character_creation_scene.cpp \
	gameplay_scene.cpp)
AUDIO_MANAGER_C = $(SRCDIR)/audio_manager.cpp
MESSAGE_LOG_C = $(SRCDIR)/message_log.cpp
DAMAGE_POPUPS_C = $(SRCDIR)/damage_popups.cpp
ENTITIES_C = $(addprefix $(SRCDIR)/entities/, \
	actor.cpp \
	box.cpp \
	chest.cpp \
	door.cpp \
	factory.cpp \
	item.cpp \
	prop.cpp)
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
GAMESTATE_O = $(GAMESTATE_C:.cpp=.o) $(ENTITIES_C:.cpp=.o)
GAME_SOURCES = $(MAIN_C) $(LIBDRAW_C) $(LIBDRAW_SPLIT_C) $(AUX_DRAW_C) $(SCENES_C) $(AUDIO_MANAGER_C) $(MESSAGE_LOG_C) $(DAMAGE_POPUPS_C) $(GAMESTATE_C)

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
PRELOAD_FILES = --preload-file $(SRCDIR)/fonts --preload-file $(SRCDIR)/img --preload-file $(SRCDIR)/audio/music --preload-file $(SRCDIR)/audio/sfx --preload-file $(SRCDIR)/shaders
WEB_OPTIONS = -DPLATFORM_WEB -DWEB -DSPAWN_MONSTERS -DNPCS_ALL_AT_ONCE -DSTART_MESSAGES


# Phony targets
.PHONY: all clean docs docs-clean game tests tests_heavy tests_coverage

# Targets
all: game tests

# Desktop build
game: $(SRCDIR)/main.o $(SRCDIR)/libdraw.o $(LIBDRAW_SPLIT_C:.cpp=.o) $(AUX_DRAW_C:.cpp=.o) $(SCENES_C:.cpp=.o) $(SRCDIR)/audio_manager.o $(SRCDIR)/message_log.o $(SRCDIR)/damage_popups.o $(GAMESTATE_O)
	$(CXX) $(WFLAGS) $(CXXFLAGS) $(CFLAGS) $^ $(RAYLIB_LIBS) -o $@

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(WFLAGS) $(CXXFLAGS) $(CFLAGS) $(RAYLIB_CFLAGS) -I$(SRCDIR) -Iinclude -c $< -o $@

# Web build
index.html: $(GAME_SOURCES)
	$(WEB_CC) -o $@ $(GAME_SOURCES) $(WEB_INCLUDES) $(WEB_LINKS) $(EMCC_OPTIONS) $(SHELL_FILE) $(PRELOAD_FILES) $(WEB_OPTIONS) $(CFLAGS) $(CXXFLAGS)

# Unit tests
$(SRCDIR)/tests.cpp: $(SRCDIR)/unit_test.h $(wildcard $(SRCDIR)/test_suites/test_*.h)
	cxxtestgen --error-printer -o $@ $(SRCDIR)/unit_test.h $(SRCDIR)/test_suites/test_gamestate_lifecycle.h $(SRCDIR)/test_suites/test_combat_bootstrap.h $(SRCDIR)/test_suites/test_dungeon_bootstrap.h $(SRCDIR)/test_suites/test_entity_factory.h $(SRCDIR)/test_suites/test_entity_placement.h $(SRCDIR)/test_suites/test_inventory.h $(SRCDIR)/test_suites/test_pathfinder.h $(SRCDIR)/test_suites/test_renderer_seams.h $(SRCDIR)/test_suites/test_tile_cache.h $(SRCDIR)/test_suites/test_utility_helpers.h $(SRCDIR)/test_suites/test_utility_helpers_expanded.h $(SRCDIR)/test_suites/test_message_log.h $(SRCDIR)/test_suites/test_audio_manager.h $(SRCDIR)/test_suites/test_inputstate.h $(SRCDIR)/test_suites/test_world_interaction.h $(SRCDIR)/test_suites/test_damage_popups.h $(SRCDIR)/test_suites/test_game_tick.h $(SRCDIR)/test_suites/test_combat_math.h $(SRCDIR)/test_suites/test_visibility.h

CXXTEST_CFLAGS := $(shell pkg-config --cflags cxxtest 2>/dev/null || echo "-I/opt/homebrew/Cellar/cxxtest/4.4_3/include")

tests: $(SRCDIR)/tests.cpp $(SRCDIR)/audio_manager.o $(SRCDIR)/message_log.o $(SRCDIR)/damage_popups.o $(SRCDIR)/test_stubs.o $(GAMESTATE_O)
	$(CXX) $(WFLAGS) -o $@ $^ $(RAYLIB_CFLAGS) $(RAYLIB_LIBS) -I$(SRCDIR) -Iinclude $(CXXTEST_CFLAGS) $(CXXFLAGS) $(CFLAGS)

$(SRCDIR)/tests_heavy.cpp: $(SRCDIR)/unit_test_heavy.h $(SRCDIR)/unit_test.h $(wildcard $(SRCDIR)/test_suites/test_*.h)
	cxxtestgen --error-printer -o $@ $(SRCDIR)/unit_test_heavy.h $(SRCDIR)/unit_test.h $(SRCDIR)/test_suites/test_gamestate_lifecycle.h $(SRCDIR)/test_suites/test_combat_bootstrap.h $(SRCDIR)/test_suites/test_dungeon_bootstrap.h $(SRCDIR)/test_suites/test_entity_factory.h $(SRCDIR)/test_suites/test_entity_placement.h $(SRCDIR)/test_suites/test_heavy_simulation.h $(SRCDIR)/test_suites/test_inventory.h $(SRCDIR)/test_suites/test_pathfinder.h $(SRCDIR)/test_suites/test_renderer_seams.h $(SRCDIR)/test_suites/test_tile_cache.h $(SRCDIR)/test_suites/test_utility_helpers.h $(SRCDIR)/test_suites/test_utility_helpers_expanded.h $(SRCDIR)/test_suites/test_message_log.h $(SRCDIR)/test_suites/test_audio_manager.h $(SRCDIR)/test_suites/test_inputstate.h $(SRCDIR)/test_suites/test_world_interaction.h $(SRCDIR)/test_suites/test_damage_popups.h $(SRCDIR)/test_suites/test_game_tick.h $(SRCDIR)/test_suites/test_combat_math.h $(SRCDIR)/test_suites/test_visibility.h

tests_heavy: $(SRCDIR)/tests_heavy.cpp $(SRCDIR)/audio_manager.o $(SRCDIR)/message_log.o $(SRCDIR)/damage_popups.o $(SRCDIR)/test_stubs.o $(GAMESTATE_O)
	$(CXX) $(WFLAGS) -o $@ $^ $(RAYLIB_CFLAGS) $(RAYLIB_LIBS) -I$(SRCDIR) -Iinclude $(CXXTEST_CFLAGS) $(CXXFLAGS) $(CFLAGS)

# Coverage
COVERAGE_DIR = coverage_report
tests_coverage: $(SRCDIR)/tests.cpp
	@echo "Building tests with coverage instrumentation..."
	$(MAKE) clean
	CXXFLAGS="-DDEBUG=1 -DDEBUG_ASSERT=1 -DNPCS_ALL_AT_ONCE -DMUSIC_OFF --coverage -fprofile-arcs -ftest-coverage" $(MAKE) tests
	@echo "Running tests..."
	-./tests
	@echo "Generating coverage report..."
	@mkdir -p $(COVERAGE_DIR)
	lcov --capture --directory $(SRCDIR) --output-file $(COVERAGE_DIR)/coverage.info --ignore-errors source,inconsistent --rc derive_function_end_line=0 2>/dev/null || echo "lcov not found; install via: brew install lcov"
	lcov --remove $(COVERAGE_DIR)/coverage.info '/opt/*' '/usr/*' '*/include/entt/*' '*/test_suites/*' --output-file $(COVERAGE_DIR)/coverage_filtered.info --rc derive_function_end_line=0 2>/dev/null || true
	genhtml $(COVERAGE_DIR)/coverage_filtered.info --output-directory $(COVERAGE_DIR)/html 2>/dev/null || echo "genhtml not found; install via: brew install lcov"
	@echo "Coverage report: $(COVERAGE_DIR)/html/index.html"

# Documentation
docs:
	doxygen $(SRCDIR)/Doxyfile

docs-clean:
	rm -rf docs/api

clean:
	rm -rfv game tests tests_heavy index.html index.js index.wasm index.data $(SRCDIR)/*.o $(SRCDIR)/entities/*.o $(SRCDIR)/*.gch $(SRCDIR)/tests.cpp $(SRCDIR)/tests_heavy.cpp $(SRCDIR)/*.gcda $(SRCDIR)/*.gcno $(SRCDIR)/entities/*.gcda $(SRCDIR)/entities/*.gcno coverage_report
