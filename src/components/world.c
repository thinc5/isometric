#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../data.h"
#include "../ui.h"
#include "../rendering.h"
#include "../serialization.h"

#include "../config.h"

int get_tile_index(int width, int x, int y) {
    return (width * y) + x;
}

/**
 * Render the world.
 */
static void render_world(SDL_Renderer *renderer, UI_Element *element)
{
    WorldState *state = (WorldState *) &element->state.world;
    Data *data = (Data *) state->data;
    Camera *camera = &data->camera;
    World *world = &data->world;
    SDL_Rect logical = {
        .x = camera->x_offset,
        .y = camera->y_offset,
        .w = state->w,
        .h = state->h,
    };
    SDL_RenderSetViewport(data->renderer, &logical);
    uint32_t time = get_time(&data->timer);

    // Get the x and y cull range.
    // int x_quart = (logical.w / get_tile_width(Data)) / 2;
    // int y_quart = (logical.h / get_tile_height(Data)) / 2;
    // int x_min = data->camera_x + data->camera_y - x_quart;
    // x_min = x_min > 0 ? x_min : 0;
    // int y_min = data->camera_x - data->camera_y - y_quart;
    // y_min = y_min > 0 ? y_min : 0;
    // int x_max = x_min + (logical.w / get_tile_width(Data)) + x_quart;
    // int y_max = y_min + (logical.h / get_tile_height(Data)) + y_quart;
    // x_max = x_max < data->width ? x_max : data->width;
    // y_max = y_max < data->height ? y_max : data->height;

    // Draw the tiles, accents, actors and cursor.
    for (int i = 0; i < world->width * world->width; i++)
    {
        int x = i % world->width;
        int y = i / world->width;
        SDL_Rect loc = iso_to_screen(camera, x, y);
        SDL_Rect object_loc = { .x = loc.x, .y = loc.y - get_tile_height(camera), .w = loc.w, .h = loc.h };

        // Get the possible things to render.
        TILE_TYPE tile = world->board[i];
        Accent *accent = &world->accents[i];
        Actor *actor = &world->actors[i];

        if (tile != NO_TILE && tiles[tile].animation != NO_ANIMATION) {
            Animation *animation = &data->animations[tiles[tile].animation];
            int step = animation->frames == 1
                ? 0
                : (time % (animation->step * animation->frames)) / animation->step;
            SDL_RenderCopy(renderer, data->tiles, &animation->textures[step], &loc);
        }

        if (accent->type != NO_ACCENT && accent->animation != NO_ANIMATION) {
            Animation *animation = &data->animations[accent->animation];
            int step = (time % (animation->step * animation->frames)) / animation->step;
            SDL_RenderCopy(renderer, data->tiles, &animation->textures[step], &object_loc);
        }

        if (actor->type != NO_ACTOR && actor->animation != NO_ANIMATION) {
            Animation *animation = &data->animations[actor->animation];
            int step = (time % (animation->step * animation->frames)) / animation->step;
            SDL_RenderCopy(renderer, data->tiles, &animation->textures[step], &object_loc);
        }
    }

    // Draw cursor
    {
        SDL_Rect transf = iso_to_screen(camera, world->cursor_x, world->cursor_y);
        transf.h = get_tile_height(camera);
        SDL_Rect crsr_sub = {
            .x = ((time % 2000) / 1000) * TILE_WIDTH,
            .y = TILE_HEIGHT * (UTIL_ROW + 1),
            .w = TILE_WIDTH,
            .h = TILE_HEIGHT,
        };
        SDL_RenderCopy(renderer, data->tiles, &crsr_sub, &transf);
    }

    // Draw selected tile
    {
        SDL_Rect transf = iso_to_screen(camera, world->stored_point.x, world->stored_point.y);
        transf.h = get_tile_height(camera);
        SDL_Rect crsr_sub = {
            .x = ((time % 2000) / 1000) * TILE_WIDTH,
            .y = TILE_HEIGHT * (UTIL_ROW + 1),
            .w = TILE_WIDTH,
            .h = TILE_HEIGHT,
        };
        SDL_RenderCopy(renderer, data->tiles, &crsr_sub, &transf);
    }

    SDL_Rect orig = {.x = 0, .y = 0, .w = WINDOW_WIDTH, .h = WINDOW_HEIGHT};
    SDL_RenderSetViewport(data->renderer, &orig);
}

static void left_mouse_down(Data *data)
{
    static int x, y;
    SDL_GetMouseState(&x, &y);
    SDL_Point clicked = iso_from_screen(&data->camera, &data->world,
                                        data->surface, x, y);
    if (clicked.x == -1)
        return;
    int pos = clicked.x + (clicked.y * data->world.width);
    // Do we have a task to perform?
    if (data->world.click_handler != NULL)
    {
        // Was the click accepted by the handler?
        if (data->world.click_handler((void *)data, data->world.stored_point, clicked)) {
            // Click was accepted, clear the handler state and the selected point.
            data->world.click_handler = NULL;
        }
    }
    data->world.stored_point = clicked;
    // Are we clicking on an Actor with an on_click function?
    if (data->world.actors[pos].type == NO_ACTOR ||
        data->world.actors[pos].on_click == NULL)
        return;
    // Overrite the on click function.
    data->world.click_handler = data->world.actors[pos].on_click;
    // printf("Click stored: %p\n", (void*) data->world.actors[pos].on_click);
}

static void mouse_hover(Data *data)
{
    static int x, y;
    SDL_GetMouseState(&x, &y);
    SDL_Point clicked = iso_from_screen(&data->camera, &data->world,
                                        data->surface, x, y);
    if (clicked.x < 0 || clicked.x >= data->world.width ||
        clicked.y < 0 || clicked.y >= data->world.height)
        return;
    data->world.cursor_x = clicked.x;
    data->world.cursor_y = clicked.y;
}

static bool handle_world_event(void *d, SDL_Event event, UI_Element *element) {
    Data *data = (Data *)d;
    int hover_index = get_tile_index(data->world.width, data->world.cursor_x, data->world.cursor_y);
    int selected_index = get_tile_index(data->world.width, data->world.cursor_x, data->world.cursor_y);
    
    switch (event.type)
    {
    case SDL_KEYDOWN:
        switch (event.key.keysym.scancode)
        {
        // Toggle debug
        case SDL_SCANCODE_ESCAPE:
            data->debug = !data->debug;
            break;
        // Pause
        case SDL_SCANCODE_P:
            pause_game(&data->timer);
            break;
        // Camera
        case SDL_SCANCODE_A:
            //if (data->camera_x - 1 >= -(data->width * 0.15))
            data->camera.x--;
            break;
        case SDL_SCANCODE_D:
            //if (data->camera_x + 1 < (data->width * 0.55))
            data->camera.x++;
            break;
        case SDL_SCANCODE_W:
            //if (data->camera_y - 1 >= -(data->height * 0.15))
            data->camera.y--;
            break;
        case SDL_SCANCODE_S:
            //if (data->camera_y + 1 < (data->height * 0.55))
            data->camera.y++;
            break;
        // Cursor
        case SDL_SCANCODE_LEFT:
            if (data->world.cursor_x + 1 < data->world.width)
                data->world.cursor_x++;
            if (data->world.cursor_y - 1 > -1)
                data->world.cursor_y--;
            break;
        case SDL_SCANCODE_RIGHT:
            if (data->world.cursor_x - 1 > -1)
                data->world.cursor_x--;
            if (data->world.cursor_y + 1 < data->world.height)
                data->world.cursor_y++;
            break;
        case SDL_SCANCODE_UP:
            if (data->world.cursor_x - 1 > -1)
                data->world.cursor_x--;
            if (data->world.cursor_y - 1 > -1)
                data->world.cursor_y--;
            break;
        case SDL_SCANCODE_DOWN:
            if (data->world.cursor_x + 1 < data->world.width)
                data->world.cursor_x++;
            if (data->world.cursor_y + 1 < data->world.height)
                data->world.cursor_y++;
            break;
        // Zoom
        case SDL_SCANCODE_EQUALS:
            //if (data->zoom + ZOOM_STEP <= ZOOM_MAX)
            data->camera.zoom += ZOOM_STEP;
            break;
        case SDL_SCANCODE_MINUS:
            //if (data->zoom - ZOOM_STEP > ZOOM_MIN)
            data->camera.zoom -= ZOOM_STEP;
            break;
        // Change tile type
        case SDL_SCANCODE_Q:
            data->world.board[hover_index] =
                (data->world.board[hover_index] + 1) % NUM_TILES;
            break;
        // Change actor type
        case SDL_SCANCODE_R:
            data->world.actors[hover_index] =
                create_actor[(data->world.actors[hover_index].type + 1) % NUM_ACTORS]((ActorPosition) { .x = data->world.cursor_x, .y = data->world.cursor_y }, 0);
            break;
        // Change accent type
        case SDL_SCANCODE_F:
            data->world.accents[hover_index] =
                create_accent[(data->world.accents[hover_index].type + 1) % NUM_ACCENTS]();
            break;
        case SDL_SCANCODE_F1:
        case SDL_SCANCODE_K:
            serialize_map(&data->world, "save.txt");
            break;
        case SDL_SCANCODE_F2:
        case SDL_SCANCODE_L:
            free_world(&data->world);
            deserialize_map(&data->world, "save.txt");
            break;
        default:
            return false;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        left_mouse_down(data);
        break;
    case SDL_MOUSEMOTION:
        mouse_hover(data);
        break;
    default:
        return false;
    }
    return true;
}

UI_Element create_world(SDL_Point position, int argc, Data* data, unsigned int width,
                         unsigned int height)
{
    UI_Element e = {0};
    e.type = WORLD;
    e.screen_position = position;
    e.capabilities = UI_RENDERING | UI_EVENT_HANDLING;
    e.render = &render_world;
    e.handle_event = &handle_world_event;
    e.state.world = (WorldState) {
        .data = data,
        .w = width,
        .h = height,
    };
    return e;
}
