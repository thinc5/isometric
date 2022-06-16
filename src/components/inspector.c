#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "../data.h"
#include "../ui.h"
#include "../rendering.h"
#include "../serialization.h"
#include "../components/world.h"

#include "../config.h"

#define LINE_WIDTH 40
#define ROW_HEIGHT 30

static void draw_info_row(SDL_Renderer *renderer, TTF_Font *font, char* display_string, size_t render_row)
{
    draw_font(renderer, font, 0, render_row * ROW_HEIGHT, display_string, SDL_COLOUR(DEBUG_FONT_COLOUR));
    memset(display_string, '0', LINE_WIDTH);
}

static void write_camera_info(char *display_string, Camera *camera) {
    snprintf(display_string, 35, "Camera info: x: %03d, y: %03d z: %03.3f", camera->x % 1000, camera->y % 1000, camera->zoom);
}

static void write_current_time(char *display_string, float time) {
    snprintf(display_string, 22, "Time Elapsed: %7.3f", time);
}

static void write_tile_type(char *display_string, TILE_TYPE type)
{
    snprintf(display_string, 17, "Tile Type:  %02d", type);
}

static void write_accent_type(char *display_string, ACCENT_TYPE type)
{
    snprintf(display_string, 16, "Accent Type: %02d", type);
}

static void write_actor_type(char *display_string, ACTOR_TYPE type)
{
    snprintf(display_string, 15, "Actor Type: %02d", type);
}

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
    SDL_SetRenderDrawColor(renderer, 125, 125, 125, 125);
    SDL_RenderFillRect(renderer, &logical);
    size_t draw_row = 0;
    char line_buffer[LINE_WIDTH] = { 0 };

    // Camera info
    write_camera_info(line_buffer, &data->camera);
    draw_info_row(data->renderer, data->font, line_buffer, draw_row++);

    // Game time elapsed
    write_current_time(line_buffer, (float) get_time(&data->timer) / 1000.0);
    draw_info_row(data->renderer, data->font, line_buffer, draw_row++);

    // Game time elapsed
    write_current_time(line_buffer, (float) get_time(&data->timer) / 1000.0);
    draw_info_row(data->renderer, data->font, line_buffer, draw_row++);

    // Game time elapsed
    write_current_time(line_buffer, (float) get_time(&data->timer) / 1000.0);
    draw_info_row(data->renderer, data->font, line_buffer, draw_row++);

    // Hovered tile
    write_tile_type(line_buffer, (int) data->world.board[get_tile_index(data->world.width, data->world.cursor_x, data->world.cursor_y)]);
    draw_info_row(data->renderer, data->font, line_buffer, draw_row++);

    // Hovered accent
    write_accent_type(line_buffer, (int) data->world.accents[get_tile_index(data->world.width, data->world.cursor_x, data->world.cursor_y)].type);
    draw_info_row(data->renderer, data->font, line_buffer, draw_row++);

    // Hovered actor
    write_actor_type(line_buffer, (int) data->world.actors[get_tile_index(data->world.width, data->world.cursor_x, data->world.cursor_y)].type);
    draw_info_row(data->renderer, data->font, line_buffer, draw_row++);

    // Selected tile
    write_tile_type(line_buffer, (int) data->world.board[get_tile_index(data->world.width, data->world.stored_point.x, data->world.stored_point.y)]);
    draw_info_row(data->renderer, data->font, line_buffer, draw_row++);

    // Selected accent
    write_accent_type(line_buffer, (int) data->world.accents[get_tile_index(data->world.width, data->world.stored_point.x, data->world.stored_point.y)].type);
    draw_info_row(data->renderer, data->font, line_buffer, draw_row++);

    // Selected actor
    write_actor_type(line_buffer, (int) data->world.actors[get_tile_index(data->world.width, data->world.stored_point.x, data->world.stored_point.y)].type);
    draw_info_row(data->renderer, data->font, line_buffer, draw_row++);

    // On click status.
    char on_click_addr[] = "On click:                            ";
    if (data->world.click_handler == NULL) {
        snprintf(on_click_addr + 10, 5, "none");
    } else {
        snprintf(on_click_addr + 10, 7, "%p", (void*) &data->world.click_handler);
    }
    draw_font(data->renderer, data->font, 0, draw_row++ * ROW_HEIGHT, on_click_addr, SDL_COLOUR(DEBUG_FONT_COLOUR));

    // Reset the render draw colour.
    SDL_SetRenderDrawColor(data->renderer, DEFAULT_BACKGROUND_COLOUR);
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
