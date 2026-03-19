#include "actor_definitions.h"
#include "tx_keys_npcs.h"
#include "tx_keys_monsters.h"

static const ActorDefinition actor_definitions[] = {
    {"human", RACE_HUMAN, "Human", "A typical human.", ALIGNMENT_NEUTRAL_NEUTRAL, 8, {0, 0, 0, 0, 0, 0}, TX_HUMAN_KEYS, TX_HUMAN_COUNT},
    {"elf", RACE_ELF, "Elf", "A graceful elf.", ALIGNMENT_GOOD_CHAOTIC, 6, {0, 2, 0, 0, -2, 0}, TX_ELF_KEYS, TX_ELF_COUNT},
    {"dwarf", RACE_DWARF, "Dwarf", "A sturdy dwarf.", ALIGNMENT_GOOD_LAWFUL, 10, {0, 0, 0, 0, 2, -2}, TX_DWARF_KEYS, TX_DWARF_COUNT},
    {"halfling", RACE_HALFLING, "Halfling", "A small halfling.", ALIGNMENT_GOOD_NEUTRAL, 6, {0, 0, 0, 0, 0, 0}, TX_HALFLING_KEYS, TX_HALFLING_COUNT},
    {"goblin", RACE_GOBLIN, "Goblin", "A sneaky goblin.", ALIGNMENT_EVIL_NEUTRAL, 6, {0, 0, 0, 0, 0, 0}, TX_GOBLIN_KEYS, TX_GOBLIN_COUNT},
    {"orc", RACE_ORC, "Orc", "A fierce orc.", ALIGNMENT_EVIL_CHAOTIC, 8, {0, 0, 0, 0, 0, 0}, TX_ORC_KEYS, TX_ORC_COUNT},
    {"bat", RACE_BAT, "Bat", "A flying bat.", ALIGNMENT_NEUTRAL_NEUTRAL, 3, {0, 0, 0, 0, 0, 0}, TX_BAT_KEYS, TX_BAT_COUNT},
    {"green_slime", RACE_GREEN_SLIME, "Green Slime", "A gooey green slime.", ALIGNMENT_NEUTRAL_NEUTRAL, 4, {-2, -2, -2, -2, -2, -2}, TX_GREEN_SLIME_KEYS, TX_GREEN_SLIME_COUNT},
    {"wolf", RACE_WOLF, "Wolf", "A wild wolf.", ALIGNMENT_NEUTRAL_NEUTRAL, 6, {0, 0, 0, 0, 0, 0}, TX_WOLF_KEYS, TX_WOLF_COUNT},
    {"warg", RACE_WARG, "Warg", "A massive warg.", ALIGNMENT_EVIL_NEUTRAL, 12, {0, 0, 0, 0, 0, 0}, TX_WARG_KEYS, TX_WARG_COUNT},
    {"rat", RACE_RAT, "Rat", "A scurrying rat.", ALIGNMENT_NEUTRAL_NEUTRAL, 4, {0, 0, 0, 0, 0, 0}, TX_RAT_KEYS, TX_RAT_COUNT},
    {"skeleton", RACE_SKELETON, "Skeleton", "A reanimated skeleton.", ALIGNMENT_EVIL_LAWFUL, 8, {0, 0, 0, 0, 0, 0}, TX_SKELETON_KEYS, TX_SKELETON_COUNT},
    {"zombie", RACE_ZOMBIE, "Zombie", "A shambling zombie.", ALIGNMENT_EVIL_NEUTRAL, 8, {0, 0, 0, 0, 0, 0}, TX_ZOMBIE_KEYS, TX_ZOMBIE_COUNT}
};

const ActorDefinition* get_actor_definition(race_t race) {
    for (const auto& def : actor_definitions) {
        if (def.race == race) {
            return &def;
        }
    }
    return nullptr;
}
