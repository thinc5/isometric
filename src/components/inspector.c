#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../data.h"
#include "../ui.h"
#include "../rendering.h"
#include "../serialization.h"

#include "../config.h"

static void render_inspector(SDL_Renderer *renderer, UI_Element *element)
{
    WorldState *state = (WorldState *) &element->state.world;
    Data *data = (Data *) state->data;
    SDL_Rect logical = {
        .x = element->screen_position.x,
        .y = element->screen_position.y,
        .w = state->w,
        .h = state->h,
    };
    SDL_SetRenderDrawColor(data->renderer, 125, 125, 125, 125);
    SDL_RenderFillRect(data->renderer, &logical);
    // uint32_t time = get_time(&data->timer);

    // Selected tile
    TILE_TYPE tile = data->world.board[(data->world.cursor_y * data->world.height) + data->world.cursor_x];
    char tile_type[] = "Tile Type:   ";
    snprintf(tile_type + 11, 2, "%u", tile);
    draw_font(data->renderer, data->font, 0, 0, tile_type);
    // Selected accent
    Accent *accent = &data->world.accents[(data->world.cursor_y * data->world.height) + data->world.cursor_x];
    char accent_type[] = "Accent Type:   ";
    snprintf(accent_type + 12, 2, "%u", accent->type);
    draw_font(data->renderer, data->font, 0, 30, accent_type);
    // Selected actor.
    Actor *actor = &data->world.actors[(data->world.cursor_y * data->world.height) + data->world.cursor_x];
    char actor_type[] = "Accent Type:   ";
    snprintf(actor_type + 12, 2, "%u", actor->type);
    draw_font(data->renderer, data->font, 0, 60, actor_type);
    SDL_SetRenderDrawColor(data->renderer, 255, 255, 255, 255);
}

UI_Element create_inspector(SDL_Point position, int argc, Data* data, unsigned int width,
                         unsigned int height)
{
    UI_Element e = {0};
    e.type = INSPECTOR;
    e.screen_position = position;
    e.capabilities = UI_RENDERING;
    e.handle_event = NULL;
    e.render = &render_inspector;
    e.state.inspector = (InspectorState) {
        .data = data,
        .w = width,
        .h = height,
    };
    return e;
}
