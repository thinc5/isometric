#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "config.h"
#include "serialization.h"
#include "timer.h"
#include "data.h"
#include "rendering.h"

// ASSETS
#include "font.h"
#include "tiles.h"
#include "animation.h"

// ACTORS & ACCENTS
#include "tile.h"
#include "accent.h"
#include "./actors/actor.h"

// EVENT HANDLING
static void handle_events(Data *data)
{
    static SDL_Event event;
    static bool muted;
    while (SDL_PollEvent(&event))
    {
        // Check top level input first
        if (event.type == SDL_QUIT)
        {
            data->status = CLOSING;
            return;
        }
        // Check ui elements next
        for (int i = 0; i < 10; i++)
        {
            UI_Element *element = &data->user_interface[i];
            if (element->type == NO_UI_ELEMENT)
                break;
            if (element->capabilities & UI_EVENT_HANDLING)
                if (element->handle_event((bool *)data, event, element))
                    return;
        }
        // Rest of the events.
        switch (event.type)
        {
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_M)
            {
                if (muted)
                {
                    Mix_VolumeMusic(VOLUME_DEFAULT);
                    Mix_Volume(-1, VOLUME_DEFAULT * 1.5);
                }
                else
                {
                    Mix_VolumeMusic(0);
                    Mix_Volume(-1, 0);
                }
                muted = !muted;
            }
            if (data->status == PAUSED)
            {
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_P:
                case SDL_SCANCODE_ESCAPE:
                    unpause_game(&data->timer);
                    break;
                default:
                    break;
                }
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            break;
        case SDL_MOUSEMOTION:
            break;
        default:
            break;
        }
    }
}

// INITIALIZATION FUNCTIONS
static void init_rendering(Data *data)
{
    data->window =
        SDL_CreateWindow(WINDOW_TITLE,
                         SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                         WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_HIDDEN);
    data->renderer = SDL_CreateRenderer(data->window, -1,
                                        SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(data->renderer, 0, 0, 0, 255);
    SDL_RenderSetLogicalSize(data->renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_SetRenderDrawBlendMode(data->renderer, SDL_BLENDMODE_BLEND);
    data->font =
        TTF_OpenFontRW(SDL_RWFromConstMem(ssp_regular_otf, ssp_regular_otf_len),
                       1, WINDOW_WIDTH / 80);
    data->surface =
        IMG_Load_RW(
            SDL_RWFromConstMem(iso_demo_png, iso_demo_png_len),
            1);
    data->tiles = SDL_CreateTextureFromSurface(data->renderer, data->surface);
    SDL_SetRenderDrawColor(data->renderer, 255, 255, 255, 255);
}

static void free_rendering(Data *data)
{
    SDL_DestroyTexture(data->tiles);
    SDL_FreeSurface(data->surface);
    TTF_CloseFont(data->font);
    SDL_DestroyRenderer(data->renderer);
    SDL_DestroyWindow(data->window);
}

static void init_sound(Data *data)
{
    Mix_VolumeMusic(VOLUME_DEFAULT);
    Mix_Volume(-1, VOLUME_DEFAULT * 1.5);
    // data->theme = Mix_LoadMUS_RW(SDL_RWFromConstMem(theme_mp3, theme_mp3_len), -1);
    // data->place = Mix_LoadWAV_RW(SDL_RWFromConstMem(fall_wav, fall_wav_len), -1);
}

static void free_sound(Data *data)
{
    // Mix_FreeMusic(data->theme);
    // Mix_FreeChunk(data->place);
}

static void init_game(Data *data)
{
    init_rendering(data);
    init_sound(data);
    data->status = PLAYING;
    init_data(data);
}

static void free_game(Data *data)
{
    free_sound(data);
    free_rendering(data);
    free_world(&data->world);
    free_data(data);
}

static void game_over(Data *data)
{
    Mix_HaltMusic();
    data->status = CLOSING;
}

static bool init_modules(void)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        return false;

    if (TTF_Init() != 0)
        return false;

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        return false;

    return true;
}

static void quit_modules(void)
{
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_VideoQuit();
    SDL_AudioQuit();
    SDL_Quit();
}

// CORE APPLICATION LOOP
void game_loop(Data *data)
{
    uint32_t this_frame = 0;
    SDL_ShowWindow(data->window);
    while (data->status != CLOSING)
    {
        this_frame = get_time(&data->timer);

        // UPDATE STATE
        if (data->current_job.process == NULL && data->job_queue.number > 0)
        {
            Queueable queued;
            pop_queue(&data->job_queue, &queued);
            data->current_job = queued.job;
        }
        if (data->current_job.process != NULL)
            data->current_job.process(data);

        // HANDLE INPUT
        handle_events(data);
        switch (data->status)
        {
        case PLAYING:
            // if (data.last_frame + 100 < this_frame)
            //     update_state(&data);
            break;
        case PAUSED:
            break;
        case GAME_OVER:
            game_over(data);
            break;
        default:
            break;
        }

        // RENDERING
        SDL_RenderClear(data->renderer);
        for (int i = 0; i < 10; i++)
        {
            UI_Element *element = &data->user_interface[i];
            if (element->type == NO_UI_ELEMENT)
                break;
            if (element->capabilities & UI_RENDERING)
                element->render(data->renderer, element);
        }
        SDL_RenderPresent(data->renderer);
        this_frame = SDL_GetTicks() - data->timer.start_time + data->timer.pause_time;
        //if (data->last_frame > this_frame - MAX_FPS)
        //    SDL_Delay(data->last_frame + MAX_FPS - this_frame);
        data->last_frame = this_frame;
    }
}

int main(int argc, char *argv[])
{
    if (!init_modules())
        return 1;
    srand(time(NULL));
    Data data = {0};
    init_game(&data);
    game_loop(&data);
    free_game(&data);
    quit_modules();
    return 0;
}
