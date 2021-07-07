#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>

#include "data.h"

unsigned int get_tile_width(Camera *camera);

unsigned int get_tile_height(Camera *camera);

SDL_Rect iso_to_screen(Camera *camera, int x, int y);

SDL_Rect iso_to_screen(Camera *camera, int x, int y);

SDL_Rect iso_fto_screen(Camera *camera, float x, float y);

SDL_Point iso_from_screen_point(Camera *camera, SDL_Surface *surface,
                          int x, int y);

void draw_font(SDL_Renderer *renderer, TTF_Font *font, int x, int y,
                      const char *str);

#endif
