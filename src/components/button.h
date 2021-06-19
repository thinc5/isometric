#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>

#include "../data.h"
#include "../ui.h"

UI_Element create_button(SDL_Point position, int argc, unsigned int width,
                         unsigned int height, SDL_Color colour,
                         void (*on_click)(void *data));

#endif
