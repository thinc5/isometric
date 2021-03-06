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
    DIRECTION x_dir = NORTH;
    DIRECTION y_dir = WEST;
    int x_diff = 0;
    int y_diff = 0;
    // Which direction on the x axis?
    if (job->current.y < job->destination.y)
    {
        y_dir = SOUTH;
        y_diff = job->destination.y - job->current.y;
    }
    else
    {
        y_dir = NORTH;
        y_diff = job->current.y - job->destination.y;
    }
    if (job->current.x < job->destination.x)
    {
        x_dir = EAST;
        x_diff = job->destination.x - job->current.x;
    }
    else
    {
        x_dir = WEST;
        x_diff = job->current.x - job->destination.x;
    }
    if (x_diff < y_diff)
    {
        job->direction = y_dir;
        if (y_dir == NORTH)
        {
            job->next.y = job->current.y - 1;
        }
        else
        {
            job->next.y = job->current.y + 1;
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
    // How far through the current tile are we?
    float perc = (float)diff / (float)job->move_rate;
    // Process a movement in a direction.
    switch (job->direction)
    {
    case NORTH:
        actor->position.y = job->current.y - perc;
        break;
    case SOUTH:
        actor->position.y = job->current.y + perc;
        break;
    case EAST:
        actor->position.x = job->current.x + perc;
        break;
    case WEST:
        actor->position.x = job->current.x - perc;
        break;
    default:
        break;
    }

    // Can this job be processed again or do we need to wait longer?
    if (job->last_move + job->move_rate > time)
        return false;

    // Set the last move time.
    job->last_move = time;
    job->current = job->next;
    // Update the actor position.
    actor->position = (ActorPosition) { .x = job->current.x, .y = job->current.y };
    // Get the next move target.
    path_find(job);
    return false;
}

// Move actor visually, does not move the actor's position in the grid.
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
    job_data->move_rate = ACTOR_MOVE_RATE;
    job_data->last_move = get_time(&data->timer);
    // Create the job and push to the queue.
    Job move_job = {0};
    move_job.process = (bool (*)(void *)) & move_frame;
    move_job.data = job_data;
    push_queue(&data->job_queue, (Queueable){.job = move_job});
    return true;
}
