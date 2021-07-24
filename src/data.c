#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "config.h"

#include "animation.h"
#include "./tiles/tile.h"
#include "./accents/accent.h"
#include "./actors/actor.h"

#include "data.h"

void init_data(Data *data)
{
    // Load animation Data.
    init_animations(data->animations);
    data->job_queue = create_queue(JOB_QUEUE_SIZE, QUEUEABLE_JOB);
    // Initialize the game Data.
    data->debug = false;
    data->fullscreen = false;
    data->current_job.data = NULL;
    data->current_job.process = NULL;
    data->current_job.render = NULL;
    // Initialize the world.
    init_world(&data->world, 5, 5);
    // Initialize the camera.
    data->camera.x_offset = 0;
    data->camera.y_offset = 0;
    data->camera.zoom = 1.0f;
    data->camera.x = -(data->world.width);// (get_tile_width(&data->camera) / WINDOW_WIDTH) / 2;
    data->camera.y = 0;// (get_tile_height(&data->camera) / WINDOW_HEIGHT) / 2;
    // Initialize the game timer.
    uint32_t now = SDL_GetTicks();
    data->timer.start_time = now;
    data->timer.pause_time = 0;
    data->timer.pause_start = 0;
    data->last_frame = now;

    // Create the corresponding world ui component.
    data->user_interface[0] = create_ui_element[WORLD](
        (SDL_Point){0, 0}, 4, data, 1920, 1080, NULL
    );
    data->user_interface[1] = create_ui_element[INSPECTOR](
        (SDL_Point){0, 0}, 4, data, 140, 300, NULL
    );
    data->user_interface[2] = create_ui_element[BUTTON](
        (SDL_Point){720, 720}, 4, 100, 75, (SDL_Colour){255, 0, 255, 255},
        NULL);
}

void free_data(Data *data)
{
    free_animations(data->animations);
    free(data->job_queue.store);
}

void init_world(World *world, uint16_t width, uint16_t height)
{
    world->click_handler = NULL;
    world->stored_point = (SDL_Point){0};
    uint32_t area = width * height;
    world->board = (TILE_TYPE *)malloc(sizeof(TILE_TYPE) * area);
    world->accents = (Accent *)malloc(sizeof(Accent) * area);
    world->actors = (Actor *)malloc(sizeof(Actor) * area);
    for (uint32_t pos = 0; pos < area; pos++)
        world->board[pos] = GRASS_TILE;
    memset(world->accents, NO_ACCENT, area * sizeof(Accent));
    memset(world->actors, NO_ACTOR, area * sizeof(Actor));
    world->width = width;
    world->height = height;
    world->logical_dim = WINDOW_WIDTH / (width < height ? height : width);
    world->cursor_x = world->width / 2;
    world->cursor_y = world->height / 2;
}

void free_world(World *world)
{
    free(world->board);
    world->board = NULL;
    free(world->accents);
    world->accents = NULL;
    free(world->actors);
    world->actors = NULL;
}
