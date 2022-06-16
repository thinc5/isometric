#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

// #include "data.h"

// Binary flags to indicate features that have been enabled.
typedef uint8_t UI_CAPABILITIES;
#define UI_RENDERING 0x01
#define UI_EVENT_HANDLING 0x02

typedef struct ButtonState
{
    SDL_Color colour;
    unsigned int h;
    unsigned int w;
    void (*on_click)(void *data);
} ButtonState;

typedef struct WorldState
{
    unsigned int h;
    unsigned int w;
    void *data;
} WorldState;

typedef struct InspectorState
{
    unsigned int h;
    unsigned int w;
    void *data;
} InspectorState;

// Registered UI element types.
typedef enum UI_ELEMENT_TYPE
{
    NO_UI_ELEMENT,
    WORLD,
    INSPECTOR,
    BUTTON,
    BUTTON_TEXT,
    NUM_ELEMENT_TYPES,
} UI_ELEMENT_TYPE;

// Corresponding element type state.
typedef union UI_ELEMENT_STATE
{
    WorldState world;
    ButtonState button;
    InspectorState inspector;
} UI_ELEMENT_STATE;

// Required data for UI Elements.
typedef struct UI_Element
{
    UI_ELEMENT_TYPE type;
    UI_ELEMENT_STATE state;
    UI_CAPABILITIES capabilities;
    SDL_Point screen_position;
    bool (*handle_event)(void *data, SDL_Event event,
                         struct UI_Element *element);
    void (*render)(SDL_Renderer *renderer, struct UI_Element *element);
} UI_Element;

extern UI_Element (*create_ui_element[NUM_ELEMENT_TYPES])(SDL_Point, int, ...);

#endif
