#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>

#include <SDL2/SDL.h>

#define MAX_STEPS 20

typedef enum ANIMATION_NAME
{
    NO_ANIMATION,
    ANIM_SAND,
    ANIM_WATER,
    ANIM_GRASS,
    ANIM_DIRT,
    ANIM_MINION_IDLE,
    ANIM_SHRUB,
    NUM_ANIMATIONS,
} ANIMATION_NAME;

typedef struct Animation
{
    uint8_t frames;
    uint32_t step;
    SDL_Rect *textures;
} Animation;

void init_animations(Animation animations[NUM_ANIMATIONS]);

void free_animations(Animation animations[NUM_ANIMATIONS]);

#endif
