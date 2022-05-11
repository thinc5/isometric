#include <SDL2/SDL.h>

#include "accent.h"

Accent (*create_accent[NUM_ACCENTS])(void) = {
    [SHRUB] = &create_shrub,
    [CASTLE] = &create_castle,
};
