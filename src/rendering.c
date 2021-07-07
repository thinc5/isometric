#include <stdint.h>

#include <SDL2/SDL.h>

#include "config.h"
#include "data.h"

#include "rendering.h"

unsigned int get_tile_width(Camera *camera)
{
    return TILE_WIDTH * camera->zoom;
}

unsigned int get_tile_height(Camera *camera)
{
    return TILE_HEIGHT * camera->zoom;
}

SDL_Rect iso_to_screen(Camera *camera, int x, int y)
{
    return (SDL_Rect){
        .x = camera->x_offset + ((x - y) -
                                 (camera->x - camera->y)) *
                                    (get_tile_width(camera) / 2),
        .y = camera->y_offset + ((x + y) -
                                 (camera->x + camera->y)) *
                                    (get_tile_height(camera) / 2),
        .w = get_tile_width(camera),
        .h = get_tile_width(camera),
    };
}

SDL_Rect iso_fto_screen(Camera *camera, float x, float y)
{
    return (SDL_Rect){
        .x = (int)(camera->x_offset + ((x - y) -
                                       (camera->x - camera->y)) *
                                          (get_tile_width(camera) / 2)),
        .y = (int)(camera->y_offset + ((x + y) -
                                       (camera->x + camera->y)) *
                                          (get_tile_height(camera) / 2)),
        .w = get_tile_width(camera),
        .h = get_tile_width(camera),
    };
}

SDL_Point iso_from_screen_point(Camera *camera, SDL_Surface *surface,
                          int x, int y)
{
    // Get generic square position on logical canvas (viewport).
    int base_x = (x - camera->x_offset) / (get_tile_width(camera));
    int base_y = (y - camera->x_offset) / (get_tile_height(camera));
    int real_x = (base_y + camera->y) +
                 (base_x + camera->x) - camera->y;
    int real_y = (base_y + camera->y) -
                 (base_x + camera->x) + camera->x;
    // Convert into percentage offset and into original sprite format
    int x_offset = (x % get_tile_width(camera)) / camera->zoom;
    int y_offset = (y % get_tile_height(camera)) / camera->zoom;
    uint32_t pixel = *((uint32_t *)surface->pixels +
                       y_offset * surface->w + x_offset);
    // Use sprite to determine corners.
    SDL_Color sample = {0};
    SDL_GetRGBA(pixel, surface->format, &sample.r, &sample.g, &sample.b,
                &sample.a);
    if (sample.r && sample.g && !sample.b && sample.a)
        real_x++;
    if (sample.r && !sample.g && !sample.b && sample.a)
        real_x--;
    if (!sample.r && sample.g && !sample.b && sample.a)
        real_y++;
    if (!sample.r && !sample.g && sample.b && sample.a)
        real_y--;
    return (SDL_Point){
        .x = real_x,
        .y = real_y,
    };
}

void draw_font(SDL_Renderer *renderer, TTF_Font *font, int x, int y,
                      const char *str)
{
    static SDL_Color c = {0, 0, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, str, c);
    SDL_Rect pos = {
        .x = x,
        .y = y,
        .w = surface->w,
        .h = surface->h,
    };
    SDL_RenderFillRect(renderer, &pos);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &pos);
    SDL_DestroyTexture(texture);
}
