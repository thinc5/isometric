#ifndef ACCENT_H
#define ACCENT_H

#include <stdint.h>

#include <SDL2/SDL.h>

#include "../animation.h"

typedef enum ACCENT_TYPE
{
    NO_ACCENT,
    SHRUB,
    NUM_ACCENTS,
} ACCENT_TYPE;

typedef struct Accent
{
    ACCENT_TYPE type;
    ANIMATION_NAME animation;
} Accent;

extern Accent (*create_accent[NUM_ACCENTS])(void);

#endif
