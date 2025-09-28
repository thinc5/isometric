#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../data.h"
#include "../ui.h"
#include "../rendering.h"
#include "../serialization.h"

#include "../config.h"

static void render_cube(SDL_Renderer *renderer, UI_Element *element)
{
    // Data *data = (Data *) state->data;
    
    // SDL_RenderSetViewport(data->renderer, &logical);
    // uint32_t time = get_time(&data->timer);

    // SDL_Rect orig = {.x = 0, .y = 0, .w = WINDOW_WIDTH, .h = WINDOW_HEIGHT};
    // SDL_RenderSetViewport(data->renderer, &orig);
}

static void left_mouse_down(Data *data)
{
    static int x, y;
    SDL_GetMouseState(&x, &y);
}

static void mouse_hover(Data *data)
{
    static int x, y;
    SDL_GetMouseState(&x, &y);
}

static bool handle_cube_event(void *d, SDL_Event event, UI_Element *element) {
    
    switch (event.type)
    {
    case SDL_KEYDOWN:
        switch (event.key.keysym.scancode)
        {
        // Toggle debug
        case SDL_SCANCODE_ESCAPE:
            // data->debug = !data->debug;
            break;
        // Pause
        case SDL_SCANCODE_P:
            // if (data->timer.paused) {
            //     unpause_game(&data->timer);
            // } else {
            //     pause_game(&data->timer);
            // }
            break;
        default:
            return false;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        // left_mouse_down(data);
        break;
    case SDL_MOUSEMOTION:
        // mouse_hover(data);
        break;
    default:
        return false;
    }
    return true;
}

UI_Element create_cube(SDL_Point position, int argc, Data* data, unsigned int width,
                         unsigned int height)
{
    UI_Element e = {0};
    e.type = WORLD;
    e.screen_position = position;
    e.capabilities = UI_RENDERING | UI_EVENT_HANDLING;
    e.render = &render_cube;
    e.handle_event = &handle_cube_event;
    e.state.world = (WorldState) {
        .data = data,
        .w = width,
        .h = height,
    };
    return e;
}
