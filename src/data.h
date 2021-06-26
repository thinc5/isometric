#ifndef DATA_H
#define DATA_H

#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "timer.h"
#include "animation.h"
#include "job.h"
#include "queue.h"
#include "ui.h"

#include "./tiles/tile.h"
#include "./accents/accent.h"
#include "./actors/actor.h"

typedef enum GAME_STATUS
{
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    CLOSING,
} GAME_STATUS;

typedef struct World
{
    uint16_t width;
    uint16_t height;
    int logical_dim;
    TILE_TYPE *board;
    Accent *accents;
    Actor *actors;
    int cursor_x;
    int cursor_y;
    bool (*click_handler)(void *ptr, ...);
    SDL_Point stored_point;
} World;

typedef struct Camera
{
    int x_offset;
    int y_offset;
    int x;
    int y;
    float zoom;
} Camera;

typedef struct Data
{
    GAME_STATUS status;
    bool debug;
    // Renderer
    SDL_Window *window;
    SDL_Renderer *renderer;
    // Resources
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *tiles;
    Animation animations[NUM_ANIMATIONS];
    // Timing
    Timer timer;
    uint32_t last_frame;
    // UI Elements
    UI_Element user_interface[10];
    // Current map Data
    World world;
    Camera camera;
    // Job queue
    Queue job_queue;
    Job current_job;
} Data;

void init_data(Data *data);

void free_data(Data *data);

void init_world(World *world, uint16_t width, uint16_t height);

void free_world(World *world);

#endif
