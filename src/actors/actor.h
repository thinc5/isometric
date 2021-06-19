#ifndef ACTOR_H
#define ACTOR_H

#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "../animation.h"

typedef enum ACTOR_TYPE
{
    NO_ACTOR,
    MINION,
    NUM_ACTORS,
} ACTOR_TYPE;

typedef struct ActorPosition {
    float x;
    float y;
} ActorPosition;

typedef struct Actor
{
    ACTOR_TYPE type;
    ANIMATION_NAME animation;
    ActorPosition position;
    bool (*on_click)(void *ptr, ...);
} Actor;

extern Actor (*create_actor[NUM_ACTORS])(ActorPosition, int, ...);

bool move_actor(void *ptr, SDL_Point from, SDL_Point to);

#endif
