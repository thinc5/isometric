#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../data.h"
#include "../ui.h"

#include "button.h"

static void relocate_button(void *data)
{
}

static bool handle_button_event(void *data, SDL_Event event, UI_Element *element)
{
    static bool click;
    ButtonState *button = &element->state.button;
    SDL_Rect pos = {element->screen_position.x, element->screen_position.y,
                    button->w, button->h};
    SDL_Point mouse_position = {0};
    SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
    if (click)
    {
        element->screen_position =
            (SDL_Point){mouse_position.x - (button->w / 2),
                        mouse_position.y - (button->h / 2)};
    }
    if (!SDL_PointInRect(&mouse_position, &pos))
        return false;
    // Only care about mouse events.
    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        click = true;
        break;
    case SDL_MOUSEBUTTONUP:
        click = false;
        break;
    default:
        return false;
    }
    return true;
}

static void render_button(SDL_Renderer *renderer, UI_Element *element)
{
    ButtonState *button = &element->state.button;
    SDL_SetRenderDrawColor(renderer, button->colour.r, button->colour.g,
                           button->colour.b, button->colour.a);
    SDL_Rect pos = {element->screen_position.x, element->screen_position.y,
                    button->w, button->h};
    SDL_RenderFillRect(renderer, &pos);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

UI_Element create_button(SDL_Point position, int argc, unsigned int width,
                         unsigned int height, SDL_Color colour,
                         void (*on_click)(void *data))
{
    UI_Element e = {0};
    e.type = BUTTON;
    e.screen_position = position;
    e.capabilities = UI_RENDERING | UI_EVENT_HANDLING;
    e.handle_event = &handle_button_event;
    e.render = &render_button;
    ButtonState *button = &e.state.button;
    button->colour = colour;
    button->w = width;
    button->h = height;
    button->on_click = on_click;
    button->on_click = &relocate_button;
    return e;
}
