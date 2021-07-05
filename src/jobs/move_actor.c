#include <stdio.h>

#include <SDL2/SDL.h>

#include "../config.h"
#include "../data.h"
#include "../rendering.h"
#include "../animation.h"
#include "../components/world.h"

#include "move_actor.h"

/**
 * Determine next tile.
 */
static void path_find(MoveActorJob *job)
{
    // Calculate direction.
    DIRECTION x_dir = EAST;
    DIRECTION y_dir = NORTH;
    int x_diff = 0;
    int y_diff = 0;
    if (job->current.x < job->destination.x)
    {
        x_dir = EAST;
        x_diff = job->destination.x - job->current.x;
    }
    else if (job->current.x > job->destination.x)
    {
        x_dir = WEST;
        x_diff = job->current.x - job->destination.x;
    }
    if (job->current.y < job->destination.y)
    {
        y_dir = NORTH;
        y_diff = job->destination.y - job->current.y;
    }
    else if (job->current.y > job->destination.y)
    {
        y_dir = SOUTH;
        y_diff = job->current.y - job->destination.y;
    }
    if (x_diff < y_diff)
    {
        job->direction = y_dir;
        if (y_dir == NORTH)
        {
            job->next.y = job->current.y + 1;
        }
        else
        {
            job->next.y = job->current.y - 1;
        }
    }
    else
    {
        job->direction = x_dir;
        if (x_dir == EAST)
        {
            job->next.x = job->current.x + 1;
        }
        else
        {
            job->next.x = job->current.x - 1;
        }
    }
}

/**
 * Process one frame of job progression.
 */
static bool move_frame(Data *data)
{
    uint32_t time = get_time(&data->timer);
    World *world = &data->world;
    MoveActorJob *job = (MoveActorJob *)data->current_job.data;
    // Have we reached our destination?
    if (job->current.x == job->destination.x &&
        job->current.y == job->destination.y)
    {
        // Write final location of actor.
        world->actors[get_tile_index(world->width, job->destination.x, job->destination.y)] = *job->moving;
        // Clear old location.
        *job->moving = (Actor) {0};
        free(job);
        data->current_job.data = NULL;
        data->current_job.process = NULL;
        data->current_job.render = NULL;
        return true;
    }
    // Change the actor position.
    Actor *actor = job->moving;
    uint32_t diff = (time - job->last_move) % job->move_rate;
    unsigned int height = get_tile_height(&data->camera);
    unsigned int width = get_tile_width(&data->camera);
    float perc = (float)diff / (float)job->move_rate;
    float x_offset = 0;
    float y_offset = 0;
    // Process a movement in a direction.
    switch (job->direction)
    {
    case NORTH:
        x_offset = -perc * (width / 2);
        y_offset = perc * (height / 2);
        break;
    case SOUTH:
        x_offset = perc * (width / 2);
        y_offset = -perc * (height / 2);
        break;
    case EAST:
        x_offset = perc * (width / 2);
        y_offset = perc * (height / 2);
        break;
    case WEST:
        x_offset = -perc * (width / 2);
        y_offset = -perc * (height / 2);
        break;
    default:
        break;
    }
    actor->position.x = job->current.x + (x_offset / 100);
    actor->position.y = job->current.y + (y_offset / 100);

    // Can this job be processed again or do we need to wait longer?
    if (job->last_move + job->move_rate > time)
        return false;

    // Set the last move time.
    job->last_move = time;
    // Set the next move target.
    SDL_Point previous = job->current;
    job->current = job->next;
    // Update the actor position.
    actor->position = (ActorPosition) { .x = job->current.x, .y = job->current.y };
    // Get the next move target.
    path_find(job);
    // Update the actual actor position on the cartesian plane
    // Write new location of the actor.
    world->actors[get_tile_index(world->width, job->next.x, job->next.y)] = *job->moving;
    // Clear old location.
    *job->moving = (Actor) {0};
    // Get reference to new location.
    job->moving = &world->actors[get_tile_index(world->width, job->next.x, job->next.y)];
    return false;
}

bool move_actor_job(void *ptr, SDL_Point from, SDL_Point to)
{
    Data *data = ptr;
    World *world = &data->world;
    // This actor cannot move to water tiles.
    TILE_TYPE tile = world->board[to.x + (to.y * world->width)];
    if (tile == NO_TILE || tile == WATER_TILE)
        return false;
    // Cannot move to the same location.
    if (from.x == to.x && from.y == to.y)
        return false;
    Actor *actor = &world->actors[from.x + (from.y * world->width)];
    // Queue the move job.
    MoveActorJob *job_data = (MoveActorJob *)malloc(sizeof(MoveActorJob));
    job_data->moving = actor;
    job_data->destination = to;
    job_data->start = from;
    job_data->current = from;
    job_data->next = job_data->current;
    path_find(job_data);
    job_data->move_rate = 1000;
    job_data->last_move = get_time(&data->timer);
    // Create the job and push to the queue.
    Job move_job = {0};
    move_job.process = (bool (*)(void *)) & move_frame;
    move_job.data = job_data;
    push_queue(&data->job_queue, (Queueable){.job = move_job});
    return true;
}
