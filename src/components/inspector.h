#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <SDL2/SDL.h>

#include "../data.h"
#include "../ui.h"

UI_Element create_inspector(SDL_Point position, int argc, Data* data, unsigned int width,
                         unsigned int height);

#endif
