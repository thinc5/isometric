#ifndef JOB_MOVE_ACTOR
#define JOB_MOVE_ACTOR

#include <SDL2/SDL.h>

#include "../actors/actor.h"

typedef enum DIRECTION
{
    NORTH,
    SOUTH,
    EAST,
    WEST,
    CARDINAL_DIRECTIONS
} DIRECTION;

typedef struct MoveActorJob
{
    Actor *moving;
    uint32_t last_move;
    uint32_t move_rate;
    SDL_Point start;
    SDL_Point current;
    SDL_Point next;
    SDL_Point destination;
    DIRECTION direction;
} MoveActorJob;

bool move_actor_job(void *ptr, SDL_Point from, SDL_Point to);

#endif
