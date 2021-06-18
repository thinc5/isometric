#include <SDL2/SDL.h>

#include "timer.h"

uint32_t get_time(Timer *timer)
{
    if (timer->paused)
        return timer->pause_start - timer->start_time - timer->pause_time;
    return SDL_GetTicks() - timer->start_time - timer->pause_time;
}

void pause_game(Timer *timer)
{
    timer->pause_start = SDL_GetTicks();
    timer->paused = true;
}

void unpause_game(Timer *timer)
{
    timer->pause_time += SDL_GetTicks() - timer->pause_start;
    timer->paused = false;
}
