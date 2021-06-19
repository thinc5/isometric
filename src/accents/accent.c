#include <SDL2/SDL.h>

#include "accent.h"

static Accent create_shrub(void)
{
    Accent shrub = {0};
    shrub.type = SHRUB;
    shrub.animation = ANIM_SHRUB;
    return shrub;
}

Accent (*create_accent[NUM_ACCENTS])(void) = {
    [SHRUB] = &create_shrub,
};
