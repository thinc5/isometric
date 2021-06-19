#include "../animation.h"
#include "tile.h"

Tile tiles[NUM_TILES] = {
    [NO_TILE] = {
        .type = NO_TILE,
        .animation = NO_ANIMATION,
    },
    [SAND_TILE] = {
        .type = SAND_TILE,
        .animation = ANIM_SAND,
    },
    [WATER_TILE] = {
        .type = WATER_TILE,
        .animation = ANIM_WATER,
    },
    [GRASS_TILE] = {
        .type = GRASS_TILE,
        .animation = ANIM_GRASS,
    },
    [DIRT_TILE] = {
        .type = DIRT_TILE,
        .animation = DIRT_TILE,
    },
};
