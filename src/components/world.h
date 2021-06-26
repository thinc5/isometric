#ifndef WORLD_H
#define WORLD_H

#include <SDL2/SDL.h>

#include "../data.h"
#include "../ui.h"

int get_tile_index(int width, int x, int y);

UI_Element create_world(SDL_Point position, int argc, Data* data, unsigned int width,
                         unsigned int height);

#endif
