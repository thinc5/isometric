#include <string.h>
#include <stdint.h>

#include <stdio.h>

#include <SDL2/SDL.h>

#include "config.h"

#include "animation.h"

void init_animations(Animation animations[NUM_ANIMATIONS])
{
    animations[ANIM_SAND] = (Animation) {
        .frames = 1,
        .step = 0,
        .textures = {(SDL_Rect){.x = TILE_WIDTH * 1, .y = TILE_ROW * TILE_WIDTH, .w = TILE_WIDTH, .h = TILE_WIDTH}, { 0 }}
    };
    animations[ANIM_WATER] = (Animation) {
        .frames = 1,
        .step = 0,
        .textures = {(SDL_Rect){
                         .x = TILE_WIDTH * 2,
                         .y = TILE_ROW * TILE_WIDTH,
                         .w = TILE_WIDTH,
                         .h = TILE_WIDTH},
                     { 0 }
        }
    };
    animations[ANIM_GRASS] = (Animation) {
        .frames = 1,
        .step = 0,
        .textures = { (SDL_Rect){
                         .x = TILE_WIDTH * 3,
                         .y = TILE_ROW * TILE_WIDTH,
                         .w = TILE_WIDTH,
                         .h = TILE_WIDTH,
                     },
                     {0}
        }
    };

    animations[ANIM_DIRT] = (Animation){
        .frames = 1,
        .step = 0,
        .textures = {(SDL_Rect){
        .x = TILE_WIDTH * 4,
        .y = TILE_ROW * TILE_WIDTH,
        .w = TILE_WIDTH,
        .h = TILE_WIDTH,
    }, {0}}
    };
    animations[ANIM_MINION_IDLE] = (Animation){
        .frames = 3,
        .step = 500,
        .textures = {
            (SDL_Rect){TILE_WIDTH * 0, (ACTOR_ROW + 1) * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH},
            (SDL_Rect){TILE_WIDTH * 1, (ACTOR_ROW + 1) * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH},
            (SDL_Rect){TILE_WIDTH * 2, (ACTOR_ROW + 1) * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH}, {0}},
    };
    animations[ANIM_SHRUB] = (Animation){
        .frames = 4,
        .step = 750,
        .textures = {
            (SDL_Rect){TILE_WIDTH * 0, ACTOR_ROW * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH},
            (SDL_Rect){TILE_WIDTH * 1, ACTOR_ROW * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH},
            (SDL_Rect){TILE_WIDTH * 2, ACTOR_ROW * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH},
            (SDL_Rect){TILE_WIDTH * 3, ACTOR_ROW * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH},
            {0}
        }
    };
}

