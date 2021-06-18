#include <SDL2/SDL.h>

#include "ui.h"

#include "./components/world.h"
#include "./components/inspector.h"
#include "./components/button.h"

UI_Element (*create_ui_element[NUM_ELEMENT_TYPES])(SDL_Point, int, ...) = {
    [WORLD] = (UI_Element (*) (SDL_Point, int, ...)) &create_world,
    [INSPECTOR] = (UI_Element (*) (SDL_Point, int, ...)) &create_inspector,
    [BUTTON] = (UI_Element (*) (SDL_Point, int, ...)) &create_button,
};
