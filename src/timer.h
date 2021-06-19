#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Timer
{
    bool paused;
    uint32_t start_time;
    uint32_t pause_time;
    uint32_t pause_start;
} Timer;

uint32_t get_time(Timer *timer);

void pause_game(Timer *timer);

void unpause_game(Timer *timer);

#endif
