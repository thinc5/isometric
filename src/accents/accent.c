#include <SDL2/SDL.h>

#include "accent.h"
#include "shrub.h"

Accent (*create_accent[NUM_ACCENTS])(void) = {
    [SHRUB] = &create_shrub,
};
