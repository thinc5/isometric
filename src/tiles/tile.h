#ifndef TILE_H
#define TILE_H

#include "../animation.h"

typedef enum TILE_TYPE
{
    NO_TILE,
    SAND_TILE,
    WATER_TILE,
    GRASS_TILE,
    DIRT_TILE,
    NUM_TILES,
} TILE_TYPE;

typedef struct Tile
{
    TILE_TYPE type;
    ANIMATION_NAME animation;
} Tile;

extern Tile tiles[NUM_TILES];

#endif
