#ifndef CONFIG_H
#define CONFIG_H

// DISPLAY SETTINGS
#define WINDOW_TITLE "ISOMETRIC GRID"
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1270

#define MAX_FPS (1000 / 144)
#define VOLUME_DEFAULT (MIX_MAX_VOLUME / 8)

// SPRITE SHEET DETAILS
#define TILE_WIDTH 64
#define TILE_HEIGHT 32

#define DEBUG_FONT_SIZE (WINDOW_WIDTH / 100)

#define UTIL_ROW 0
#define TILE_ROW 1
#define ACTOR_ROW 5

// GAMEPLAY CONSTANTS
#define ZOOM_STEP 0.02f
#define ZOOM_MAX 4.00f
#define ZOOM_MIN 1.0f

#define ACTOR_MOVE_RATE 500

// QUEUE
#define JOB_QUEUE_SIZE 10

// Macro to wrap colour definitions as an SDL_Colour struct initializer.
#define SDL_COLOUR(COLOUR) (SDL_Color) { COLOUR }

// Static colour definitions
#define DEFAULT_BACKGROUND_COLOUR 255, 255, 255, 255
#define DEBUG_FONT_COLOUR 125, 0, 255, 255

#endif
